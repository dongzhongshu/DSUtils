//
// Created by Administrator on 2019/12/16.
//
#include "rbt.h"
#include <stdio.h>
#include <stdlib.h>
void rbtree_route_left(rbtree* T, rbtree_node* x)
{
    rbtree_node* y = x->rbelement.right;

    if(y->rbelement.left != T->nil){
        y->rbelement.left->rbelement.parent = x;
    }
    x->rbelement.right = y->rbelement.left;
    y->rbelement.parent = x->rbelement.parent;
    if(x->rbelement.parent == T->nil){
        T->root = y;
    }else if(x->rbelement.parent->rbelement.left == x){
        x->rbelement.parent->rbelement.left = y;
    }else{
        x->rbelement.parent->rbelement.right = y;
    }
    x->rbelement.parent = y;
    y->rbelement.left = x;

}


void rbtree_route_right(rbtree* T, rbtree_node* y)
{
    rbtree_node* x = y->rbelement.left;
    if(x->rbelement.right != T->nil){
        x->rbelement.right->rbelement.parent = y;
    }
    y->rbelement.left = x->rbelement.right;
    if(y->rbelement.parent == T->nil){
        T->root = x;
    }else if(y->rbelement.parent->rbelement.left == y){
        y->rbelement.parent->rbelement.left = x;
    }else{
        y->rbelement.parent->rbelement.right = x;
    }
    x->rbelement.parent = y->rbelement.parent;
    x->rbelement.right = y;
    y->rbelement.parent = x;
}

void rbtree_insert_fix(rbtree* T, rbtree_node* node)
{
    while(node->rbelement.parent->color == RED){
        if(node->rbelement.parent == node->rbelement.parent->rbelement.parent->rbelement.left){//left tree
            rbtree_node * unclenode = node->rbelement.parent->rbelement.parent->rbelement.right;
            if(unclenode->color == RED){
                node->rbelement.parent->color = BLACK;
                unclenode->color = BLACK;
                node->rbelement.parent->rbelement.parent->color = RED;
                node = node->rbelement.parent->rbelement.parent;
            }else{
                rbtree_node *znode = node;
                if(znode == znode->rbelement.parent->rbelement.right){
                    znode = znode->rbelement.parent;
                    rbtree_route_left(T, znode);
                }
                znode->rbelement.parent->color = BLACK;
                znode->rbelement.parent->rbelement.parent->color = RED;
                rbtree_route_right(T, znode->rbelement.parent->rbelement.parent);
            }
        }else{
            rbtree_node * unclenode = node->rbelement.parent->rbelement.parent->rbelement.left;
            if(unclenode->color == RED){
                node->rbelement.parent->color = BLACK;
                unclenode->color = BLACK;
                node->rbelement.parent->rbelement.parent->color = RED;
                node = node->rbelement.parent->rbelement.parent;
            }else{
                rbtree_node *znode = node;
                if(znode == znode->rbelement.parent->rbelement.left){
                    znode = znode->rbelement.parent;
                    rbtree_route_right(T, znode);
                }
                znode->rbelement.parent->color = BLACK;
                znode->rbelement.parent->rbelement.parent->color = RED;
                rbtree_route_left(T, znode->rbelement.parent->rbelement.parent);
            }
        }
    }
    T->root->color = BLACK;
}


int rbtree_insert(rbtree* T, RBT_VALUE value)
{
    rbtree_node* parent = T->nil;
    rbtree_node* find = T->root;
//    printf("hello\n");
    while(find != T->nil){
        parent = find;
        if(value.key == find->value.key){
            return 1;
        }
        if(value.key < find->value.key){
            find = find->rbelement.left;
        }else{
            find = find->rbelement.right;
        }
    }
//    printf("hello...\n");
    rbtree_node* pvalue = (rbtree_node*)calloc(1, sizeof(rbtree_node));
    if(pvalue == NULL){
        return 2;
    }
    pvalue->value = value;
    pvalue->rbelement.right = T->nil;
    pvalue->rbelement.left = T->nil;
    pvalue->rbelement.parent = parent;
    if(parent == T->nil){
        T->root = pvalue;
    }else if(value.key < parent->value.key){
        parent->rbelement.left = pvalue;
    }else{
        parent->rbelement.right = pvalue;
    }
    pvalue->color = RED;
//    printf("hello;;;\n");
    rbtree_insert_fix(T, pvalue);
    return 0;
}

rbtree_node* rbtree_max(rbtree* T, rbtree_node* x){
    while(x->rbelement.right != T->nil){
        x = x->rbelement.right;
    }
    return x;
}
rbtree_node* rbtree_min(rbtree* T, rbtree_node* y)
{
    while(y->rbelement.left != T->nil){
        y = y->rbelement.left;
    }
    return y;
}

rbtree_node *rbtree_successor(rbtree *T, rbtree_node *x) {
    rbtree_node *y = x->rbelement.parent;

    if (x->rbelement.right != T->nil) {
        return rbtree_min(T, x->rbelement.right);
    }

    while ((y != T->nil) && (x == y->rbelement.right)) {
        x = y;
        y = y->rbelement.parent;
    }
    return y;
}


