#include <stdlib.h>
#include "tree.h"

typedef struct node node_t;

struct tree{
  node_t *root;
  element_copy_fun element_copy;
  key_free_fun key_free;
  element_free_fun elem_free;
  element_comp_fun compare;
};

struct node{
  node_t *left;
  node_t *right;
  tree_key_t key;
  elem_t elem;
};

tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free, element_free_fun elem_free, element_comp_fun compare)
{
  tree_t * new_tree = calloc(1, sizeof(tree_t));
  new_tree->element_copy = element_copy;
  new_tree->key_free = key_free;
  new_tree->elem_free = elem_free;
  new_tree->compare = compare;

  return new_tree;
}

void node_delete(tree_t *tree, node_t *node, bool delete_keys, bool delete_elements)
{
  if (node == NULL)
    {
      return;
    }
      
  if (node->left != NULL){
    node_delete(tree, node->left, delete_keys, delete_elements);
  }
  if (delete_keys == true)
    {
      tree->key_free(node->key);
    }
  if (delete_elements == true)
    {
      tree->elem_free(node->elem);
    }
  if (node->right != NULL){
    node_delete(tree, node->right, delete_keys, delete_elements);
  }
  if (delete_keys == true && delete_elements == true)
    {
      free(node);
    }
}

void tree_delete(tree_t *tree, bool delete_keys, bool delete_elements)
{
  if (delete_keys == true && delete_elements == false)
    {
      return;
    }
  
  node_delete(tree, tree->root, delete_keys, delete_elements);
}

int node_size(node_t *node){
  if (node == NULL){
    return 0;
  }
  int size = 0;
  if(node->left == NULL && node->right == NULL){
    return size;
  }
  if(node->left != NULL){
    size += node_size(node->left) + 1;
  }
  if (node->right != NULL){
    size += node_size(node->right) + 1;
  }
  return size;
}
int tree_size(tree_t *tree)
{
  if(!tree){
    return 0;
  }
  if(tree->root ==  NULL){
    return 0;
  }
  return 1 + node_size(tree->root);

}

int node_depth(node_t *node)
{
  int left_depth = 0;
  int right_depth = 0;
  if(node->left != NULL)
    {
    left_depth = node_depth(node->left) + 1;
    }
  if(node->right != NULL)
    {
    right_depth = node_depth(node->right) + 1;
    }
  if(left_depth >= right_depth)
    {
    return left_depth;
    }
  else
    {
    return right_depth;
    }
}
int tree_depth(tree_t *tree)
{
  if(!tree)
    {
    return 0;
    }
  return 1 + node_depth(tree->root);
}

node_t **place_to_insert_node(node_t *node, tree_key_t key)
{
  node_t **pekare = &node;
  if (key.i == (*pekare)->key.i)
    {
      return NULL;
    }
  if ((*pekare) == NULL)
  {
    return pekare;
  }
    
  if (key.i < (*pekare)->key.i)
    {
      pekare = &(*pekare)->left;
      place_to_insert_node(*pekare, key);
    }
  if (key.i > (*pekare)->key.i)
    {
      pekare = &(*pekare)->right;
      place_to_insert_node(*pekare, key);
    }

  return pekare;
}

node_t **place_to_insert_tree(tree_t *tree, tree_key_t key)
{
  return place_to_insert_node(tree->root, key);
}

node_t *left_left (node_t *node)
{
  node->left->right = node;
  node = node->left;
  node->right->left = NULL;
  return node;
}

node_t *right_right(node_t *node)
{
   node->left->left = node;
   node = node->right;
   node->left->right = NULL;
   return node;
}


node_t *balance_node(node_t *node)
{
  if (node->right == NULL)
    {
      if (node->left->right == NULL)
        {
          //left left
          return left_left(node);
        }
      else if (node->left->left == NULL)
        {
          //left right
          node->left->right->left = node->left;
          node->left = node->left->right;
          node->left->right = NULL;
          return left_left(node);
          
        }
    }
  if ( node->left == NULL)
    {
      if (node->right->left == NULL)
        {
          //right right
          return right_right(node);
        }
      else if (node->right->right == NULL)
        {
          //right left
          node->right->left->right = node->right;
          node->right = node->right->left;
          node->right->left = NULL;
          return right_right(node);
        }
    }
  if( abs(node_depth(node->right) - node_depth(node->left)) > 1)
    {
      
    }
  
}

bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  element_copy_fun copy_fun = tree->element_copy;
  if( copy_fun != NULL)
    {
      elem = copy_fun(elem);
    }
  node_t **insert_place = place_to_insert_tree(tree, key);
  if(insert_place == NULL)
    {
      return false;
    }
  else
    {
      node_t *new_node = calloc(1, sizeof(node_t));
      new_node->elem = elem;
      new_node->key = key;
      *insert_place = new_node;
      return true;
    }
  
}

bool tree_has_key(tree_t *tree, tree_key_t key)
{
  if (place_to_insert_tree(tree, key) == NULL)
    {
      return false;
    }
  else
    {
      return true;
    }   
}

node_t *node_get(node_t * node, tree_key_t key)
{
  if (node == NULL)
    {
      return NULL;
    }
  if ( node->key.i == key.i)
    {
      return node;
    }
  if ( key.i < node->key.i)
    {
      return node_get(node->left, key);
    }
  else
    {
      return node_get(node->right, key);
    }
  
  
}

bool tree_get(tree_t *tree, tree_key_t key, elem_t *result)
{
  node_t *node_result = node_get(tree->root , key);
  if (node_result == NULL)
    {
      return false;
    }
  else
    {
      *result = node_result->elem;
      return true;
    }
}

