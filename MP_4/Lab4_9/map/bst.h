#ifndef _BST_H_
#define _BST_H_

#include "utility.h"
#include "department.h"

typedef struct bst_node
{
    char* key;
    struct bst_node* left;
    struct bst_node* right;
    Department* dep;
} bst_node;

typedef struct BST
{
    bst_node* root;
} BST;


status_code bst_construct(BST* tree);
status_code bst_destruct(BST* tree);
status_code bst_set_null(BST* tree);

status_code bst_insert(BST* tree, const char* key, Department* dep);
status_code bst_search_node(BST* tree, const char* key, bst_node** node);
status_code bst_contains(BST* tree, const char* key, int* is_contained);
status_code bst_get(BST* tree, const char* key, Department** dep);
status_code bst_erase(BST* tree, const char* key);
void print_tree(bst_node *node, int tab);

#endif // _BST_H_