void rbtree_delete_fixup(rbtree *T, rbtree_node *x) {

    while ((x != T->root) && (x->color == BLACK)) {
        if (x == x->rbelement.parent->rbelement.left) {

            rbtree_node *w= x->rbelement.parent->rbelement.right;
            if (w->color == RED) {
                w->color = BLACK;
                x->rbelement.parent->color = RED;

                rbtree_route_left(T, x->rbelement.parent);
                w = x->rbelement.parent->rbelement.right;
            }

            if ((w->rbelement.left->color == BLACK) && (w->rbelement.right->color == BLACK)) {
                w->color = RED;
                x = x->rbelement.parent;
            } else {

                if (w->rbelement.right->color == BLACK) {
                    w->rbelement.left->color = BLACK;
                    w->color = RED;
                    rbtree_route_right(T, w);
                    w = x->rbelement.parent->rbelement.right;
                }

                w->color = x->rbelement.parent->color;
                x->rbelement.parent->color = BLACK;
                w->rbelement.right->color = BLACK;
                rbtree_route_left(T, x->rbelement.parent);

                x = T->root;
            }

        } else {

            rbtree_node *w = x->rbelement.parent->rbelement.left;
            if (w->color == RED) {
                w->color = BLACK;
                x->rbelement.parent->color = RED;
                rbtree_route_right(T, x->rbelement.parent);
                w = x->rbelement.parent->rbelement.left;
            }

            if ((w->rbelement.left->color == BLACK) && (w->rbelement.right->color == BLACK)) {
                w->color = RED;
                x = x->rbelement.parent;
            } else {

                if (w->rbelement.left->color == BLACK) {
                    w->rbelement.right->color = BLACK;
                    w->color = RED;
                    rbtree_route_left(T, w);
                    w = x->rbelement.parent->rbelement.left;
                }

                w->color = x->rbelement.parent->color;
                x->rbelement.parent->color = BLACK;
                w->rbelement.left->color = BLACK;
                rbtree_route_right(T, x->rbelement.parent);

                x = T->root;
            }

        }
    }

    x->color = BLACK;
}


rbtree_node *rbtree_delete(rbtree *T, rbtree_node *z) {

    rbtree_node *y = T->nil;
    rbtree_node *x = T->nil;

    if ((z->rbelement.left == T->nil) || (z->rbelement.right == T->nil)) {
        y = z;
    } else {
        y = rbtree_successor(T, z);
    }

    if (y->rbelement.left != T->nil) {
        x = y->rbelement.left;
    } else if (y->rbelement.right != T->nil) {
        x = y->rbelement.right;
    }

    x->rbelement.parent = y->rbelement.parent;
    if (y->rbelement.parent == T->nil) {
        T->root = x;
    } else if (y == y->rbelement.parent->rbelement.left) {
        y->rbelement.parent->rbelement.left = x;
    } else {
        y->rbelement.parent->rbelement.right = x;
    }

    if (y != z) {
        z->value = y->value;
        /*z->value = y->value;*/
    }

    if (y->color == BLACK) {
        rbtree_delete_fixup(T, x);
    }

    return y;
}

rbtree_node *rbtree_search(rbtree *T, KEY_TYPE key) {

    rbtree_node *node = T->root;
    while (node != T->nil) {
        if (key < node->value.key) {
            node = node->rbelement.left;
        } else if (key > node->value.key) {
            node = node->rbelement.right;
        } else {
            return node;
        }
    }
    return T->nil;
}


void rbtree_traversal(rbtree *T, rbtree_node *node) {
    if (node != T->nil) {
        rbtree_traversal(T, node->rbelement.left);
        printf("key:%d, color:%d\n", node->value.key, node->color);
        rbtree_traversal(T, node->rbelement.right);
    }
}


int main() {

    int keyArray[20] = {24,25,13,35,23, 26,67,47,38,98, 20,19,17,49,12, 21,9,18,14,15};

    rbtree *T = (rbtree *)malloc(sizeof(rbtree));
    if (T == NULL) {
        printf("malloc failed\n");
        return -1;
    }

    T->nil = (rbtree_node*)malloc(sizeof(rbtree_node));
    T->nil->color = BLACK;
    T->root = T->nil;

    rbtree_node *node = T->nil;
    int i = 0;
    for (i = 0;i < 20;i ++) {
//        node = (rbtree_node*)malloc(sizeof(rbtree_node));
//        node->value.key = keyArray[i];
//        node->value = keyArray[i];
        RBT_VALUE value;
        value.key = keyArray[i];
        value.value = keyArray[i];
        rbtree_insert(T, value);

    }

    rbtree_traversal(T, T->root);
    printf("----------------------------------------\n");

    for (i = 0;i < 20;i ++) {

        rbtree_node *node = rbtree_search(T, keyArray[i]);
        rbtree_node *cur = rbtree_delete(T, node);
        free(cur);

        rbtree_traversal(T, T->root);
        printf("----------------------------------------\n");
    }



}
