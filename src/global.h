#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <pthread.h>

#define PORT 8888

#define STORAGE_SIZE 512
#define POSTBUFFER_SIZE 512
#define MAX_JSON_SIZE 512
#define MAX_ANSWER_SIZE 512
#define REST_API_CALL "/devices"
#define REST_API_LENGTH 8 
#define GET 0
#define POST 1

pthread_t tid_list, tid_add, tid_id, tid_get ;
pthread_attr_t attr;
pthread_mutex_t lock;

