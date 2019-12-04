#include "build/temp/_test_storage.c"
#include "src/storage.h"
#include "/var/lib/gems/2.3.0/gems/ceedling-0.29.1/vendor/unity/src/unity.h"


const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";

void setUp(void)

{



   printf("devices.uid[0] at %p\n", &devices.uid[0]);

   memset(devices.uid, 0, 512);

   for(int i = 0; i < 512; i++)

   {

     memset(devices.details[i], '\0', 512);

   }

   tail = 0;

}



void tearDown(void)

{

   tail = 0;

}



void test_storage_list_devices_should_ReturnError(void)

{



     UnityAssertEqualString((const char*)(("List is empty")), (const char*)((list_available_devices())), (

    ((void *)0)

    ), (UNITY_UINT)(46));

}



void test_storage_get_device_details_should_ReturnError(void)

{

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(-1))), (

   ((void *)0)

   ), (UNITY_UINT)(51));

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(0))), (

   ((void *)0)

   ), (UNITY_UINT)(52));

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(1000))), (

   ((void *)0)

   ), (UNITY_UINT)(53));

}



void test_storage_should_ReturnOne(void)

{

    tail = 1;

    devices.uid[0] = 1;

    printf("Devices first uid: %d at %p\n", devices.uid[0], &devices.uid[0]);

    strncpy(devices.details[0], test_string, strlen(test_string));

    char *output = get_string();

    printf("Output: %s\n", output);

    UnityAssertEqualString((const char*)(("1")), (const char*)((get_string())), (

   ((void *)0)

   ), (UNITY_UINT)(64));

}



void test_storage_get_available_id(void)

{

    tail = 0;

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((2)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(71), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(72), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((4)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(73), UNITY_DISPLAY_STYLE_INT);

}
