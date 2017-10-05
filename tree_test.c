#include "tree.h"
#include <CUnit/CUnit.h>
#include <stdio.h>
#include "CUnit/Basic.h"
#include <stdlib.h>


void test_tree_new()
{
  CU_ASSERT_TRUE(tree_new(NULL, NULL, NULL, NULL) != NULL);
}
int tree_compare_fun(elem_t elem1 , elem_t elem2)
{

  if (elem1.i > elem2.i)
    {
      return 1;
    }
  if (elem1.i < elem2.i)
    {
      return -1;
    }
  else
    {
      return 0;
    }
}

void test_tree_depth()
{
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  elem_t key; key.i = 3;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);
  
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
  elem_t key2; key2.i = 1;
  elem_t elem2; elem2.i = 5552;
  tree_insert(tree , key2 , elem2);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 2);
  CU_ASSERT_TRUE(tree->root->right->key.i == 3);  
  
  tree_t *tree2  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  key.i = 3;
  elem.i = 555;
  tree_insert(tree2, key , elem);
  
  key1.i = 1;
  elem1.i = 555;
  tree_insert(tree2, key1 , elem1);
  
  key2.i = 2;
  elem2.i = 5552;
  tree_insert(tree2, key2 , elem2);

  
  CU_ASSERT_TRUE(tree_depth(tree2) == 2);
  
  tree_t *tree3  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  key.i = 1;
  elem.i = 555;
  tree_insert(tree3, key , elem);
  
  key1.i = 2;
  elem1.i = 555;
  tree_insert(tree3, key1 , elem1);
  
  key2.i = 3;
  elem2.i = 5552;
  tree_insert(tree3, key2 , elem2);

  
  CU_ASSERT_TRUE(tree_depth(tree3) == 2);
  
  tree_t *tree4  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  key.i = 1;
  elem.i = 555;
  tree_insert(tree4, key , elem);
  
  key1.i = 3;
  elem1.i = 555;
  tree_insert(tree4, key1 , elem1);
  
  key2.i = 2;
  elem2.i = 5552;
  tree_insert(tree4, key2 , elem2);

  
  CU_ASSERT_TRUE(tree_depth(tree4) == 2);
}

int main(int argc, char *argv[])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite tree = CU_add_suite("Test new tree", NULL, NULL);
  CU_add_test(tree, "Tree", test_tree_new);

  CU_pSuite tree_depth = CU_add_suite("Test tree depth", NULL, NULL);
  CU_add_test(tree_depth, "Tree_depth", test_tree_depth);

  //Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return 0;
}
