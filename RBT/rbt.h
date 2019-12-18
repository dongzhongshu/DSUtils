//
// Created by Administrator on 2019/12/16.
//

#ifndef DSUTILS_RBT_H
#define DSUTILS_RBT_H
typedef int KEY_TYPE;
typedef int VALUE_TYPE;
#define BLACK 0
#define RED 1
typedef struct  _RBT_VALUE {
    KEY_TYPE key;
    VALUE_TYPE value;
}RBT_VALUE;
#define RBT_ENTRY(name, type) struct name{  \
    struct type* left;  \
    struct type* right; \
    struct type* parent;    \
}
typedef struct _rbtree_node {
    unsigned char color;
    RBT_ENTRY(,_rbtree_node)  rbelement;
    RBT_VALUE value;
} rbtree_node;

typedef struct _rbtree{
    rbtree_node* root;
    rbtree_node* nil;
}rbtree;
void rbtree_route_left(rbtree* T, rbtree_node* x);
void rbtree_route_right(rbtree* T, rbtree_node* y);
int rbtree_insert(rbtree* T, RBT_VALUE value);
void rbtree_insert_fix(rbtree* T, rbtree_node* node);
rbtree_node* rbtree_max(rbtree* T, rbtree_node* x);
rbtree_node* rbtree_min(rbtree* T, rbtree_node* y);
rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x);
rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z);
void rbtree_delete_fixup(rbtree *T, rbtree_node *x);
rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key);
void rbtree_traversal(rbtree *T, rbtree_node *node);
#endif //DSUTILS_RBT_H
