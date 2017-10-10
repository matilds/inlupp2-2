#include "tree.h"
#include <CUnit/CUnit.h>
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

void key_elem_free(elem_t elem)
{
  if (elem.i != 0)
    {
      elem.i = 0;
    }
}






void test_tree_new()
{
  CU_ASSERT_TRUE(tree_new(NULL, NULL, NULL, NULL) != NULL);
}






void test_tree_delete()
//hur ska man ta bort träd? tar man bort trädet även om man inte kör free tree/key funktionerna??
  
{ 
  tree_t *tree  = tree_new(NULL, key_elem_free, key_elem_free,tree_compare_fun);
  CU_ASSERT_TRUE(tree != NULL);
  tree_delete(tree, true, true);
  CU_ASSERT_TRUE(tree_size(tree) == 0);
    
 
  
  tree_key_t key; key.i = 0;
  elem_t elem; elem.i = 0;
  tree_t *tree1  = tree_new(NULL,key_elem_free,key_elem_free,tree_compare_fun);
  int i = 0;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,elem);
      i++;
    }
  tree_delete(tree1, true, true);
  CU_ASSERT_TRUE(tree_size(tree) == 0);
   
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

  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,elem);
      i++;
    }
  CU_ASSERT_TRUE(tree_size(tree1) == 100);
 
}





void test_tree_depth()
{
  tree_t *tree  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 0);
  
  elem_t key; key.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  CU_ASSERT_TRUE(tree_depth(tree) == 1);
  
  elem_t key1; key1.i = 2;
  elem_t elem1; elem1.i = 555;
  tree_insert(tree , key1 , elem1);
  
  elem_t key2; key2.i = 3;
  elem_t elem2; elem2.i = 555;
  tree_insert(tree , key2 , elem2);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 2); 

  elem_t key3; key3.i = 4;
  elem_t elem3; elem3.i = 555;
  tree_insert(tree , key3 , elem3);
  
  elem_t key4; key4.i = 5;
  elem_t elem4; elem4.i = 555;
  tree_insert(tree , key4 , elem4);

  elem_t key5; key5.i = 6;
  elem_t elem5; elem5.i = 555;
  tree_insert(tree , key5 , elem5);
  
  elem_t key6; key6.i = 7;
  elem_t elem6; elem6.i = 555;
  tree_insert(tree , key6 , elem6);
  
  elem_t key7; key7.i = 8;
  elem_t elem7; elem7.i = 555;
  tree_insert(tree , key7 , elem7);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 4); // depth = 5  

  elem_t key8; key8.i = 9;
  elem_t elem8; elem8.i = 555;
  tree_insert(tree , key8 , elem8);

  elem_t key9; key9.i = 10;
  elem_t elem9; elem9.i = 555;
  tree_insert(tree , key9 , elem9);
  
  CU_ASSERT_TRUE(tree_depth(tree) == 4); //detpth = 5 
  
}

void test_tree_insert()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);
  elem_t elem1; elem1.i = 555;
  tree_key_t key1; key1.i = 1;

  CU_ASSERT_TRUE(tree_insert(tree, key1, elem1) == true);
  CU_ASSERT_TRUE(tree_insert(tree, key1, elem1) == false);
  
  elem_t elem2; elem1.i = 555;
  tree_key_t key2; key1.i = 2;

  CU_ASSERT_TRUE(tree_insert(tree, key2, elem2) == true);
  
  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  tree_key_t key; key.i = 0;
  elem_t elem; elem.i = 555;
  while (i < 32)
    {
      key.i = i;
      CU_ASSERT_TRUE(tree_insert(tree1,key,elem) == true);
      i++;
    }                 
  CU_ASSERT_TRUE(tree_size(tree1) == 32);
  CU_ASSERT_TRUE(tree_depth(tree1) == 6);
}





