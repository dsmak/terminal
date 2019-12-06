#include "global.h"

// Structure for passing arguments to the threaded functions
struct args {
   int uid;
   char details[MAX_ANSWER_SIZE];
};
// structure to imitate DB
struct Devices {
  int uid[STORAGE_SIZE];
  char details[STORAGE_SIZE][MAX_JSON_SIZE];
} devices;

static int tail = 0;

// Lists ID of available devices
// returns list of available IDs
void *list_available_devices(void* list);

// Returns device details by UID
// If does not exist - return message
void *get_device_details(void *struct_deviceID_details);

// Adds device to a list
// unique ID is added and details is json-c string
// returns 0 if failed
// does not give error descriptions, could have been done
void *add_device(void *struct_deviceID_details);

// returns next available id
// simply next unused number
// could be done differently, i.e. generating random number
// and checking for being unique, but KISS 
void *get_available_id(void* id);

// Functions to delete the device 
// and to print all devices with attached data
// could be useful 
// but they are out of scope
