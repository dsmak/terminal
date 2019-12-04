#include "json_processor.h"

char * attach_id(char* new_device_details, int uid)
{
  // creating buffer
  char * new_json_with_id;
  struct json_object *parsed_json;
  // parsing json string to json-c
  parsed_json = json_tokener_parse(new_device_details);
  // adding id
  json_object_object_add(parsed_json, "UID", json_object_new_int(uid));
  // saves new json with id to the device details
  new_json_with_id = json_object_to_json_string_ext(parsed_json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY);
  // freeng space
  free(parsed_json);
  return new_json_with_id;
}

