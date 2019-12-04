#include "unity.h"
#include "string.h"
#include "storage.h"

const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";
/*
COVER 
get_available_id
  - check if id is correct +

add_device:
  - adds device to the storage struct, id and details +

get_string:
  - Check strings with different number of entries 
 
list_available_devices:
  - List is empty +
  - List contains one value +
  - List contains few values +

get_device_details:
  - Check Invalid request +
  - Check first, last and in the middle details added +
*/
void setUp(void)
{
   // set up empty storage
   printf("devices.uid[0] at %p\n", &devices.uid[0]);
   memset(devices.uid, 0, STORAGE_SIZE);
   for(int i = 0; i < STORAGE_SIZE; i++)
   {
     memset(devices.details[i], '\0', MAX_JSON_SIZE);
   }
   tail = 0;
}

void tearDown(void)
{
   tail = 0;
}
// Checks for the next available id
void test_storage_get_available_id(void)
{
    tail = 0; // ID must be tail + 1;
    TEST_ASSERT_EQUAL_INT(1, get_available_id());    
    TEST_ASSERT_EQUAL_INT(2, get_available_id());    
    TEST_ASSERT_EQUAL_INT(3, get_available_id());    
    TEST_ASSERT_EQUAL_INT(4, get_available_id());    
}

// Checks if device is added properly
void test_storage_add_device(void)
{
  tail = 0;
  int uid = get_available_id();
  char *test = "test";
  add_device(uid, test);
  TEST_ASSERT_EQUAL_INT(1,devices.uid[0]);
}

// Checks if empty list is returned properly
void test_storage_list_devices_should_ReturnError(void)
{
     TEST_ASSERT_EQUAL_STRING("List is empty", list_available_devices());
}

// checks get_string() function for one and many entries
void test_storage_should_ReturnOne(void)
{
    tail = 1;
    devices.uid[0] = 1;
    printf("Devices first uid: %d at %p\n", devices.uid[0], &devices.uid[0]);
    strncpy(devices.details[0], test_string, strlen(test_string)); 
    char *output = get_string();
    printf("Output: %s\n", output);
    TEST_ASSERT_EQUAL_STRING("1", get_string());
}
// Checks if list with one and many entries returns properly
void test_storage_list_devices_One_and_Many(void)
{
  devices.uid[0] = get_available_id(); // first value
  devices.details[0];
  strncpy(devices.details[0], test_string, strlen(test_string));
  TEST_ASSERT_EQUAL_STRING("1\n", list_available_devices());
  for(int i = 1; i < 6; i++)  // five more values
  {
    devices.uid[i] = get_available_id();
    strncpy(devices.details[i], test_string, strlen(test_string)); 
  }     
  TEST_ASSERT_EQUAL_STRING("1\n2\n3\n4\n5\n6\n", list_available_devices());
}

// checks if Invalid request is returned correctly
void test_storage_get_device_details_should_ReturnError(void)
{
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(-1));
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(0));
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(STORAGE_SIZE + 1));
}

