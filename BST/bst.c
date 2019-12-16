//
// Created by Administrator on 2019/12/14.
//
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "bst.h"

PBST_Node bst_create_node(BST_VALUE value)
{
    PBST_Node node = (PBST_Node)calloc(1, sizeof(BST_Node));
    assert(node != NULL);
    node->value = value;
    node->bst.left = node->bst.right = NULL;
    return node;
}
int bst_insert(PBST_Tree tree, BST_VALUE value)
{
    if(tree == NULL){
        return -1;
    }
    if(tree->root == NULL){
        tree->root = bst_create_node(value);
        assert(tree->root != NULL);
        return 0;
    }
    PBST_Node node = tree->root;
    PBST_Node tmp = tree->root;
    while(node != NULL){
        tmp = node;
        if(value.key < node->value.key){
            node = node->bst.left;
        }else{
            node = node->bst.right;
        }
    }
    if(value.key < tmp->value.key){
        tmp->bst.left = bst_create_node(value);
    }else{
        tmp->bst.right = bst_create_node(value);
    }
    return 0;
}

VALUE_TYPE bst_find(PBST_Tree tree, KEY_TYPE key)
{
    VALUE_TYPE v = 0;
    if(tree == NULL || tree->root == NULL){
        return v;
    }
    PBST_Node node = tree->root;
    while(node != NULL){
        if(key == node->value.key){
            return node->value.value;
        }
        if(key < node->value.key){
            node = node->bst.left;
        }else{
            node = node->bst.right;
        }
    }
    return v;
}

void bst_travel(PBST_Node node)
{
    if(node == NULL){
        return;
    }
    bst_travel(node->bst.left);
    printf("%4d", node->value.value);
    bst_travel(node->bst.right);
}


int bst_delete(PBST_Tree tree, KEY_TYPE key)
{
    PBST_Node node = tree->root;
    PBST_Node tmp = tree->root;
    while(node != NULL){
        if(key == node->value.key){
            break;
        }
        tmp = node;
        if(key < node->value.key){
            node = node->bst.left;
        }else{
            node = node->bst.right;
        }
    }
    if(node == NULL){
        return 0;
    }
    bst_delete_node(tree, tmp, node);
    return 0;
}

int bst_delete_node(PBST_Tree tree, PBST_Node parent, PBST_Node currentnode)
{
    PBST_Node lnode = currentnode->bst.left;
    PBST_Node tmp = lnode;
    if(currentnode->bst.right == NULL && currentnode->bst.left == NULL){
        if(currentnode == tree->root){
            tree->root = NULL;
        }else{
            if(parent->bst.left == currentnode){
                parent->bst.left = NULL;
            }else{
                parent->bst.right = NULL;
            }
        }
        free(currentnode);
        return 0;
    }
    if(currentnode->bst.right == NULL){
        if(parent->bst.left == currentnode){
            parent->bst.left = currentnode->bst.left;
        }else{
            parent->bst.right = currentnode->bst.left;
        }
        free(currentnode);
        return 0;
    }
    if(currentnode->bst.left == NULL){
        if(parent->bst.left == currentnode){
            parent->bst.left = currentnode->bst.right;
        }else{
            parent->bst.right = currentnode->bst.right;
        }
        free(currentnode);
        return 0;
    }
    PBST_Node lright = lnode;
    tmp = lnode;
    while(lright->bst.right != NULL){
        tmp = lright;
        lright = lright->bst.right;
    }
    currentnode->value = lright->value;
    if(lright == tmp){
        parent->bst.left = lnode->bst.left;
    }else{
        tmp->bst.right = lright->bst.left;
    }
    free(lright);
    return 0;
}

#define __MAIN__
#ifdef __MAIN__
int main()
{
    int a[] = { 62, 88, 58, 47, 35, 73, 51, 99, 37, 93, 36, 39, 42, 62 };
    BST_Tree bstree = {0};
    for(int i=0; i<(sizeof(a)/sizeof(int)); i++){
        BST_VALUE bstvalue = {a[i],a[i]};
        bst_insert(&bstree, bstvalue);
    }
    bst_travel(bstree.root);
    VALUE_TYPE v = bst_find(&bstree, 88);
    printf("\nfind key 88's value:%d\n", v);
    printf("\n------------------------\n");
    printf("delete 47\n");
    bst_delete(&bstree, 47);
    bst_travel(bstree.root);
    printf("\n------------------------\n");
    printf("delete 62\n");
    bst_delete(&bstree, 62);
    bst_travel(bstree.root);
    return 0;
}

#endif