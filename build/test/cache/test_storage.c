#include "build/temp/_test_storage.c"
#include "src/storage.h"
#include "/var/lib/gems/2.3.0/gems/ceedling-0.29.1/vendor/unity/src/unity.h"


const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";



void setUp(void)

{

}



void tearDown(void)

{

}



void test_storage_list_devices_should_ReturnError(void)

{



     UnityAssertEqualString((const char*)(("List is empty")), (const char*)((list_available_devices())), (

    ((void *)0)

    ), (UNITY_UINT)(18));

}



void test_storage_get_device_details_should_ReturnError(void)

{

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(1))), (

   ((void *)0)

   ), (UNITY_UINT)(23));

}



void test_storage_should_ReturnOne(void)

{



    devices.uid[1] = 1;

    memset(devices.details[1], '\0', sizeof(devices.details[0]));

    strncpy(devices.details[1], test_string, strlen(test_string));

    tail=2;

    UnityAssertEqualString((const char*)(("1")), (const char*)((list_available_devices())), (

   ((void *)0)

   ), (UNITY_UINT)(33));

}
