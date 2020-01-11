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

int parse_token(char *source, int start, int end);

void parse_value(char *source, int start, int end)
{
    int cursor = start;

    //printf("start=%d, end=%d\n", start, end);
    while (source[cursor] != '=' && cursor < end) {
        cursor++;
    }

//    char *key = malloc(cursor-start+1);
//    snprintf(key, cursor-start+1, "%s", source+start);

    char *value = malloc(end-cursor);
    snprintf(value, end-cursor, "%s", source+cursor+1);

//    printf("key=%s, value=%s\n", key, value);

    if (value[0] == '[') {
        int new_start = cursor+2;
        int new_end = end - 1;

        new_start = parse_token(source, new_start, new_end);
        while (source[new_start] != '\0' && new_start < new_end) {
            new_start = parse_token(source, new_start, new_end);
        }
    } else {
        // add to hashmap
        data_struct_t* object = malloc(sizeof(data_struct_t));
        snprintf(object->key_string, cursor-start+1, "%s", source+start);
        object->val = malloc(end-cursor);
        snprintf((char *)object->val, end-cursor, "%s", source+cursor+1);
        printf("key=%s, value=%s\n", object->key_string, (char *)object->val);
        hashmap_put(mymap, object->key_string, object);
    }

    free(value);
}

int parse_token(char *source, int start, int end)
{
    int remain = start;
    int array = 0;

    while (source[remain] != '\0' && remain < end) {
        if (source[remain] == ',' || source[remain] == '[' || source[remain] == ']') {
            if (source[remain] == ',') {
                if (array == 0) {
                    break;
                } else {
                    remain++;
                    continue;
                }
            }

            if (source[remain] == '[') {
                remain++;
                array++;
                continue;
            }

            remain++;
            array--;
            continue;
        }

        remain++;
    }

    if (remain == start)
        return remain;

    // parse value from start to remain
    //
    parse_value(source, start, remain);

    return remain+1;
}

int main(void)
{
    data_struct_t* value;
    char key_string[KEY_MAX_LENGTH];
    int error;
    int start = 0;

    char source[] = "key1=value1,key2=value2,key3=[key4=value4,key5=value5,key6=[key7=value7]],key8=value8";
    //char source[] = "key6=[key7=value7],key8=value8";

    mymap = hashmap_new();

    start = parse_token(source, start, strlen(source));
    while (source[start] != '\0' && start < strlen(source)) {
        start = parse_token(source, start, strlen(source));
    }

    //FIXME: free object
    hashmap_free(mymap);

    return 1;
}
