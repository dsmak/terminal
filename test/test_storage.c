#include "unity.h"
#include "string.h"
#include "storage.h"
// NOTE: TESTS HAS TO BE RUN SEQUENTIALLY DUE TO SHARED RESOURCES
//
const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";
/*
COVER 
get_available_id
  - check if id is correct +

add_device:
  - adds device to the storage struct, id and details +
  - fails to add device
 
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
   //printf("Setup: devices.uid[0] at %p\n", &devices.uid[0]);
   //memset(devices.uid, 0, STORAGE_SIZE);
   //for(int i = 0; i < STORAGE_SIZE; i++)
   //{
   //  memset(devices.details[i], '\0', MAX_JSON_SIZE);
   //}
   //tail = 1;
}

void tearDown(void)
{
//   tail = 0;
}
// Checks for the next available id +
void test_storage_get_available_id(void)
{
//    TEST_IGNORE_MESSAGE("Done");
    tail = 0; // ID must be tail + 1;
    TEST_ASSERT_EQUAL_INT(1, get_available_id());    
    TEST_ASSERT_EQUAL_INT(2, get_available_id());    
    TEST_ASSERT_EQUAL_INT(3, get_available_id());    
    TEST_ASSERT_EQUAL_INT(4, get_available_id());    
}

// Checks if device is added properly
void test_storage_add_device(void)
{
//   TEST_IGNORE_MESSAGE("Done");
   char *test = "test";
   add_device(2, test);
   TEST_ASSERT_EQUAL_INT(2,devices.uid[1]);
   TEST_ASSERT_EQUAL_STRING("test", devices.details[1]);
} 

void test_storage_add_device_failure(void)
{
//   TEST_IGNORE_MESSAGE("Done");
   TEST_ASSERT_EQUAL_INT(0, add_device(-1, "test"));              //  Invalid ID 
   TEST_ASSERT_EQUAL_INT(0, add_device(0,"test"));               //  ***
   TEST_ASSERT_EQUAL_INT(0, add_device(STORAGE_SIZE,"test"));   //  ***
   TEST_ASSERT_EQUAL_INT(-1,add_device(5, "test"));            //  Valid ID
}

// Checks if empty list is returned properly +
void test_storage_list_devices_should_ReturnError(void)
{
//   TEST_IGNORE_MESSAGE("Done");
   TEST_ASSERT_EQUAL_STRING("List is empty", list_available_devices());
}


// Checks if list with one and many entries returns properly
void test_storage_list_devices_One_and_Many(void)
{
   //TEST_IGNORE_MESSAGE("Done");
   devices.uid[0] = get_available_id();
   TEST_ASSERT_EQUAL_STRING("1\n", list_available_devices());
   devices.uid[1] = get_available_id();
   devices.uid[2] = get_available_id();
   devices.uid[3] = get_available_id();
   devices.uid[4] = get_available_id();
   TEST_ASSERT_EQUAL_STRING("1\n2\n3\n4\n5\n", list_available_devices());
}

// checks if Invalid request is returned correctly +
void test_storage_get_device_details_should_ReturnError(void)
{
//    TEST_IGNORE_MESSAGE("Done");
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(-10));
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(0));
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(STORAGE_SIZE + 1));
}

