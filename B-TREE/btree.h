//
// Created by Administrator on 2019/12/20.
//

#ifndef DSUTILS_BTREE_H
#define DSUTILS_BTREE_H
typedef int KEY_TYPE;
typedef int VALUE_TYPE;
typedef struct _NODE_VALUE{
    KEY_TYPE key;
    VALUE_TYPE value;
}NODE_VALUE;
typedef struct _btree_node{
NODE_VALUE* keys;
struct _btree_node** childrens;
int keynum;
int leaf;
}btree_node;

typedef struct _btree{
    btree_node* root;
    int t;//度数,最大关键字数为[t-1,2t-1];最大childrens为[t,2t]
}btree;

btree_node* btree_create_node(int t, int leaf);
void btree_create(btree* T, int t);
int btree_insert(btree* T, NODE_VALUE value);
int btree_insert_nonfull(btree* T, btree_node* x, NODE_VALUE value);
int btree_split_child(btree* T, btree_node* x, int i);
int btree_delete_key(btree* T, btree_node* node, KEY_TYPE key);
int btree_delete(btree* T, KEY_TYPE key);
int btree_merge(btree* T, btree_node* node, int idx);
void btree_destroy_node(btree_node *node);
void btree_traverse(btree_node *x);
void btree_print(btree *T, btree_node *node, int layer);
int btree_bin_search(btree_node *node, int low, int high, KEY_TYPE key);
#endif //DSUTILS_BTREE_H
