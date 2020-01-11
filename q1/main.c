#include <stdio.h>
#include "rbtree.h"
#include "map.h"

#define N 3

root_t tree = RB_ROOT;

struct sort_array
{
    int* p;
    int n;
};

void init_sort_array(struct sort_array *sa, int *p, int n)
{
    sa->p = p;
    sa->n = n;
}

void free_sort_array(struct sort_array *sa)
{
    if (sa != NULL)
        free(sa);
}

void iterate_array(struct sort_array *sa, int target)
{
    int i;
    int *p = sa->p;

    if (p == NULL)
        return;

    for (i = 0; i < sa->n; i++) {
        map_t *data = get(&tree, p[i]);
        if (data != NULL) {
            if (data->val != target)
                continue;
            data->val++;
        }
    }
}

void andMerge(struct sort_array * source[])
{
    int i;
    int base = 0;
    int  shortest = 0;
    struct sort_array *result = malloc(sizeof(struct sort_array));//FIXME

    // Find the shortest array
    for (i = 0; i < N; i++) {
        if (source[i]->n < shortest) {
            base = i;
            shortest = source[i]->n;
        }
    }

    for (i = 0; i < source[base]->n; i++) {
        put(&tree, source[base]->p[i], 0);
    }

    // Iterate each array and result is stored in the global variable tree
    for (i = 0; i < N; i++) {
        iterate_array(source[i], i);
    }
}

int main(void) {
    int a[] = {1,3,4,6,7,8,9,10};
    int b[] = {2,4,6,8,10};
    int c[] = {5,6,7,8,9,11,12,13,14};
    struct sort_array *p[N];
    int i;
    
    // FIXME: read from files or something
    p[0] = malloc(sizeof(struct sort_array));
    init_sort_array(p[0], a, sizeof(a)/sizeof(a[0]));
    p[1] = malloc(sizeof(struct sort_array));
    init_sort_array(p[1], b, sizeof(b)/sizeof(b[0]));
    p[2] = malloc(sizeof(struct sort_array));
    init_sort_array(p[2], c, sizeof(c)/sizeof(c[0]));
   
    // Merge
    andMerge(p);

    // Output
    map_t *node;
    for (node = map_first(&tree); node; node=map_next(&(node->node))) {
        if (node->val != N)
            continue;
        printf("%d\n", node->key);
    }
    

    // Clean resource
    for (i = 0; i < N; i++) {
        free_sort_array(p[i]);
    }

    return 0;
}
