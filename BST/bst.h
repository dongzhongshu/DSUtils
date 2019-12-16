//
// Created by Administrator on 2019/12/14.
//

#ifndef DSUTILS_BST_H
#define DSUTILS_BST_H
typedef int KEY_TYPE;
typedef int VALUE_TYPE;
typedef struct  _BST_VALUE {
    KEY_TYPE key;
    VALUE_TYPE value;
}BST_VALUE;
#define BST_ENTRY(name, type) struct name{  \
    struct type* left;  \
    struct type* right; \
}
typedef struct _BST_Node{
    BST_VALUE value;
    BST_ENTRY(,_BST_Node) bst;
}BST_Node, *PBST_Node;
typedef struct _BST_Tree{
    BST_Node *root;
}BST_Tree, *PBST_Tree;

PBST_Node bst_create_node(BST_VALUE value);
int bst_insert(PBST_Tree tree, BST_VALUE value);
int bst_delete(PBST_Tree tree,KEY_TYPE key);
int bst_delete_node(PBST_Tree tree, PBST_Node parent, PBST_Node currentnode);
VALUE_TYPE bst_find(PBST_Tree tree, KEY_TYPE key);
void bst_travel(PBST_Node);
#endif //DSUTILS_BST_H
