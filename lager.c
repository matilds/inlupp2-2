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

item_t *item_new(){
  item_t *new_item = calloc(1, sizeof(item_t));
  return new_item;
}

//////////////////////ADD ITEM//////////////////////



int tree_compare_fun_p(elem_t elem1 , elem_t elem2)
{
  char *str1 = elem1.p;
  char *str2 = elem2.p;

  if(strcmp(str1, str2) > 0)
    {
      return 1;
    }
  if (strcmp(str1, str2) < 0)
    {
      return -1;
    }
  else
    {
      return 0;
    }
}


bool storage_has_shelf(elem_t shelf, void *data){
  char *shelf_name = ((shelf_t *)shelf.p)->shelf_name;
  if(strcmp(shelf_name, (char *)data) == 0)
    {
      printf("Det ligger redan en annan vara på hyllan. \n");
      
      return true;
    }
  
  return false;
}

bool tree_item_has_shelf(key_t name, elem_t item, void *data)
{
  return list_apply(((item_t *)item.p)->storage, storage_has_shelf, data);
}

bool tree_has_shelf(tree_t *tree, char *shelf_name)
{
  return tree_apply(tree, inorder, tree_item_has_shelf, shelf_name);
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
      return edit_shelf(tree, item);
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

item_t *create_item(tree_t *tree)
{
  item_t *item = item_new();
  list_t *storage = list_new(NULL, NULL, NULL); /* TODO:  */

  elem_t elem_shelf;
  shelf_t *new_shelf = shelf_new();
  elem_shelf.p = new_shelf;
  
  item->desc = ask_question_string("Beskrivning: ");
  item->price = ask_question_int("Pris: ");
  do
    {
      new_shelf->shelf_name = ask_question_shelf("Lagerhylla: ");
    }
  while (tree_has_shelf(tree, new_shelf->shelf_name));

  new_shelf->amount = ask_question_int("Antal: ");
  
  list_append(storage, elem_shelf);
  item->storage = storage;

  return item;
}

void add_item_to_db(tree_t *tree)
{
  key_t name;
  name.p =  ask_question_string("Namn på varan: ");
  
  if(tree_has_key(tree, name))
    {
      printf("Varan finns redan, använder samma pris och beskrivning. \n");

      elem_t elem; //ändring
      tree_get(tree, name, &elem);
    
      item_t *item = (&elem)->p;
      edit_shelf(tree, item);
      print_item(item);
    
      return;
    }
  else
    {
      item_t *new_item = create_item(tree);
      char answer;
      
      new_item->name = name.p;
      print_item(new_item);
      
      while(true)
        {
          answer = ask_question_want_to_add_item();
          
          if(answer == 'J')
            {
              elem_t elem_new_item;
              elem_new_item.p = new_item;
              tree_insert(tree, name, elem_new_item);
              
              return;
            }
          if(answer == 'N')
            {
              return;
            }
          
          if(answer == 'R')
            {
              add_item_to_db(tree); /* TODO:  */ // anropa edit_item... istället
              
              return;
            }
        }
    }
}

//////////////////////LIST DB//////////////////////


void view_item(elem_t *tree_elems){
  int item_to_view = ask_question_int("Välj en siffra för att visa en vara");
  print_item(tree_elems[item_to_view - 1].p);
}

int edit_item(tree_t *tree){
  int edit_item_num = 0;
  while(edit_item_num > tree_size(tree) || edit_item_num < 1)
    {
      edit_item_num = ask_question_int("\nVilken vara vill du ändra?");
    }
  return edit_item_num;
}

void list_20_items(tree_t *tree, int *page_to_view)
{
  int item_amount = tree_size(tree);
  key_t *item_names = tree_keys(tree);
  int view_from_item = *page_to_view * 2;
  int cursor = view_from_item;
  
  for(; cursor < (view_from_item + 2) && cursor < item_amount; cursor++)
    {
      printf("%d. %s\n", cursor+1, item_names[cursor].p);
    }
}

// input view_or_edit: 0 if used to view. 1 if used to edit
int list_db(tree_t *tree, int view_or_edit)
{
  int item_amount = tree_size(tree);
  int page_to_view = 0;
  
  if(item_amount < 1){
    printf("Inga varor att lista. \n");
    return 0;
  }
  
  elem_t *item_elems = tree_elements(tree);
  
  list_20_items(tree, &page_to_view);

  char answer;
  int view_from_item;
  
  while(true){
    if(view_or_edit == 0){
      answer = ask_question_view_item();
    }
    else{
      answer = ask_question_list_items_to_edit();
    }
    
    if(answer == 'N'){
      page_to_view += 1;
      view_from_item = page_to_view * 2;
      if(view_from_item > item_amount){
        page_to_view -= 1;
        list_20_items(tree, &page_to_view);
        printf("Du är redan på sista sidan!\n");
      }
      else{
        list_20_items(tree, &page_to_view);
      }
    }
    if(answer == 'T'){
      page_to_view -= 1;
      if(page_to_view < 0){
        page_to_view = 0;
        list_20_items(tree, &page_to_view);
        printf("Du är redan på första sidan!\n");
      }
      else{
        list_20_items(tree, &page_to_view);
      }
    }
    if(answer == 'V'){
      if(view_or_edit == 0){
        view_item(item_elems);
        return 0;
      }
      else{
        return edit_item(tree);
      }
    }
    if(answer == 'A'){
      return 0;
    }
  }
}

//////////////////////EDIT ITEM//////////////////////

bool copy_shelves(elem_t shelf, void *data)
{
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  elem_t new_shelf_elem;
  
  *new_shelf = *(shelf_t *)shelf.p;
  new_shelf_elem.p = new_shelf;
  list_append((list_t *)data, new_shelf_elem);

  return true;
}

int edit_db(tree_t *items, action_t *undo, item_t *copy_p)
{
  int edit_item_num = list_db(items, 1);
  if(edit_item_num == 0)
    {
      return 0;
    }
  elem_t *all_items = tree_elements(items);
  item_t *item_to_edit = all_items[edit_item_num - 1].p;
  char answer;
  print_item(item_to_edit);
  
  while(true)
    {
      answer = ask_question_edit_item();
      if(answer == 'B')
        {
          printf("Nuvarande beskrivning: %s\n---------------------------\nNy beskrivning:", item_to_edit->desc);
          *copy_p = *item_to_edit;
          item_to_edit->desc = ask_question_string("");
        }
      else if(answer == 'P')
        {
          printf("Nuvarande pris: %d kr\n----------------------\nNytt pris:", item_to_edit->price);
          *copy_p = *item_to_edit;
          item_to_edit->price = ask_question_int("");
        }
      else if(answer == 'L')
        {
          list_apply(item_to_edit->storage, print_storage, NULL);
          
          undo->copy.name = item_to_edit->name;
          undo->copy.desc = item_to_edit->desc;
          undo->copy.price = item_to_edit->price;
      
          list_t *copy_storage = list_new(NULL, NULL, NULL);

          list_apply(item_to_edit->storage, copy_shelves, copy_storage);
          undo->copy.storage = copy_storage;
          
          edit_shelf(items, item_to_edit);
        }
      else if(answer == 'A')
        {
          return 0;
        }
      undo->merch = item_to_edit;
      return 1;
  }  
}



//////////////////////EVENT LOOP//////////////////////

void event_loop(tree_t *tree)
{
  item_t *copy_pointer = calloc(1, sizeof(item_t)); /* TODO:  */
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
          add_item_to_db(tree);
          break;
        case 'T':
         
          break;
        case 'H':
          list_db(tree, 0);
          break;
        case 'R':
          edit_db(tree, undo, copy_pointer); /* TODO:  */ //ta bort copy pointer
          break;

        case 'G':

          break;
             
        default:
          assert(false);
          break;
        }
    }
}

//////////////////////MAIN//////////////////////

int main()
{
  tree_t *tree = tree_new(NULL, NULL, NULL, tree_compare_fun_p); /* TODO:  */
  event_loop(tree);
  
  return 0;
}
