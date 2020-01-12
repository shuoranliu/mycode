#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_BUF_LEN 1024

struct char_stack {
    char buf[MAX_BUF_LEN];
    int top;
};

void init_char_stack(struct char_stack *cs)
{
    memset(cs->buf, 0, MAX_BUF_LEN);
    cs->top = 0;
}

int push_char_stack(struct char_stack *cs, char c)
{
    if (cs->top >= MAX_BUF_LEN) {
        return -1;
    }

    cs->buf[cs->top] = c;
    cs->top++;
    return cs->top;
}

char pop_char_stack(struct char_stack *cs)
{
    char ret;

    if (cs->top <= 0)
        return 0;

    cs->top--;
    ret = cs->buf[cs->top];    
    cs->buf[cs->top] = 0;
    return ret;
}

char * stack_to_string(struct char_stack *buf, int reverse)
{
    int len = buf->top;
    char *str= malloc(len+1);
    int i;

    if (reverse) {
        for (i = 0; i < len; i++) {
            str[i] = pop_char_stack(buf);
        }
    } else {
        for (i = len-1; i >= 0; i--) {
            str[i] = pop_char_stack(buf);
        }
    }
    str[len] = '\0';
    return str;
}

void parse_value(char *token)
{
    int len = strlen(token);
    char *key = malloc(len);
    char *value = malloc(len);
    int i, j;

    for (i = 0; i < len; i++) {
        if (token[i] == '=') {
            key[i] = '\0';
            break;
        }
        key[i] = token[i];
    }

    for (j = 0; j < len - i; j++) {
        value[j] = token[j+i+1];
    }
    value[j] = '\0';

    printf("key: %s, value: %s\n", key, value);
    free(key);
    free(value);
}

void parse_token(char *source)
{
    char *token = strtok(source, ",");

    while (token != NULL) {
        parse_value(token);
        token = strtok(NULL, ",");
    }

}

void parse(char source[])
{
    int i;
    struct char_stack cs;

    init_char_stack(&cs);

    for (i = 0; i < strlen(source); i++) {
        if (source[i] != ']') {
            push_char_stack(&cs, source[i]);
            continue;
        }

        struct char_stack token_buf;
        struct char_stack key_buf;
        char c;

        init_char_stack(&token_buf);
        while ((c = pop_char_stack(&cs)) != '[') {
            push_char_stack(&token_buf, c);
        }

        char *token= stack_to_string(&token_buf, 1);

        parse_token(token);

        init_char_stack(&key_buf);
        // skip to comma
        while ((c = pop_char_stack(&cs)) != ',') {
            if (c == '=')
                continue;
            if (c == '[')
                break;
            push_char_stack(&key_buf, c);
        }

        char *key = stack_to_string(&key_buf, 1);
        printf("key: %s\n", key);
        free(key);
    }

    char *remain = stack_to_string(&cs, 0);
    parse_token(remain);
    free(remain);
}

int main(void)
{
    int error;
    int start = 0;

    char source[] = "key1=value1,key2=value2,key3=[key4=value4,key5=value5,key6=[key7=value7]],key8=value8";
    //char source[] = "key6=[key7=value7],key8=value8";

    parse(source);

    return 1;
}
