#include "build/temp/_test_storage.c"
#include "src/storage.h"
#include "libs/Unity/src/unity.h"




const char* test_string = "\"{\"cardType\": [\"Visa\", \"MasterCard\", \"EFTPOS\" ], \"TransactionType\":[\"Cheque\", \"Savings\", \"Credit\"]}";

void setUp(void)

{

}



void tearDown(void)

{



}



void test_storage_get_available_id(void)

{



    tail = 0;

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((2)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(47), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((4)), (UNITY_INT)((get_available_id())), (

   ((void *)0)

   ), (UNITY_UINT)(49), UNITY_DISPLAY_STYLE_INT);

}





void test_storage_add_device(void)

{



   char *test = "test";

   add_device(2, test);

   UnityAssertEqualNumber((UNITY_INT)((2)), (UNITY_INT)((devices.uid[1])), (

  ((void *)0)

  ), (UNITY_UINT)(58), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualString((const char*)(("test")), (const char*)((devices.details[1])), (

  ((void *)0)

  ), (UNITY_UINT)(59));

}



void test_storage_add_device_failure(void)

{



   UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((add_device(-1, "test"))), (

  ((void *)0)

  ), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((add_device(0,"test"))), (

  ((void *)0)

  ), (UNITY_UINT)(66), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((add_device(512,"test"))), (

  ((void *)0)

  ), (UNITY_UINT)(67), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualNumber((UNITY_INT)((-1)), (UNITY_INT)((add_device(5, "test"))), (

  ((void *)0)

  ), (UNITY_UINT)(68), UNITY_DISPLAY_STYLE_INT);

}





void test_storage_list_devices_should_ReturnError(void)

{



   UnityAssertEqualString((const char*)(("List is empty")), (const char*)((list_available_devices())), (

  ((void *)0)

  ), (UNITY_UINT)(75));

}







void test_storage_list_devices_One_and_Many(void)

{



   devices.uid[0] = get_available_id();

   UnityAssertEqualString((const char*)(("1\n")), (const char*)((list_available_devices())), (

  ((void *)0)

  ), (UNITY_UINT)(84));

   devices.uid[1] = get_available_id();

   devices.uid[2] = get_available_id();

   devices.uid[3] = get_available_id();

   devices.uid[4] = get_available_id();

   UnityAssertEqualString((const char*)(("1\n2\n3\n4\n5\n")), (const char*)((list_available_devices())), (

  ((void *)0)

  ), (UNITY_UINT)(89));

}





void test_storage_get_device_details_should_ReturnError(void)

{



    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(-10))), (

   ((void *)0)

   ), (UNITY_UINT)(96));

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(0))), (

   ((void *)0)

   ), (UNITY_UINT)(97));

    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((get_device_details(512 + 1))), (

   ((void *)0)

   ), (UNITY_UINT)(98));

}
