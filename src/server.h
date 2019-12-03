#include <microhttpd.h>
#include "global.h"
#include "json_processor.h"


// Structure that stores connection information for MHD
struct connection_info_struct
{
    int connection_type;
    char *answer_string;
    struct MHD_PostProcessor *postprocessor;
};

// Creates and sends MHD response, I could come up with a better function name
static int send_details(struct MHD_Connection *connection, const char *terminal_details);

// Iteration function is used by MHD post processor to iterate through POSTed chunks of data
static int iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                        const char *filename, const char *content_type,
                        const char *transfer_encoding, const char *data, uint64_t off,
                        size_t size);

// Request completed does clean up after request
static void request_completed (void *cls, struct MHD_Connection *connection,
                               void **con_cls, enum MHD_RequestTerminationCode toe);

// Function that serves particular connection and prepares the reply
// It contains the logic to serve POST and GET requests and responces
static int answer_to_connection (void *cls, struct MHD_Connection *connection,
                                 const char *url, const char *method,
                                 const char *version, const char *upload_data,
                                 size_t *upload_data_size, void **con_cls);