void test_tree_has_key()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);
  tree_key_t key1; key1.i = 1;

  CU_ASSERT_TRUE(tree_has_key(tree, key1) == false);

  elem_t key; key.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  CU_ASSERT_TRUE(tree_has_key(tree, key1) == true);

  
  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,elem);
      i++;
    }
  tree_key_t super_key; super_key.i = 55;
  CU_ASSERT_TRUE(tree_has_key(tree, super_key) == true); 
}








void test_tree_get()
{

  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);
  tree_key_t key1; key1.i = 1;
  elem_t *elem_get = calloc(1, sizeof(elem_t));
  bool get = tree_get(tree, key1, elem_get);
  CU_ASSERT_TRUE(get == false);
  // CU_ASSERT_TRUE(elem_get == NULL); //kolla elem hur????
  
  elem_t key; key.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  get = tree_get(tree, key1, elem_get);
  CU_ASSERT_TRUE(get == true);
  // CU_ASSERT_TRUE(elem_get != NULL); // Kan man testa så här?????
  

  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,elem);
      i++;
    }
  tree_key_t super_key; super_key.i = 55;
  CU_ASSERT_TRUE(tree_get(tree1, super_key, elem_get) == true); 
  
}








void test_tree_remove()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);
  tree_key_t key; key.i = 55;
  elem_t *removed_elem = calloc(1,sizeof(elem_t));
  CU_ASSERT_TRUE(tree_remove(tree, key, removed_elem) == false); 

  elem_t key1; key1.i = 1;
  elem_t elem1; elem1.i = 555; 
  tree_insert(tree, key1, elem1);

  CU_ASSERT_TRUE(tree_remove(tree, key1, removed_elem) == true);
  CU_ASSERT_TRUE(tree_remove(tree, key1, removed_elem) == false);
  
  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  elem_t elem; elem.i = 555;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,elem);
      i++;
    }
  tree_key_t super_key; super_key.i = 55;
  CU_ASSERT_TRUE(tree_remove(tree1, super_key, removed_elem) == true); 
}


void test_tree_update()
{
  
  tree_t *tree = tree_new(NULL,NULL,NULL,tree_compare_fun);
  elem_t elem; elem.i = 555;
  tree_key_t key; key.i = 1;
  elem_t result; 

  CU_ASSERT_TRUE(tree_update(tree, key, elem, result) == false);
  
  tree_insert(tree, key, elem);
  elem_t new_elem; new_elem.i = 1123;

  CU_ASSERT_TRUE(tree_update(tree, key, new_elem, result));
  
  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  
  int i = 0;
  elem_t key1; key1.i = 0;
  elem_t element; elem.i = 555;
  while (i < 10)
    {
      key1.i = i;
      tree_insert(tree1,key1,element);
      i++;
    }
  tree_key_t super_key; super_key.i = 55;
  //tree_remove// key som inte finns i trädet??? 
  CU_ASSERT_TRUE(tree_update(tree1, super_key, element, result) == false);
  
}


void test_tree_keys()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);

  CU_ASSERT_TRUE(tree_keys(tree) == NULL);

  elem_t key; key.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  
  tree_key_t *key_array = tree_keys(tree);

  CU_ASSERT_TRUE(key_array[0].i == 1);

   tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  elem_t element; elem.i = 555;
  while (i < 100)
    {
      key.i = i;
      tree_insert(tree1,key,element);
      i++;
    }
  //  CU_ASSERT_TRUE(tree_keys(tree1)[5].i == 5); 
  // CU_ASSERT_TRUE(tree_keys(tree1)[79].i == 79); 
}







void test_tree_elements()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun);

  CU_ASSERT_TRUE(tree_elements(tree) == NULL);

  elem_t key; key.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key , elem);

  
  tree_key_t *element_array = tree_elements(tree);

  CU_ASSERT_TRUE(element_array[0].i == 555);

  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  elem_t key1; key1.i = 0;
  elem_t element; element.i = 0;
  while (i < 100)
    {
      key1.i = i;
      element.i = i+1;
      tree_insert(tree1,key1,element);
      i++;
    }
  CU_ASSERT_TRUE(tree_elements(tree1)[5].i == 6); 
  CU_ASSERT_TRUE(tree_elements(tree1)[79].i == 80); 
}

