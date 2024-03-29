#include "global.h"
#include "server.h"

static int send_details(struct MHD_Connection *connection, const char *terminal_details)
{
    int ret;
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer (strlen(terminal_details) +1 ,
                                                (void*) terminal_details,
                                                MHD_RESPMEM_PERSISTENT);
    if(!response)
    {
        return MHD_NO;
    } 
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    
    return ret;
}

static int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                        const char *filename, const char *content_type,
                        const char *transfer_encoding, const char *data, uint64_t off,
                        size_t size)
{
    struct connection_info_struct *con_info = coninfo_cls;
    
    if ( 0 == strcmp(key,"json"))
    {
        if((size > 0) && (size <= MAX_JSON_SIZE))
        {
            char *answer_string;
            
            answer_string = malloc(MAX_JSON_SIZE);
            if (!answer_string)
            {
                return MHD_NO;
            }
            
            snprintf(answer_string, MAX_JSON_SIZE - 1, "%s", data);
            
            con_info->answer_string = answer_string;
        } else
        {
            con_info->answer_string = NULL;
            return MHD_NO;
        }
    }
    return MHD_YES;
}

static void request_completed (void *cls, struct MHD_Connection *connection,
                               void **con_cls, enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *con_info = *con_cls;
    
    if (NULL == con_info)
    {
        return;
    }
    
    if (con_info->connection_type == POST)
    {
        MHD_destroy_post_processor (con_info->postprocessor);
        
        if (con_info->answer_string)
        {
            free(con_info->answer_string);
        }
    }
    free (con_info);
    *con_cls = NULL;
}

static int answer_to_connection (void *cls, struct MHD_Connection *connection,
                                 const char *url, const char *method,
                                 const char *version, const char *upload_data,
                                 size_t *upload_data_size, void **con_cls)
{
    if ( NULL == *con_cls) // if it is first iteration of the new request
    {
	// set up connection structure
        struct connection_info_struct *con_info;
        con_info = malloc(sizeof(struct connection_info_struct));
        if (NULL == con_info)
        {
            return MHD_NO;
        }
        
        con_info->answer_string = NULL;
        // if new request is POST, create postprocessor
        if (0 == strcmp (method, "POST"))
        {
	    //printf("%s\n", "Received POST request");
            
	    if (!strncasecmp(url, REST_API_CALL, REST_API_LENGTH))
            {
                con_info->postprocessor =
                        MHD_create_post_processor( connection, POSTBUFFER_SIZE,
                                                   iterate_post, (void *)con_info);
                if (NULL == con_info->postprocessor)
                {
                    free (con_info);
                    return MHD_NO;
                }
                con_info->connection_type = POST;
            } else
            {
                return MHD_NO;
            }
        } else
        {
            con_info->connection_type = GET;
        }
        *con_cls = (void*)con_info;
        return MHD_YES;
    }
    // if the new request is GET
    if (0 == strcmp (method, "GET"))
    {

	//printf("%s\n", "Received GET request");
        // Terminal id can not be less than 1, therefore if terminal_id = 0
        // Then output is the list of available devices
        // Not good from security point of view, but easy and intuitive 
        int terminal_id;
        
        terminal_id = strcasecmp(url, REST_API_CALL);
        
        if(0 == terminal_id)
        {
            char* list = malloc(MAX_ANSWER_SIZE);
            memset(list,'\0', sizeof(list));
            pthread_attr_init(&attr);
            pthread_create(&tid_list, &attr, list_available_devices, list);
            pthread_join(tid_list, NULL);
            //printf("Server: %s\n", list);
            terminal_id = send_details(connection, list);
        }
        else if (0 < terminal_id)
        {
            terminal_id = atoi(url+(REST_API_LENGTH)+1);
            if(0 != terminal_id)
            {
                // get device details
		struct args* msg = malloc(sizeof(struct args));
		msg->uid = terminal_id;
                memset(msg->details, '\0', MAX_ANSWER_SIZE);

                pthread_create(&tid_get, NULL, get_device_details,(void*)msg);
                pthread_join(tid_get, NULL);
                
		//printf("Server: %d details:\n %s\n", msg->uid, ((struct args*)msg)->details);

                terminal_id = send_details(connection,((struct args*)msg)->details);
		free(msg);
            } else
            {
                return MHD_NO;
            }
        } else
        {
          return MHD_NO;
        }
        
        
    }
    // for second and following iterations of POST request 
    if (0 == strcmp (method, "POST"))
    {
       //printf("%s\n", "working on POST request");
       if (!strncasecmp(url, REST_API_CALL, REST_API_LENGTH))
       {
	 struct connection_info_struct *con_info = *con_cls;
  
         if (*upload_data_size != 0)
	 {
		MHD_post_process (con_info->postprocessor, upload_data,
				  *upload_data_size);
                *upload_data_size = 0;
		return MHD_YES;
	 } 
	else if (NULL != con_info->answer_string)
	{
	// assign ID here
        int uid = 0;

        pthread_attr_init(&attr);
        pthread_create(&tid_id, &attr, get_available_id, &uid); // gets available ID, from "storage.h"
        pthread_join(tid_id, NULL);
        //printf("%s %d\n", "Server: new id is: ", uid);
	char* details_with_id = attach_id(con_info->answer_string, uid); // attaches ID, from "json_processor.h"
        // get device details
	struct args *msg = (struct args *)malloc(sizeof(struct args));
        msg->uid = uid;
        strcpy(msg->details, details_with_id);
        // adds device to the storage
        pthread_attr_init(&attr);
        pthread_create(&tid_add, &attr, add_device, (void*)msg);
        pthread_join(tid_add, NULL);

	con_info->answer_string = details_with_id; // saving new details for response
        //printf("Server: \n%s\n", con_info->answer_string);
	send_details(connection, con_info->answer_string);
        free(msg);

        }

      }

    }
}

int main ()
{
    struct MHD_Daemon *daemon;
    
    printf("%s\n", "Start server ");
    
    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                               &answer_to_connection, NULL,
                               MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                               NULL, MHD_OPTION_END);
    if (NULL == daemon)
    {
        printf ("failure in MHD_start_daemon().\n");
        return 1;
    }
    
    (void) getchar ();
    
    MHD_stop_daemon (daemon);
    return 0;
}

