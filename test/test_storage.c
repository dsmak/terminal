#include "unity.h"
#include "string.h"
#include "storage.h"

const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";
/*
COVER 
list_available_devices:
  - List is empty
  - List contains one value
  - List contains max number of values set in global.h

add_device:
  - adds device to the storage struct, id and details
 
get_device_details:
  - Check Invalid request
  - Check first, last and in the middle details added

get_string:
  - Check strings with different number of entries

get_available_id
  - check if id is correct
*/
void setUp(void)
{
}

void tearDown(void)
{
}

void test_storage_list_devices_should_ReturnError(void)
{
     // Checks if list is empty
     TEST_ASSERT_EQUAL_STRING("List is empty", list_available_devices());
}

void test_storage_get_device_details_should_ReturnError(void)
{
    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", get_device_details(1));
}

void test_storage_should_ReturnOne(void)
{
    // add device to the structure
    devices.uid[1] = 1;
    memset(devices.details[1], '\0', sizeof(devices.details[0]));
    strncpy(devices.details[1], test_string, strlen(test_string));
    tail=2;
    TEST_ASSERT_EQUAL_STRING("1", list_available_devices());
}