bool print_elem_i(tree_key_t key, elem_t elem, void *data)
{
  int num =  *((int*)data);
 

  printf("%d. %d\n", num, elem.i);
  *((int*)data) =  *((int*)data) + 1;
  return true;
  
}
void test_tree_apply()
{
  //returnar rätt? true,true,false == false
  
  tree_t*tree = tree_new(NULL, NULL, NULL, tree_compare_fun);
  int num = 1;
  CU_ASSERT_TRUE(tree_apply(tree, inorder, print_elem_i, &num) == false );

  elem_t key1; key1.i = 1;
  elem_t elem; elem.i = 555;
  tree_insert(tree, key1 , elem);

  CU_ASSERT_TRUE(tree_apply(tree, inorder, print_elem_i, &num) == true );
  
  tree_t *tree1  = tree_new(NULL,NULL,NULL,tree_compare_fun);
  int i = 0;
  elem_t key; key.i = 0;
  elem_t element; element.i = 0;
  while (i < 20)
    {
      key.i = i;
      element.i = i+1;
      tree_insert(tree1,key,element);
      i++;
    }
  int a_num = 1;
  CU_ASSERT_TRUE(tree_apply(tree, inorder, print_elem_i, &a_num) == true);
  elem_t eleme; eleme.i = 234234234;
  elem_t keyn; keyn.i = 21;
  tree_insert(tree1, keyn, eleme);
  CU_ASSERT_TRUE(tree_apply(tree, inorder, print_elem_i, &a_num) == true );
  // borde den ge false här???
  //
  
}



int main()
{
  // Initialise
  CU_initialize_registry();

  // Set up suites and tests
    CU_pSuite tree_new = CU_add_suite("Test new tree", NULL, NULL);
  CU_add_test(tree_new, "Tree_new", test_tree_new);
  
  CU_pSuite tree_delete = CU_add_suite("Test tree delete", NULL, NULL);
  CU_add_test(tree_delete, "Tree_delete", test_tree_delete);

  CU_pSuite tree_has_key = CU_add_suite("Test tree has key", NULL, NULL);
  CU_add_test(tree_has_key, "Tree_has_key", test_tree_has_key);

  CU_pSuite tree_size = CU_add_suite("Test tree size", NULL, NULL);
  CU_add_test(tree_size, "Tree_size", test_tree_size);
  
  CU_pSuite tree_depth = CU_add_suite("Test tree depth", NULL, NULL);
  CU_add_test(tree_depth, "Tree_depth", test_tree_depth);

  CU_pSuite tree_insert = CU_add_suite("Test tree insert", NULL, NULL);
  CU_add_test(tree_insert, "Tree_insert", test_tree_insert);

  CU_pSuite tree_get = CU_add_suite("Test tree get", NULL, NULL);
  CU_add_test(tree_get, "Tree_get", test_tree_get);

  CU_pSuite tree_remove = CU_add_suite("Test tree remove", NULL, NULL);
  CU_add_test(tree_remove, "Tree_remove", test_tree_remove);
   
  CU_pSuite tree_update = CU_add_suite("Test tree update", NULL, NULL);
  CU_add_test(tree_update, "Tree_update", test_tree_update);
   
  CU_pSuite tree_keys = CU_add_suite("Test tree keys", NULL, NULL);
  CU_add_test(tree_keys, "Tree_keys", test_tree_keys);
  
  CU_pSuite tree_elements = CU_add_suite("Test tree elements", NULL, NULL);
  CU_add_test(tree_elements, "Tree_elements", test_tree_elements);
   
  CU_pSuite tree_apply = CU_add_suite("Test tree apply", NULL, NULL);
  CU_add_test(tree_apply, "Tree_apply", test_tree_apply);
 
   //Actually run tests
  CU_basic_run_tests();

  // Tear down
  CU_cleanup_registry();
  return 0;
}
