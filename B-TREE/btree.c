//
// Created by Administrator on 2019/12/20.
//
#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
btree_node* btree_create_node(int t, int leaf){
    btree_node* bnode = (btree_node*)calloc(1, sizeof(btree_node));
    if(bnode == NULL){
        printf("bode == NULL\n");
        return bnode;
    }
    bnode->keys = (NODE_VALUE*)calloc(1, (2*t-1) * sizeof(NODE_VALUE));
    bnode->childrens = (btree_node**)calloc(1, (2*t) * sizeof(btree_node*));
    bnode->keynum = 0;
    bnode->leaf = leaf;
    return bnode;
}

void btree_create(btree* T, int t){
    if(T == NULL){
        return;
    }
    T->t = t;
    T->root = btree_create_node(t, 1);
}

int btree_split_child(btree* T, btree_node* x, int i)
{
    btree_node* y = x->childrens[i];
    btree_node* z = btree_create_node(T->t, y->leaf);
    z->keynum = T->t - 1;
    for(int j=0; j<T->t - 1; j++){
        z->keys[j] = y->keys[j+T->t];
    }
    if(y->leaf == 0){
        for(int j=0; j<T->t; j++){
            z->childrens[j] = y->childrens[j+T->t];
        }
    }
    y->keynum = T->t - 1;
    for(int j=x->keynum; j>=i+1; j--){
        x->childrens[j+1] = x->childrens[j];
    }
    x->childrens[i+1] = z;
    for(int j=x->keynum-1; j>=i; j--){
        x->keys[j+1] = x->keys[j];
    }
    x->keys[i] = y->keys[T->t - 1];
    x->keynum += 1;
    return 0;
}

int btree_insert_nonfull(btree* T, btree_node* x, NODE_VALUE value)
{
    int i = x->keynum - 1;
    printf("x->leaf:%d\n", x->leaf);
    if(x->leaf == 1){
        while(i >= 0 && x->keys[i].key > value.key){
            x->keys[i+1] = x->keys[i];
            i--;
        }
        x->keys[i+1].key = value.key;
        x->keys[i+1].value = value.value;
        x->keynum += 1;
    }else{
        while(i>=0 && x->keys[i].key > value.key){
            i -- ;
        }
        if(x->childrens[i+1]->keynum >= (2*(T->t)) - 1){
            btree_split_child(T, x, i+1);
            if(value.key > x->keys[i+1].key) i++;
        }
        btree_insert_nonfull(T, x->childrens[i+1], value);
    }
    return 0;
}

int btree_insert(btree* T, NODE_VALUE value)
{
    printf("T:%x, T->num:%d, T->root:%x\n", &T, T->t, T->root);
    btree_node* r = T->root;
    printf("r->keynum:%d\n", r->keynum);
    if(r->keynum == 2 * T->t -1){
        btree_node* node = btree_create_node(T->t, 0);
        T->root = node;
        node->childrens[0] = r;
        btree_split_child(T, node, 0);
        int i = 0;
        if(node->keys[0].key < value.key){
            i++;
        }
        btree_insert_nonfull(T, node->childrens[i], value);
    }else{
        btree_insert_nonfull(T, r, value);
    }
    return 0;
}

void btree_destroy_node(btree_node *node) {
    free(node->childrens);
    free(node->keys);
    free(node);

}


int btree_merge(btree* T, btree_node* node, int idx)
{
    btree_node *left = node->childrens[idx];
    btree_node *right = node->childrens[idx+1];

    int i = 0;

    /////data merge
    left->keys[T->t-1] = node->keys[idx];
    for (i = 0;i < T->t-1;i ++) {
        left->keys[T->t+i] = right->keys[i];
    }
    if (!left->leaf) {
        for (i = 0;i < T->t;i ++) {
            left->childrens[T->t+i] = right->childrens[i];
        }
    }
    left->keynum += T->t;

    //destroy right
    btree_destroy_node(right);

    //node
    for (i = idx+1;i < node->keynum;i ++) {
        node->keys[i-1] = node->keys[i];
        node->childrens[i] = node->childrens[i+1];
    }
    node->childrens[i+1] = NULL;
    node->keynum -= 1;

    if (node->keynum == 0) {
        T->root = left;
        btree_destroy_node(node);
    }
    return 0;
}



