#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "utils.h"
#include "list.h"
#include "tree.h"

//////////////////////STRUCTS//////////////////////

typedef struct shelf shelf_t;
typedef struct item item_t;
typedef struct action action_t;
typedef elem_t key_t;

struct shelf{
  char *shelf_name;
  int  amount;
};

struct item{
  char   *name;
  char   *desc;
  int    price;
  list_t *storage;
};

struct action{
  int type;
  /* TODO:  */ //enum { NOTHING, ADD, REMOVE, EDIT } type; 
  item_t *merch;
  item_t copy;
};

shelf_t *shelf_new(){
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  return new_shelf;
}

//////////////////////ADD ITEM//////////////////////

bool storage_has_shelf(elem_t shelf, void *data){
  char *shelf_name = ((shelf_t *)shelf.p)->shelf_name;
  if(strcmp(shelf_name, (char *)data) == 0)
    {
      printf("Det ligger redan en annan vara på hyllan.\n");
      
      return true;
    }
  
  return false;
}

bool tree_item_has_shelf(key_t name, elem_t item, void *data)
{
  return list_apply(((item_t *)item.p)->storage, storage_has_shelf, data);
}
/* TODO:  */
bool tree_has_shelf(tree_t *tree, char *shelf_name){
  tree_apply(tree, inorder, tree_item_has_shelf, shelf_name);
  if(strcmp(shelf_name, "change made") == 0){
    return true;
  }
  return false;
}

bool edit_item_shelf(elem_t shelf, void *data)
{
  char *shelf_name = ((shelf_t *)shelf.p)->shelf_name;
  
  if(strcmp(shelf_name, (char *)data) == 0)
    {
      int *amount = &((shelf_t *)shelf.p)->amount;
      *amount = ask_question_int("Hyllan finns redan. Nytt antal: ");
   
      return true;
    }
  
  return false;
}

bool item_has_shelf(list_t *storage, char *data)
{
  return list_apply(storage, edit_item_shelf, data);  
}

bool edit_shelf(tree_t *tree, item_t *item)
{
  char *shelf_name = ask_question_shelf("Namn på lagerhyllan: ");

  if(item_has_shelf(item->storage, shelf_name))
    {
      return true;
    }
  else if(tree_has_shelf(tree, shelf_name))
    {
      edit_shelf(tree, item);
    }
  else
    {
      elem_t elem_shelf;
      shelf_t *shelf = shelf_new();
      elem_shelf.p = shelf;

      int amount = ask_question_int("Hur många varor ska finnas på hyllan: ");  

      shelf->shelf_name = shelf_name;
      shelf->amount = amount;
      
      list_append(item->storage, elem_shelf);
    }
  
  return true;
}

bool print_storage(elem_t shelf, void *data)
{
  char *hylla = ((shelf_t *)shelf.p)->shelf_name;
  int antal =  ((shelf_t *)shelf.p)->amount;
  
  printf("Hylla: %s, Antal: %d\n", hylla, antal);

  return true;
}

void print_item(item_t *item)
{
  printf("\nNamn:   %s\n", item->name);
  printf("Beskrivning:   %s\n", item->desc);
  printf("Pris:  %d kr\n", item->price);
  
  list_apply(item->storage, print_storage, NULL);
}

void add_item_to_db(tree_t *tree)
{
  key_t name;
  name.p =  ask_question_string("Namn på varan: ");
  
  if(tree_has_key(tree, name)){
    printf("Varan finns redan, använder samma pris och beskrivning.\n");

    elem_t *elem;
    tree_get(tree, name, elem);
    
    item_t *item = elem->p;
    edit_shelf(tree, item);
    print_item(item);
    return;
  }
  /* TODO:  *//* TODO:  *//* TODO:  */
  item_t *new_item = input_item(tree);
  new_item->name = name;
  print_item(new_item);
  char answer;
  while(true){
    answer = ask_question_want_to_add_item();
    if(answer == 'J'){
      tree_insert(tree, name, new_item);
      return;
    }
    if(answer == 'N'){
      return;
    }
    if(answer == 'R'){
      add_item_to_db(tree);
      return;
    }
  }
}

void event_loop(tree_t *tree)
{
  action_t *undo = calloc(1, sizeof(action_t));
  undo->type = 0;
  bool run_loop = true;
  while(run_loop)
    {
      switch(ask_question_menu())
        {
        case 'A':
          run_loop = false;
          break;
        case 'L':

          break;
        case 'T':
         
          break;
        case 'H':
          
          break;
        case 'R':

          break;

        case 'G':

          break;
             
        default:
          assert(false);
          break;
        }
    }
}

int main()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, NULL); /* TODO:  */
  event_loop(tree);
  
  return 0;
}
