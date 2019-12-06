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

}

void tearDown(void)
{
//   tail = 0;
}

// Checks for the next available id +
void test_storage_get_available_id(void)
{
    TEST_IGNORE_MESSAGE("Done");

    tail = 0; // ID must be tail + 1;
    int id = 0;

    pthread_attr_init(&attr);
    pthread_create(&tid_id, &attr, get_available_id, &id);
    pthread_join(tid_id, NULL);

    TEST_ASSERT_EQUAL_INT(1, id);

    pthread_create(&tid_id, &attr, get_available_id, &id);
    pthread_join(tid_id, NULL);

    TEST_ASSERT_EQUAL_INT(2, id);

    pthread_create(&tid_id, &attr, get_available_id, &id);
    pthread_join(tid_id, NULL);

    TEST_ASSERT_EQUAL_INT(3, id);

    pthread_create(&tid_id, &attr, get_available_id, &id);
    pthread_join(tid_id, NULL);

    TEST_ASSERT_EQUAL_INT(4, id);    
}

//Checks if device is added properly
void test_storage_add_device(void)
{
   TEST_IGNORE_MESSAGE("Done");

   char* test = "test";
   // get device details
   struct args *msg = (struct args *)malloc(sizeof(struct args));
   msg->uid = 2;
   strcpy(msg->details, test);
   // adds device to the storage
   pthread_create(&tid_add, NULL, add_device, (void*)msg);
   pthread_join(tid_add, NULL);

   // test if added correctly
   TEST_ASSERT_EQUAL_INT(2,devices.uid[1]);
   TEST_ASSERT_EQUAL_STRING("test", devices.details[1]);
   // Test if returned properly
   TEST_ASSERT_EQUAL_STRING("test", msg->details);
} 


// Checks if empty list is returned properly +
void test_storage_list_devices_should_ReturnError(void)
{
   TEST_IGNORE_MESSAGE("Done");
   char* list = malloc(MAX_ANSWER_SIZE);
   memset(list,'\0', sizeof(list));
   pthread_attr_init(&attr);
   pthread_create(&tid_list, &attr, list_available_devices, list);
   pthread_join(tid_list, NULL);

   //list_available_devices(list);
   TEST_ASSERT_EQUAL_STRING("List is empty\n", list);
}

// Checks if list with one and many entries returns properly
void test_storage_list_devices_One_and_Many(void)
{
   TEST_IGNORE_MESSAGE("Done");
   int id = 0;
   tail = 0;
   void* status = 0;
   pthread_attr_init(&attr);
   pthread_create(&tid_id, &attr, get_available_id, &id);
   pthread_join(tid_id, &status);
 
   TEST_ASSERT_EQUAL_INT(1, id);

   devices.uid[0] = id;   

   char* list = malloc(MAX_ANSWER_SIZE);
   memset(list,'\0', sizeof(list));
   pthread_attr_init(&attr);
   pthread_create(&tid_list, &attr, list_available_devices, list);
   pthread_join(tid_list, &status);
   TEST_ASSERT_EQUAL_STRING("1\n", list);
}

// checks if Invalid request is returned correctly
// FAILS !!!
void test_storage_get_device_details_should_ReturnError(void)
{
    //TEST_IGNORE_MESSAGE("Done");
    struct args* msg = (struct args*)malloc(sizeof(struct args));
    msg->uid = 0;
    
    pthread_attr_init(&attr);
    pthread_create(&tid_list, &attr, list_available_devices, (void*)msg);
    pthread_join(tid_list, NULL);


    TEST_ASSERT_EQUAL_STRING("Invalid request!\n",((struct args*) msg)->details);
    
    msg->uid = -10;

    pthread_attr_init(&attr);
    pthread_create(&tid_list, &attr, list_available_devices, (void*)msg);
    pthread_join(tid_list, NULL);

    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", ((struct args*) msg)->details);

    msg->uid = STORAGE_SIZE + 1;

    pthread_attr_init(&attr);
    pthread_create(&tid_list, &attr, list_available_devices, ((struct args*) msg)->details);
    pthread_join(tid_list, NULL);


    TEST_ASSERT_EQUAL_STRING("Invalid request!\n", msg->details);
}

