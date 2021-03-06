//
// Created by Administrator on 2019/12/20.
//

#ifndef DSUTILS_BPLUSTREE_H
#define DSUTILS_BPLUSTREE_H
#include <stdbool.h>
//#ifdef _WIN32
//#define bool char
//#define false 0
//#define true 1
//#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int KEY_TYPE;
typedef int VALUE_TYPE;
typedef struct record {
    int value;
} record;

/* Type representing a node in the B+ tree.
 * This type is general enough to serve for both
 * the leaf and the internal node.
 * The heart of the node is the array
 * of keys and the array of corresponding
 * pointers.  The relation between keys
 * and pointers differs between leaves and
 * internal nodes.  In a leaf, the index
 * of each key equals the index of its corresponding
 * pointer, with a maximum of order - 1 key-pointer
 * pairs.  The last pointer points to the
 * leaf to the right (or NULL in the case
 * of the rightmost leaf).
 * In an internal node, the first pointer
 * refers to lower nodes with keys less than
 * the smallest key in the keys array.  Then,
 * with indices i starting at 0, the pointer
 * at i + 1 points to the subtree with keys
 * greater than or equal to the key in this
 * node at index i.
 * The num_keys field is used to keep
 * track of the number of valid keys.
 * In an internal node, the number of valid
 * pointers is always num_keys + 1.
 * In a leaf, the number of valid pointers
 * to data is always num_keys.  The
 * last leaf pointer points to the next leaf.
 */
typedef struct node {
    void ** pointers;
    int * keys;
    struct node * parent;
    bool is_leaf;
    int num_keys;
    struct node * next; // Used for queue.
} node;


void enqueue(node * new_node);
node * dequeue(void);
int height(node * const root);
int path_to_root(node * const root, node * child);
void print_leaves(node * const root);
void print_tree(node * const root);
void find_and_print(node * const root, int key, bool verbose);
void find_and_print_range(node * const root, int range1, int range2, bool verbose);
int find_range(node * const root, int key_start, int key_end, bool verbose,
               int returned_keys[], void * returned_pointers[]);
node * find_leaf(node * const root, int key, bool verbose);
record * find(node * root, int key, bool verbose, node ** leaf_out);
int cut(int length);

// Insertion.

record * make_record(int value);
node * make_node(void);
node * make_leaf(void);
int get_left_index(node * parent, node * left);
node * insert_into_leaf(node * leaf, int key, record * pointer);
node * insert_into_leaf_after_splitting(node * root, node * leaf, int key,
                                        record * pointer);
node * insert_into_node(node * root, node * parent,
                        int left_index, int key, node * right);
node * insert_into_node_after_splitting(node * root, node * parent,
                                        int left_index,
                                        int key, node * right);
node * insert_into_parent(node * root, node * left, int key, node * right);
node * insert_into_new_root(node * left, int key, node * right);
node * start_new_tree(int key, record * pointer);
node * insert(node * root, int key, int value);

// Deletion.

int get_neighbor_index(node * n);
node * adjust_root(node * root);
node * coalesce_nodes(node * root, node * n, node * neighbor,
                      int neighbor_index, int k_prime);
node * redistribute_nodes(node * root, node * n, node * neighbor,
                          int neighbor_index,
                          int k_prime_index, int k_prime);
node * delete_entry(node * root, node * n, int key, void * pointer);
node * delete(node * root, int key);
#endif //DSUTILS_BTREE_H
