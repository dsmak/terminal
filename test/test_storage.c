#include "unity.h"

#include "storage.h"

const char* string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";

void setUp(void)
{
}

void tearDown(void)
{
}

void test_storage_should_ReturnError(void)
{
     // Checks if list is empty
     TEST_ASSERT_EQUAL_STRING("List is empty", list_available_devices());
}

void test_storage_should_ReturnOne(void)
{
    // add device to the structure
    add_device(1,"Something");
    add_device(2,"Something2");
    add_device(2,"Something3");
    printf("WHAT - %d %s", devices.uid[2], devices.details[2]);
    TEST_ASSERT_EQUAL_STRING("1", list_available_devices());
}
