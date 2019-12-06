#include "storage.h"

// Structure for passing arguments to the threaded functions

void *list_available_devices(void* list)
{
  printf("%s","list_available_devices: Enter thread\n");
  if (tail != 0) // if list is not empty
  {
    char answer[MAX_ANSWER_SIZE];
    int i = 0;
    int index = 0;
    printf("%s","list_available_devices locks mutex\n");
    pthread_mutex_lock(&lock);
    for(i = 0; i < tail; i++)
    {
      index += sprintf(&answer[index], "%d\n", devices.uid[i]);
    }
    pthread_mutex_unlock(&lock);
    strncpy((char*)list, answer, strlen(answer) +1);
    printf("list_available_devices: list:  %s\n, where answer was %s",(char*)list, answer);
  } else
  {
    strcpy((char*)list, "List is empty\n");
  }
  printf("%s","list_available_devices: Exit thread\n");
  sleep(1);
  pthread_exit(list);
}

void *get_device_details(void *deviceID_details)
{
  int uid = ((struct args*)deviceID_details)->uid; 
  //printf("Get device details: %d id\n", uid);
  
  char* answer = malloc(MAX_ANSWER_SIZE);
  memset(answer,'\0',sizeof(answer)); 

  if (uid <= 0 || uid > tail)
  {
    sprintf(answer,"%s",
             "Invalid request!\n");
  }
  else
  {
    pthread_mutex_lock(&lock);
    strncpy(answer,
             devices.details[uid - 1], strlen(devices.details[uid-1] + 1));
    pthread_mutex_unlock(&lock);
  }
  strncpy(((struct args *)deviceID_details)->details, answer, strlen(answer)+1);
  free(answer);
  pthread_exit((void*)deviceID_details);
}

void *add_device(void *deviceID_details)
{
  int uid = ((struct args*)deviceID_details)->uid; 
  //printf("add_device: tail %d uid: %d\n", tail, uid);
  if(uid >= STORAGE_SIZE || uid < 1)
  {
    // supposed to handle error here
    printf("%s","add_device: Error!\n");
  } else 
  {
    pthread_mutex_lock(&lock);
    //printf("add_device: Device input id = %d\n it stores data to devices.uid[%d]\n", uid, uid -1);
    devices.uid[uid-1] = uid;
    //printf("add_device: Data in devices.uid[%d] is %d\n",uid - 1, devices.uid[uid-1]);
    memset(devices.details[uid-1], '\0', sizeof(devices.details[uid - 1])- 1);
    strcpy(devices.details[uid-1], ((struct args*)deviceID_details)->details);
    //printf("add_device: Data in devices.details[%d] is %s\n with id: %d\n", uid-1, devices.details[uid-1], devices.uid[uid-1]);
    pthread_mutex_unlock(&lock);
  }
  pthread_exit(deviceID_details);
}

void *get_available_id(void* id)
{
  pthread_mutex_lock(&lock);
  *(int*)id = tail + 1;
  ++tail;
  pthread_mutex_unlock(&lock);
  pthread_exit(id);
}
