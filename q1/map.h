/***************************************************************************
 * 
 * Copyright (c) 2016 zkdnfcf, Inc. All Rights Reserved
 * $Id$ 
 * 
 **************************************************************************/
 
 /**
 * @file hash.h
 * @author zk(zkdnfc@163.com)
 * @date 2016/05/31 18:26:01
 * @version $Revision$ 
 * @brief 
 *  
 **/
#ifndef _MAP_H
#define _MAP_H

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct map {
    struct rb_node node;
    int key;
    int val;
};

typedef struct map map_t;
typedef struct rb_root root_t;
typedef struct rb_node rb_node_t;

map_t *get(root_t *root, int key) {
   rb_node_t *node = root->rb_node; 
   while (node) {
        map_t *data = container_of(node, map_t, node);

        //compare between the key with the keys in map
        if (key < data->key) {
            node = node->rb_left;
        }else if (key > data->key) {
            node = node->rb_right;
        }else {
            return data;
        }
   }
   return NULL;
}

int put(root_t *root, int key, int val) {
    map_t *data = (map_t*)malloc(sizeof(map_t));
    //FIXME: malloc fail?
    data->key = key;
    data->val = val;
    
    rb_node_t **new_node = &(root->rb_node), *parent = NULL;
    while (*new_node) {
        map_t *this_node = container_of(*new_node, map_t, node);
        parent = *new_node;

        if (key < this_node->key) {
            new_node = &((*new_node)->rb_left);
        }else if (key > this_node->key) {
            new_node = &((*new_node)->rb_right);
        }else {
            return 0;
        }
    }

    rb_link_node(&data->node, parent, new_node);
    rb_insert_color(&data->node, root);

    return 1;
}

map_t *map_first(root_t *tree) {
    rb_node_t *node = rb_first(tree);
    return (rb_entry(node, map_t, node));
}

map_t *map_next(rb_node_t *node) {
    rb_node_t *next =  rb_next(node);
    return rb_entry(next, map_t, node);
}

void map_free(map_t *node){
    if (node != NULL) {
        free(node);
        node = NULL;
    }
}

#endif  //_MAP_H

/* vim: set ts=4 sw=4 sts=4 tw=100 */
