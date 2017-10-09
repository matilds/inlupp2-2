#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <CUnit/Automated.h>

void test_list_new()
{
  CU_ASSERT_TRUE(list_new(NULL, NULL, NULL) != NULL);
}

void test_list_insert_and_length()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t elem; elem.i = 1;
  elem_t elem2; elem2.i = 2;

  CU_ASSERT_TRUE(list_length(list) == 0);
  
  CU_ASSERT_TRUE(list_insert(list, 10, elem));
  CU_ASSERT_TRUE(list_length(list) == 1);

  CU_ASSERT_TRUE(list_insert(list, -1, elem2));
  CU_ASSERT_TRUE(list_length(list) == 2);
  
}

void test_list_append()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t elem; elem.i = 1;
  elem_t elem2; elem2.i = 2;
  elem_t result;
  
  list_append(list, elem);
  CU_ASSERT_TRUE(list_get(list, 0, &result) == true);
  CU_ASSERT_TRUE(list_get(list, -1, &result) == true);

  list_append(list, elem2);
  CU_ASSERT_TRUE(list_get(list, 1, &result) == true);
  CU_ASSERT_TRUE(result.i == elem2.i);
  CU_ASSERT_TRUE(list_get(list, 5, &result) == false);

}

void test_list_prepend()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t elem; elem.i = 1;
  elem_t elem2; elem2.i = 2;
  elem_t result;

  list_prepend(list, elem);
  CU_ASSERT_TRUE(list_get(list, 0, &result) == true);
  CU_ASSERT_TRUE(list_get(list, -1, &result) == true);

  list_prepend(list, elem2);
  CU_ASSERT_TRUE(list_get(list, 1, &result) == true);
  CU_ASSERT_TRUE(list_get(list, 5, &result) == false);
}

// Not defined in list.h
void test_list_get_link()
{}

void test_list_remove()
{
  list_t *list = list_new(NULL, NULL, NULL);

  /* TODO:  *//* TODO:  *//* TODO:  */ //need delete function
}

void test_list_get()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t result;
  CU_ASSERT_TRUE(list_get(list, 10, &result) == false);
  
  elem_t elem; elem.i = 1;
  list_insert(list, 0, elem);
  CU_ASSERT_TRUE(list_get(list, 0, &result) == true);
  list_insert(list, 1, elem);
  CU_ASSERT_TRUE(list_get(list, 1, &result) == true);
  CU_ASSERT_FALSE(result.i == 0);
}

void test_list_first()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t result;
  CU_ASSERT_TRUE(list_first(list, &result) == false);

  elem_t elem; elem.i = 2;
  elem_t elem2; elem2.i = 1;
  elem_t elem3; elem3.i = 3;
  list_insert(list, 0, elem);
  
  list_first(list, &result);
  CU_ASSERT_TRUE(result.i == 2);
  
  list_insert(list, 0, elem2);
  list_insert(list, -1, elem3);

  list_last(list, &result);
  CU_ASSERT_TRUE(result.i == 3);
  
}

void test_list_last()
{
  list_t *list = list_new(NULL, NULL, NULL);
  elem_t result;
  CU_ASSERT_TRUE(list_last(list, &result) == false);

  elem_t elem; elem.i = 2;
  elem_t elem2; elem2.i = 1;
  elem_t elem3; elem3.i = 3;
  list_insert(list, 0, elem);
  list_last(list, &result);
  CU_ASSERT_TRUE(result.i == 2);
  
  list_insert(list, 0, elem2);
  list_insert(list, -1, elem3);
  
  list_last(list, &result);
  CU_ASSERT_TRUE(result.i == 3);
}

void test_list_delete()
{
  list_t *list = list_new(NULL, NULL, NULL);

  /* TODO:  *//* TODO:  *//* TODO:  */ //need delete function
}

void test_list_apply()
{}

void test_list_contains()
{}

int main()
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
  CU_pSuite list_new = CU_add_suite("Test new list", NULL, NULL);
  CU_add_test(list_new, "list_new", test_list_new);
  
  CU_pSuite list_insert_and_length = CU_add_suite("Test list insert & length", NULL, NULL);
  CU_add_test(list_insert_and_length, "list_insert_and_length, list_length", test_list_insert_and_length);

  CU_pSuite list_get = CU_add_suite("Test list get!", NULL, NULL);
  CU_add_test(list_get, "list_get", test_list_get);

  CU_pSuite list_append = CU_add_suite("Test list append", NULL, NULL);
  CU_add_test(list_append, "list_append", test_list_append);

  CU_pSuite list_first = CU_add_suite("Test list first", NULL, NULL);
  CU_add_test(list_first, "list_first", test_list_first);
  
  CU_pSuite list_last = CU_add_suite("Test list last", NULL, NULL);
  CU_add_test(list_last, "list_last", test_list_last);
  
  //Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return 0;
}
