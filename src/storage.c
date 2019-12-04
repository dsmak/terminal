#include "global.h"
#include "storage.h"

const char* list_available_devices()
{
  if (tail != 0) // if list is not empty
  {
    return get_string(); // returns the list of id's
  } else
  {
    return "List is empty";
  }
}

int add_device(int uid, char* details)
{
  devices.uid[tail] = uid;
  memset(devices.details[tail], '\0', sizeof(devices.details[tail]));
  strcpy(devices.details[tail], details);
  return -1;
}

char* get_device_details(int uid)
{
  if (0 > uid && tail + 1 < uid )
  {
    return "Invalid request!\n";
  }
  else
  {
    return devices.details[uid];
  }
}

const char * get_string()
{
  char * new_string;
  char * temp;
  // List of available ID's is limited to MAX_ANSWER_SIZE set in global.h
  // Could've been expansible, but I decided to leave it as it is
  // strNcat manages buffer overflow problem here
  new_string = (char*)malloc(MAX_ANSWER_SIZE);
  for(int i = 1; i <= tail; i++)
  {
    printf("Test: %d\n", devices.uid[i]);
    int length = snprintf(NULL, 0, "%d\n", devices.uid[i]);
    // convert integer (id) to string
    temp = malloc( length + 1);
    snprintf(temp, length + 1, "%d\n", devices.uid[i]);
    // append id to the list
    strncat(new_string, temp, MAX_ANSWER_SIZE - strlen(new_string) - 1);
  }
  return new_string;
}

int get_available_id()
{
  int id = tail + 1;
  ++tail;
  return id;
}