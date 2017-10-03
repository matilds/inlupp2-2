#include <stdlib.h>
#include "tree.h"
#include <stdio.h> 

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

tree_t *tree_new(element_copy_fun element_copy, key_free_fun key_free,
                 element_free_fun elem_free, element_comp_fun compare)
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
  if(node == NULL)
    {
       if(left_depth >= right_depth)
         {
           return left_depth;
             
         }
       else
         {
           return right_depth;
         }       
    }
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

  if(key.i > node->key.i)
    {
      if (node->right == NULL)
        {
          return &(node->right);
        }
      return place_to_insert_node(node->right , key);
     
    }

  if(key.i < node->key.i)
    {
      
      if(node->left == NULL)
        {
          return &(node->left);
        }
      return place_to_insert_node(node->left , key);
    }
  return NULL;
}

node_t **place_to_insert_tree(tree_t *tree, tree_key_t key)
{
  return place_to_insert_node(tree->root, key);
}

node_t **left_left (node_t **node)
{
  node_t *tmp = (*node)->left->right;
  (*node)->left->right = (*node);
  (*node) = (*node)->left;
  (*node)->right->left = tmp;
  return node;
}
node_t **left_right(node_t **node)
{
   (*node)->left->right->left = (*node)->left;
   (*node)->left = (*node)->left->right;
   (*node)->left->right = NULL;
   return left_left(node);
}

node_t **right_right(node_t **node)
{
  node_t *tmp = (*node)->right->left;
  (*node)->left->left = (*node);
  (*node) = (*node)->right;
  (*node)->left->right = tmp;
   return node;
}

node_t **right_left(node_t **node)
{
   (*node)->right->left->right = (*node)->right;
   (*node)->right = (*node)->right->left;
   (*node)->right->left = NULL;
   return right_right(node);
}

/*
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
*/

node_t **find_unbalanced_to_fix(node_t **node, node_t **unbalanced_part)
{
  int depth_right = node_depth((*node)->right);
  int depth_left = node_depth((*node)->left);
  
  if (node == NULL)
    {
      return unbalanced_part;
    }
  if(abs(depth_left - depth_right) > 1)
    {
      unbalanced_part = node;
      if(depth_left > depth_right)
        {
          find_unbalanced_to_fix(&(*node)->left, unbalanced_part);
        }
      else
        {
          find_unbalanced_to_fix(&(*node)->right, unbalanced_part);
        }
    }
  return unbalanced_part;
}

node_t **balanced_node(node_t **node)
{
  
  
  if (node_depth((*node)->right) > node_depth((*node)->left))
    {  
      if (node_depth((*node)->right->right) > node_depth((*node)->right->left))
        {
          return right_right(node);
        }
      else if (node_depth((*node)->right->left) > node_depth((*node)->right->right)
               && (*node)->left == NULL)
        {
          return right_left(node);
        }
      else if(node_depth((*node)->right->left->right) > node_depth((*node)->right->left->left))
        {
          left_right(&(*node)->right);
          return right_right(node);
        }
      else
        {
          left_left(&(*node)->right);
          return right_right(node);
        }
      
    }

  else //if (node_depth(node->left) > node_depth(node->right))
    {
      if (node_depth((*node)->left->left) > node_depth((*node)->left->right))
        {
          return left_left(node);
        }
      else if (node_depth((*node)->left->right) > node_depth((*node)->left->left)
               &&((*node)->right == NULL))
        {
          return left_right(node);
        }
      else if( node_depth((*node)->left->right->left) > node_depth((*node)->left->right->right))
        {
          right_left(&(*node)->left);
          return left_left(node);
        }
      else
        {
          right_right(&(*node)->left);
          return left_left(node);
        }
    }
}

bool tree_insert(tree_t *tree, tree_key_t key, elem_t elem)
{
  element_copy_fun copy_fun = tree->element_copy;
  if( copy_fun != NULL)
    {
      elem = copy_fun(elem);
    }
  //test
  if(tree->root == NULL)
    {
      node_t *new = calloc(1,sizeof(node_t));
      new->elem = elem;
      new->key = key;
      tree->root = new;
      return true;
    }
  //
  
  node_t **insert_place = place_to_insert_tree(tree, key);
  // if key exists in tree
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
      
      if(abs(node_depth(tree->root->right) - node_depth(tree->root->left)) <= 1)
        {
          return true;
        }
      else
        {
          node_t **unbalanced_part;
          do
        {
          
          unbalanced_part = NULL; //set to NULL every iteration.
          //unbalanced_part = find_unbalanced_to_fix(&tree->root, unbalanced_part);
          //balanced_node(unbalanced_part);
          left_left(find_unbalanced_to_fix(&tree->root, unbalanced_part));
      
        }
      while(unbalanced_part != NULL);
      
        }
    }
  return true;
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

int main()
{

  tree_t *tree  = tree_new(NULL,NULL,NULL,NULL);
  
  elem_t key;
  key.i = 3;
  elem_t elem;
  elem.i = 555;
  tree_insert(tree, key , elem);
  
  elem_t key1;
  key1.i = 2;
  elem_t elem1;
  elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
  elem_t key2;
  key2.i = 1;
  elem_t elem2;
  elem2.i = 5552;
  tree_insert(tree , key2 , elem2);

  int i = tree_depth(tree);
  int j = tree_size(tree);
  printf("depth = %d , size = %d\n" , i ,j);

 
  printf("root = %d\n" , tree->root->key.i);
  printf("root->left = %d\n" , tree->root->left->key.i);
  printf("root->right = %d\n" , tree->root->right->key.i);
}