int btree_delete_key(btree* T, btree_node* node, KEY_TYPE k)
{
    if(node == NULL){
        return 0;
    }
    int idx=0, i;
    while(idx<node->keynum && k < node->keys[idx].key){
        idx++;
    }
    if(idx < node->keynum && k == node->keys[idx].key){
        if(node->leaf){
            for(i=idx; i<node->keynum-1; i++){
                node->keys[i] = node->keys[i+1];
            }
            node->keys[node->keynum - 1].value  = 0;
            node->keys[node->keynum - 1].key = 0;
            node->keynum -- ;
            if(node->keynum == 0){
                free(node);
                T->root = NULL;
            }
            return 0;
        }else if(node->childrens[idx]->keynum >= T->t){
            btree_node* left = node->childrens[idx];
            node->keys[idx] = left->keys[left->keynum - 1];
            btree_delete_key(T, left, left->keys[left->keynum - 1].key);
        }else if(node->childrens[idx+1]->keynum >= T->t){
            btree_node* right = node->childrens[idx + 1];
            node->keys[idx] = right->keys[0];
            btree_delete_key(T, right, right->keys[0].key);
        }else{
            btree_merge(T, node, idx);
            btree_delete_key(T, node->childrens[idx], k);
        }

    }else{
        btree_node *child = node->childrens[idx];
        if (child == NULL) {
            printf("Cannot del key = %d\n", k);
            return 01;
        }

        if (child->keynum == T->t - 1) {

            btree_node *left = NULL;
            btree_node *right = NULL;
            if (idx - 1 >= 0)
                left = node->childrens[idx-1];
            if (idx + 1 <= node->keynum)
                right = node->childrens[idx+1];

            if ((left && left->keynum >= T->t) ||
                (right && right->keynum >= T->t)) {

                int richR = 0;
                if (right) richR = 1;
                if (left && right) richR = (right->keynum > left->keynum) ? 1 : 0;

                if (right && right->keynum >= T->t && richR) { //borrow from next
                    child->keys[child->keynum] = node->keys[idx];
                    child->childrens[child->keynum+1] = right->childrens[0];
                    child->keynum ++;

                    node->keys[idx] = right->keys[0];
                    for (i = 0;i < right->keynum - 1;i ++) {
                        right->keys[i] = right->keys[i+1];
                        right->childrens[i] = right->childrens[i+1];
                    }

                    right->keys[right->keynum-1].key = 0;
                    right->keys[right->keynum-1].value = 0;
                    right->childrens[right->keynum-1] = right->childrens[right->keynum];
                    right->childrens[right->keynum] = NULL;
                    right->keynum --;

                } else { //borrow from prev

                    for (i = child->keynum;i > 0;i --) {
                        child->keys[i] = child->keys[i-1];
                        child->childrens[i+1] = child->childrens[i];
                    }

                    child->childrens[1] = child->childrens[0];
                    child->childrens[0] = left->childrens[left->keynum];
                    child->keys[0] = node->keys[idx-1];
                    child->keynum ++;

                    left->keys[left->keynum-1].key = 0;
                    left->keys[left->keynum-1].value = 0;
                    left->childrens[left->keynum] = NULL;
                    left->keynum --;
                }

            } else if ((!left || (left->keynum == T->t - 1))
                       && (!right || (right->keynum == T->t - 1))) {

                if (left && left->keynum == T->t - 1) {
                    btree_merge(T, node, idx-1);
                    child = left;
                } else if (right && right->keynum == T->t - 1) {
                    btree_merge(T, node, idx);
                }
            }
        }

        btree_delete_key(T, child, k);
    }
    return 0;
}

int btree_delete(btree* T, KEY_TYPE k)
{
    if(T == NULL){
        return -1;
    }
    btree_delete_key(T, T->root, k);
    return 0;
}


void btree_traverse(btree_node *x) {
    int i = 0;

    for (i = 0;i < x->keynum;i ++) {
        if (x->leaf == 0)
            btree_traverse(x->childrens[i]);
        printf("%C ", x->keys[i].value);
    }

    if (x->leaf == 0) btree_traverse(x->childrens[i]);
}

void btree_print(btree *T, btree_node *node, int layer)
{
    btree_node* p = node;
    int i;
    if(p){
        printf("\nlayer = %d keynum = %d is_leaf = %d\n", layer, p->keynum, p->leaf);
        for(i = 0; i < node->keynum; i++)
            printf("%c ", p->keys[i].value);
        printf("\n");
#if 0
        printf("%p\n", p);
		for(i = 0; i <= 2 * T->t; i++)
			printf("%p ", p->childrens[i]);
		printf("\n");
#endif
        layer++;
        for(i = 0; i <= p->keynum; i++)
            if(p->childrens[i])
                btree_print(T, p->childrens[i], layer);
    }
    else printf("the tree is empty\n");
}


int btree_bin_search(btree_node *node, int low, int high, KEY_TYPE key) {
    int mid;
    if (low > high || low < 0 || high < 0) {
        return -1;
    }

    while (low <= high) {
        mid = (low + high) / 2;
        if (key > node->keys[mid].key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return low;
}


int main() {
    btree T = {0};

    btree_create(&T, 3);

    int i = 0;
    char key[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (i = 0;i < 26;i ++) {
        //key[i] = rand() % 1000;
        printf("%c ", key[i]);
        NODE_VALUE value;
        value.key = key[i];
        value.value = key[i];
        btree_insert(&T, value);
    }
    printf("insert success.\n");
    btree_print(&T, T.root, 0);

    for (i = 0;i < 26;i ++) {
        printf("\n---------------------------------\n");
        btree_delete(&T, key[25-i]);
        //btree_traverse(T.root);
        btree_print(&T, T.root, 0);
    }

}
