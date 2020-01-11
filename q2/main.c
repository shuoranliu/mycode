#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "hashmap.h"

#define KEY_MAX_LENGTH (256)
typedef struct data_struct_s
{
    char key_string[KEY_MAX_LENGTH];
    const char *val;
} data_struct_t;

map_t mymap;

void parse_value(char *source)
{
    char *key = malloc(256);
    char *value= malloc(256);
    char *token= strtok(source, "=");

    while (token != NULL) {
        if (token[0] == '\[') {
            token = strtok(NULL, "=");
            strncpy(key, token+1, strlen(token));
        } else {
            strncpy(key, token, strlen(token));
            token = strtok(NULL, ",");
            if (token[0] == '\[') {
                continue;
            } else {
                strncpy(value, token, strlen(token));
            }
            token = strtok(NULL, "=");
        }
        printf("key: %s, value: %s\n", key, value);
    }
    printf("key: %s, value: %s\n", key, value);
}

int main(void)
{
    data_struct_t* value;
    char key_string[KEY_MAX_LENGTH];
    int error;

    char source[] = "key1=value1,key2=value2,key3=[key4=value4,key5=value5,key6=[key7=value7]],key8=value8";

    mymap = hashmap_new();

    parse_value(source);

//    char *token = strtok(source, ",");
//
//    while (token != NULL) {
//        printf("token: %s\n", token);
//        token = strtok(NULL, ",");
//    }

    value = malloc(sizeof(data_struct_t));
    snprintf(value->key_string, KEY_MAX_LENGTH, "%s", "key1");
    value->val = "value1";


    hashmap_put(mymap, value->key_string, value);

    snprintf(key_string, KEY_MAX_LENGTH, "%s", "key1");
    error = hashmap_get(mymap, key_string, (void**)(&value));

    printf("value is %s\n", value->val);

    free(value);
    hashmap_free(mymap);
}
