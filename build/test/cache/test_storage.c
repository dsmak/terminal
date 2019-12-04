#include "build/temp/_test_storage.c"
#include "src/storage.h"
#include "/var/lib/gems/2.3.0/gems/ceedling-0.29.1/vendor/unity/src/unity.h"




const char* string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";



void setUp(void)

{

}



void tearDown(void)

{

}



void test_storage_should_ReturnError(void)

{



     UnityAssertEqualString((const char*)(("List is empty")), (const char*)((list_available_devices())), (

    ((void *)0)

    ), (UNITY_UINT)(18));

}



void test_storage_should_ReturnOne(void)

{



    add_device(1,"Something");

    add_device(2,"Something2");

    add_device(2,"Something3");

    printf("WHAT - %d %s", devices.uid[2], devices.details[2]);

    UnityAssertEqualString((const char*)(("1")), (const char*)((list_available_devices())), (

   ((void *)0)

   ), (UNITY_UINT)(28));

}
