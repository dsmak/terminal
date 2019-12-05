#include "global.h"
#include "../libs/json-c/json.h"

// attaches ID using json-c and returns new JSON string
// if input is not JSON returns NULL
char * attach_id(char* new_device_details, int uid);
