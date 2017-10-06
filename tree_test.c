#include "tree.h"
#include <CUnit/CUnit.h>
#include <stdio.h>
#include "CUnit/Basic.h"
#include <stdlib.h>

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

void test_tree_new()
{
  CU_ASSERT_TRUE(tree_new(NULL, NULL, NULL, NULL) != NULL);
}



void test_tree_delete()
{ /* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  */
  
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

  /* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  */
}

void test_tree_size()
{
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);

  CU_ASSERT_TRUE(tree_size(tree) == 0);
  
  elem_t key; key.i = 3;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  CU_ASSERT_TRUE(tree_size(tree) == 1);
    
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
  elem_t key2; key2.i = 1;
  elem_t elem2; elem2.i = 5552;
  tree_insert(tree , key2 , elem2);

  CU_ASSERT_TRUE(tree_size(tree) == 3);
}

void test_tree_depth()
{
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);

  CU_ASSERT_TRUE(tree_depth(tree) == 0);
  
  elem_t key; key.i = 3;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  CU_ASSERT_TRUE(tree_depth(tree) == 1);
  
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
  elem_t key2; key2.i = 1;
  elem_t elem2; elem2.i = 555;
  tree_insert(tree , key2 , elem2);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 2);

  elem_t key3; key3.i = 4;
  elem_t elem3; elem3.i = 555;
  tree_insert(tree, key3, elem3);

  CU_ASSERT_TRUE(tree_depth(tree) == 3);

}

void test_place_to_insert_tree()
{
  //CU_ASSERT_PTR_NULL();
  
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  /* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  */
}

void find_unbalanaced_to_fix()
{
  /* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  */
}

void test_balance_node()
{
  /* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  *//* TODO:  */
}

void test_tree_insert()
{
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);

  elem_t key; key.i = 3;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  CU_ASSERT_TRUE(tree_size(tree) == 1);

    
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
   CU_ASSERT_TRUE(tree_size(tree) == 2);

  elem_t key2; key2.i = 2;
  elem_t elem2; elem2.i = 555;
  tree_insert(tree , key2 , elem2);

  CU_ASSERT_TRUE(tree_size(tree) == 2); 
}

void test_tree_has_key()
{
   tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);

   CU_ASSERT_TRUE(tree_has_key(tree, 
   
  elem_t key; key.i = 3;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

 

    
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
   CU_ASSERT_TRUE(tree_size(tree) == 2);

  elem_t key2; key2.i = 2;
  elem_t elem2; elem2.i = 555;
  tree_insert(tree , key2 , elem2);

  CU_ASSERT_TRUE(tree_size(tree) == 2); 
} 
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
