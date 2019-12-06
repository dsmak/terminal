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

    UnityIgnore( (("Done")), (UNITY_UINT)(38));



    tail = 0;

    int id = 0;



    pthread_attr_init(&attr);

    pthread_create(&tid_id, &attr, get_available_id, &id);

    pthread_join(tid_id, 

                        ((void *)0)

                            );



    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((id)), (

   ((void *)0)

   ), (UNITY_UINT)(47), UNITY_DISPLAY_STYLE_INT);



    pthread_create(&tid_id, &attr, get_available_id, &id);

    pthread_join(tid_id, 

                        ((void *)0)

                            );



    UnityAssertEqualNumber((UNITY_INT)((2)), (UNITY_INT)((id)), (

   ((void *)0)

   ), (UNITY_UINT)(52), UNITY_DISPLAY_STYLE_INT);



    pthread_create(&tid_id, &attr, get_available_id, &id);

    pthread_join(tid_id, 

                        ((void *)0)

                            );



    UnityAssertEqualNumber((UNITY_INT)((3)), (UNITY_INT)((id)), (

   ((void *)0)

   ), (UNITY_UINT)(57), UNITY_DISPLAY_STYLE_INT);



    pthread_create(&tid_id, &attr, get_available_id, &id);

    pthread_join(tid_id, 

                        ((void *)0)

                            );



    UnityAssertEqualNumber((UNITY_INT)((4)), (UNITY_INT)((id)), (

   ((void *)0)

   ), (UNITY_UINT)(62), UNITY_DISPLAY_STYLE_INT);

}





void test_storage_add_device(void)

{

   UnityIgnore( (("Done")), (UNITY_UINT)(68));



   char* test = "test";



   struct args *msg = (struct args *)malloc(sizeof(struct args));

   msg->uid = 2;

   strcpy(msg->details, test);



   pthread_create(&tid_add, 

                           ((void *)0)

                               , add_device, (void*)msg);

   pthread_join(tid_add, 

                        ((void *)0)

                            );





   UnityAssertEqualNumber((UNITY_INT)((2)), (UNITY_INT)((devices.uid[1])), (

  ((void *)0)

  ), (UNITY_UINT)(80), UNITY_DISPLAY_STYLE_INT);

   UnityAssertEqualString((const char*)(("test")), (const char*)((devices.details[1])), (

  ((void *)0)

  ), (UNITY_UINT)(81));



   UnityAssertEqualString((const char*)(("test")), (const char*)((msg->details)), (

  ((void *)0)

  ), (UNITY_UINT)(83));

}







void test_storage_list_devices_should_ReturnError(void)

{

   UnityIgnore( (("Done")), (UNITY_UINT)(90));

   char* list = malloc(512);

   memset(list,'\0', sizeof(list));

   pthread_attr_init(&attr);

   pthread_create(&tid_list, &attr, list_available_devices, list);

   pthread_join(tid_list, 

                         ((void *)0)

                             );





   UnityAssertEqualString((const char*)(("List is empty\n")), (const char*)((list)), (

  ((void *)0)

  ), (UNITY_UINT)(98));

}





void test_storage_list_devices_One_and_Many(void)

{

   UnityIgnore( (("Done")), (UNITY_UINT)(104));

   int id = 0;

   tail = 0;

   void* status = 0;

   pthread_attr_init(&attr);

   pthread_create(&tid_id, &attr, get_available_id, &id);

   pthread_join(tid_id, &status);



   UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((id)), (

  ((void *)0)

  ), (UNITY_UINT)(112), UNITY_DISPLAY_STYLE_INT);



   devices.uid[0] = id;



   char* list = malloc(512);

   memset(list,'\0', sizeof(list));

   pthread_attr_init(&attr);

   pthread_create(&tid_list, &attr, list_available_devices, list);

   pthread_join(tid_list, &status);

   UnityAssertEqualString((const char*)(("1\n")), (const char*)((list)), (

  ((void *)0)

  ), (UNITY_UINT)(121));

}





void test_storage_get_device_details_should_ReturnError(void)

{



    struct args* msg = (struct args*)malloc(sizeof(struct args));

    msg->uid = 0;



    pthread_attr_init(&attr);

    pthread_create(&tid_list, &attr, list_available_devices, (void*)msg);

    pthread_join(tid_list, 

                          ((void *)0)

                              );





    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((((struct args*) msg)->details)), (

   ((void *)0)

   ), (UNITY_UINT)(136));



    msg->uid = -10;



    pthread_attr_init(&attr);

    pthread_create(&tid_list, &attr, list_available_devices, (void*)msg);

    pthread_join(tid_list, 

                          ((void *)0)

                              );



    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((((struct args*) msg)->details)), (

   ((void *)0)

   ), (UNITY_UINT)(144));



    msg->uid = 512 + 1;



    pthread_attr_init(&attr);

    pthread_create(&tid_list, &attr, list_available_devices, ((struct args*) msg)->details);

    pthread_join(tid_list, 

                          ((void *)0)

                              );





    UnityAssertEqualString((const char*)(("Invalid request!\n")), (const char*)((msg->details)), (

   ((void *)0)

   ), (UNITY_UINT)(153));

}
