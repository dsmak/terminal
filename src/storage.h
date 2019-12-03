#include "global.h"

// structure to imitate DB
static struct Devices {
  int uid[STORAGE_SIZE];
  char details[STORAGE_SIZE][MAX_JSON_SIZE];
} devices;

static int tail = 0;

// Lists ID of available devices
// returns list of available IDs
const char* list_available_devices();

// Returns device details by UID
// If does not exist - return message
char* get_device_details(int uid);

// Adds device to a list
// where uid is unique ID and details is json-c string
// returns 0 if failed, i.e. input json is too large
// does not give error descriptions, could have been done
int add_device(int uid, char* details);

// returns next available id
// simply next unused number
// could be done differently, i.e. generating random number
// and checking for being unique, but KISS 
int get_available_id();

// used to fethch integer id's from the storage structure
// and put them into a c string
const char * get_string();

// Functions to delete the device 
// and to print all devices with attached data
// could be useful 
// but they are out of scope
