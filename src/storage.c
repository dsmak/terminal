#include "storage.h"

char* list_available_devices()
{
  if (tail != 0) // if list is not empty
  {
    static char answer[MAX_ANSWER_SIZE];
    int i = 0;
    int index = 0;
    for(i = 0; i < tail; i++)
    {
      index += sprintf(&answer[index], "%d\n", devices.uid[i]);
    }
    return answer;
  } else
  {
    return "List is empty";
  }
}

int add_device(int uid, char* details)
{
  //printf("add_device: tail %d\n", tail);
  if(uid >= STORAGE_SIZE || uid < 1)
  {
    return 0;
  }
  //printf("add_device: Device input id = %d\n it stores data to devices.uid[%d]\n", uid, uid -1);
  devices.uid[uid-1] = uid;
  //printf("add_device: Data in devices.uid[%d] is %d\n",uid - 1, devices.uid[uid-1]);
  memset(devices.details[uid-1], '\0', sizeof(devices.details[uid - 1])- 1);
  strcpy(devices.details[uid-1], details);
  //printf("add_device: Data in devices.details[%d] is %s\n with id: %d\n", uid-1, devices.details[uid-1], devices.uid[uid-1]);
  return -1;
}

char* get_device_details(int uid)
{
  //printf("-- get device details: %d id\n");
  if (uid <= 0 || uid > tail + 1 || uid == 0)
  {
    return "Invalid request!\n";
  }
  else
  {
    return devices.details[uid - 1];
  }
}

int get_available_id()
{
  int id = tail + 1;
  ++tail;
  return id;
}
