#include "tree.h"
#include <CUnit/CUnit.h>
#include <stdio.h>
#include "CUnit/Basic.h"
#include <stdlib.h>


void test_tree_new()
{
  CU_ASSERT_TRUE(tree_new(NULL, NULL, NULL, NULL) != NULL);
}

int main(int argc, char *argv[])
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite tree = CU_add_suite("Test new tree", NULL, NULL);
  CU_add_test(tree, "Tree", test_tree_new);

  //Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return 0;
}
