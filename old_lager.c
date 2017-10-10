#include <stdbool.h>
#include <ctype.h>
#include "lager.h"
#include "utils.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include "tree.h" 
#include <stdlib.h>



////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

char str_to_upper(char *str)
{
  return toupper (str[0]);


}

bool is_shelf(char *str)
{
  int i = 0;
  if(isupper(str[i]) && isalpha(str[i]) && isdigit(str[i+1]))
    {
      while(str[i+2] != '\0')
        {
          if(isdigit(str[i+2]))
            {
              i += 1;
            }
          else
            {
              return false;
            }
        }  
    }
  else
    {
      return false;
    } 
  return true; 
}
    


char *ask_question_shelf(char *question)
{
  char *shelf;
  shelf = ask_question(question , is_shelf , (convert_func) strdup).s;
  return shelf;
} 

shelf_t *make_shelf(char *name , int amount)
{
  shelf_t *new_shelf = calloc(1 , sizeof(shelf_t));
  new_shelf->amount = amount; 
  new_shelf->shelf = strdup(name);
 
  return new_shelf;
}
list_t *make_shelf_list(char* name , int amount)
{
  list_t *new = list_new();
  shelf_t *shelf = make_shelf(name , amount);
  list_prepend(new , shelf);
  return new;
} 

item_t *item_new(char *name , char *desc , int price , list_t *shelf )
{
  item_t *new = calloc(1 , sizeof(item_t));
  new->shelf = shelf; 
  new->name = name;
  new->desc = desc;
  new->price = price;
  return new;
}
/*
bool is_shelf_finns(tree_t *tree , char *shelf1)
{
  
  item_t **elemlist = tree_elements(tree);       
  for(int i = 0; i < tree_size(tree); i++)
    {
      item_t *item = elemlist[i];
      for(int j = 0; j < list_length(item->shelf); j++)
        {
          shelf_t *shelf  = *list_get(item->shelf , j);
          printf("comparing shelf %s , actual shelf: %s\n" , shelf->shelf , shelf1);
          if(strcmp(shelf1 ,  shelf->shelf) == 0 )
            {
              return true;
                     
            }
        }
    }
  return false;
*/


bool check_if_exist(tree_t *tree, char *new_shelf)
{
  item_t **elemlist = tree_elements(tree);
  for (int i = 0; i < tree_size(tree); i++)
    {
      item_t *item = elemlist[i];
      for (int j = 0; j < list_length(item->shelf); j++)
        {  shelf_t *shelf = *list_get(item->shelf, j);
          int compare = strcmp(new_shelf, shelf->shelf);
          if (compare == 0)
            {
              return true;
            }
        }
    }
  return false;


}

void print_shelf(shelf_t *shelf, void *index)
{
 
 
  char *name = shelf->shelf;
  int amount = shelf->amount;
  printf("Hylla %d: %s\t Antal: %d\n", *(int*)index, name, amount);
  ( *((int*)index))++;
  
}
item_t *get_item(tree_t *tree , int index  , char *c)
{
  int j = 1;
  char **keys = tree_keys(tree);
  int i = atoi(c) - 1;
  i = i + (index * 2);
  char *key = keys[i];
  item_t *item = tree_get(tree, key);
  char *desc = item->desc;
  int price = item->price;
  printf("-------------Vara--------------\n"); 
  printf("Namn: %s\nDescription: %s\nPrice: %d\n", key, desc, price);
  list_apply(item->shelf, print_shelf, &j);
  printf("-------------------------------\n");
  return item;
}

item_t *print_single_item(tree_t *tree, int index)
{
  if (tree_size(tree) == 0)
    {
     printf("\nLagret är tomt\n");
     return NULL;
    }
  char *c = ask_question_string("\nVälj den siffra du vill se, tryck A för att avsluta\n");
  while(!(strcmp("A", c) == 0) && !(strcmp("a", c) == 0) && !(is_number(c)))
    {
      printf("Felaktig input\n");
      c = ask_question_string("\nVälj den siffra du vill se, tryck A för att avsluta");
    }
  
  if (strcmp("A", c) == 0 || strcmp("a", c) == 0)
    {
      return NULL;
    }
   item_t *item =get_item(tree, index, c);
   return item;
}

bool is_correct_char_list(char *c)
{
  {
    if(strlen(c) != 1)
      {
        return false;
      }
    if(strchr("jJnN" , *c))
      {
        return true;
      }
    else
      {
        return false;
      }
  }
}

char ask_question_print_list()

  
{
  
  answer_t a = ask_question("vill du se fler varor [j]a, [n]ej\n" , is_correct_char_list , (convert_func) str_to_upper);
  return a.i;
}



int print_list(tree_t *tree, int i, int l)
{
  int j = 1;
  char **keys = tree_keys(tree);
  int length = tree_size(tree);
  printf("----------LISTA AV VAROR------------\n");
  while(i <= length && j <= 2)
    {
      printf("%d. %s\n", j, keys[i-1]);
      i++;
      if (j >= 2 && j < tree_size(tree) - (l*2))
        {
          
          char c = ask_question_print_list();
          if (c == 'J')
            {
              l++;
              print_list(tree, i, l);
            }
          if (c == 'N')
            {
            }
        }
      j++;
    }
 
  return l;
   
}



  //////////////////////////////////
 /////REDIGERA EN VARA////////////
////////////////////////////////



bool is_correct_char_edit(char *c)
{
  {
    if(strlen(c) != 1)
      {
        return false;
      }
    if(strchr("bBpPlLaAtT" , *c))
      {
        return true;
      }
    else
      {
        return false;
      }
  }
}

void edit_menu()
{
  printf("\n[B]eskrivning\n");
  printf("[P]ris\n"); 
  printf("[L]agerhylal\n");
  printf("an[t]al\n");
  printf("\nVälj alternativ eller [a]vbryt\n");
  
}
int ask_question_edit()
                     
{
  
  answer_t a = ask_question("" , is_correct_char_edit , (convert_func) str_to_upper);
  return a.i;
}

////////////////////////
//////////
/////////////////77777
/////////////////
//////////////////
/////////////
/////////
//////////

/*
action_t *undo_action(item_t item1, void *item2 , int i)
{
  action_t *undo = calloc(1, sizeof(action_t));
  if(i== 0)
    {
      printf("Det finns inget att ångra\n");
      return NULL;
    }
  if(i==1)
    {
      printf("add kommer snart\n");
      return NULL;
    }
  if(i==2)
    {
      printf("remove kommer snart\n");
    }
  if(i==3)
    {
      
    }
  
  undo->merch = item;
  undo->copy = *item;
  return undo;
  
}
*/

shelf_t *copy_shelf(shelf_t *shelf)
{
  shelf_t *copy = calloc(1 , sizeof(shelf_t));
  copy->shelf = shelf->shelf;
  copy->amount = shelf->amount;
  return copy;
  
}

list_t *copy_list( list_t *list)
{
  list_t *copy = calloc(list_length(list), sizeof(list_t));
  link_t *current = list->first;
  for( int i = 0; i < list_length(list);i++)
    {
      list_append(copy, copy_shelf(current->element));
      current = current->next;
    }
  return copy;
}




item_t *copy_item(item_t *item)
{
  item_t *copy = calloc(1,sizeof(item_t));
  copy->name = item->name;
  copy->desc = item->desc;
  copy->price = item->price;
  copy->shelf = copy_list(item->shelf);
  return copy;
}















int edit_item_aux(tree_t *tree , item_t *item , action_t* undo)
{

  int v = 1;
  
  edit_menu();
  int answer = ask_question_edit();

  if( answer == 'B')
    {
      undo->type = 3;
      undo->copy = *copy_item(item);
      printf("Nuvarande beskrivning: %s\n", item->desc);
      printf("---------------------------------\n");
      char * new_desc = ask_question_string("Ny beskrivning");
      item->desc = new_desc;
    }
  if( answer == 'P')
    {
      undo->type = 3;
      undo->copy = *copy_item(item);
      printf("Nuvarande Pris: %d\n", item->price);
      printf("---------------------------------\n");
      int new_price = ask_question_int("Nytt pris:");
      item->price = new_price;
      
    }
  if(answer == 'L')
    {
      undo->type = 3;
      undo->copy = *copy_item(item);
      printf("Nuvarande Hyllor:\n");
      list_apply(item->shelf , print_shelf, &v);
      printf("---------------------------------\n");
      int hylla = ask_question_int("vilken hylla vill du redigera, ange hyllans nummer?\n");
      while( hylla> list_length(item->shelf))
        {
          printf("varan finns ej på hylla %d\n", hylla);
          hylla = ask_question_int("vilken hylla vill du redigera, ange hyllans nummer?\n");
        }
      
        shelf_t *old_shelf = *list_get(item->shelf, hylla-1);
        char *new_shelf = ask_question_shelf("Vilken hylla vill du flytta varorna till?");
        while(check_if_exist(tree, new_shelf) == true)
          {
            printf("Denna hylla är upptagen\n");
            new_shelf = ask_question_shelf("Vilken hylla vill du flytta varorna till?");
          }
        old_shelf->shelf = new_shelf;
      
      }
      if(answer == 'T')
        {
          undo->type = 3;
          undo->copy = *copy_item(item);
          printf("Nuvarande antal:\n");
          list_apply(item->shelf, print_shelf, &v);
          printf("---------------------------------\n");
          int hylla_a = ask_question_int("vilken hylla vill du ändra antalet på, ange hyllans nummer\n");
          while(hylla_a > list_length(item->shelf))
            {
              printf("varan finns inte på hylla %d\n" , hylla_a);
              hylla_a = ask_question_int("vilken hylla vill du ändra antalet på, ange hyllans nummer\n");
            }
          int new_amount = ask_question_int("Nytt antal:");
          shelf_t *old_amount = *list_get(item->shelf, hylla_a -1);
              old_amount->amount = new_amount;
            }
      
        
        
          

      undo->merch = item;
      
      if(answer == 'A')
    {
      return 0;
    }
          return 1;
}

void edit_item(tree_t *tree , action_t *undo)
{
 
  char *answer;
  do
    {
      
      int i =print_list(tree , 1 , 0);
      item_t *item = print_single_item(tree, i);
      if(item == NULL)
        {
          return;
        }
      int k = edit_item_aux(tree , item, undo);
      if(k==0)
        {
          return;
        }
      answer = ask_question_string("vill du [R]edigera mer eller [a]vsluta redigerngsläget\n");
      while(strchr("aA", *answer) == false && strchr("rR", *answer) == false)
        {
          printf("felaktigt inmatning\n");
          answer = ask_question_string("vill du [R]edigera mer eller [a]vsluta redigerngsläget\n");
        }
    }
  while(strchr("aA", *answer) == false);
    }
      








  ///////////////////////////////////
 //////////////////////////////////
/////////////////////////////////



bool is_correct_char_add(char *c)
{
  if(strlen(c) != 1)
    {
      return false;
    }
  if(strchr("jJnNrR" , *c))
    {
      return true;
    }
  else
    {
      return false;
    }
}


 int ask_question_add()
{
  
  answer_t a = ask_question("Vill du lägga till varan?\n[j]a, [n]ej, [r]edigera\n" , is_correct_char_add , (convert_func) str_to_upper);
  return a.i;
}


 
item_t *input_item(tree_t *tree)
{
  
  char *name = ask_question_string("name?");
  if(tree_has_key(tree , name))
    {
      item_t *item1 = tree_get(tree , name);
      char *shelf1 = ask_question_shelf("Hylla?");
      while( (check_if_exist(tree , shelf1)) == true)
        {
          printf("hyllan är upptagen\n");
          shelf1 = ask_question_shelf("Hylla?");
        }
      
      int amount1 = ask_question_int("amount?");
      list_prepend ( item1->shelf , make_shelf(shelf1  , amount1)) ;
      printf("-------------Vara--------------\n");
      printf("Namn: %s\nBeskrivning: %s\nPris: %d\nHylla: %s\nAntal: %d\n" , name ,item1->desc , item1->price , shelf1 , amount1);
      printf("-------------------------------\n");
      return item1;
    }
  else {
    
  }
    {
      char *desc = ask_question_string("description");
      int price = ask_question_int("price?");

      char *shelf = ask_question_shelf("shelf?");
      while( (check_if_exist(tree , shelf)) == true)
        {
          printf("hyllan är upptagen\n");
          shelf = ask_question_shelf("Hylla?");
        }
  
      int amount = ask_question_int("amount?");
      list_t *shelf_list = make_shelf_list(shelf , amount);
      printf("-------------Vara--------------\n");
      printf("Namn: %s\nBeskrivning: %s\nPris: %d\nHylla: %s\nAntal: %d\n" , name , desc , price , shelf , amount);
      printf("-------------------------------\n");
  
  
    item_t *item = item_new(name , desc , price , shelf_list);
    return item;
    }
}

void insert_item_to_tree(tree_t *tree )
{
  item_t *item = input_item(tree);
  int answer = ask_question_add();

  if(answer == 'R')
    {
      do
        {
          int i = 1;
          printf("-------------Vara--------------\n");
         printf("Namn: %s\nBeskrivning: %s\nPris:  %d\n" , item->name ,item->desc , item->price  );
         list_apply(item->shelf, print_shelf , &i);
         printf("-------------------------------\n");
          answer = ask_question_add();
          if(answer == 'N' || answer == 'J')
            {
              break;
            } 
        }
      while(true);
           
    }
  if(answer == 'J')
    {
      
      tree_insert(tree , item->name , item);
    }
  if(answer == 'N')
    {
      return;
      }
   
  
}

bool is_correct_char(char *c)
{
  if(strlen(c) != 1)
    {
      return false;
    }
  if(strchr("LlTtRrGgHhAaGg" , *c))
    {
      return true;
    }
  else
    {
      return false;
    }
}


 






void print_menu()
{
  printf("\nVälkommen till lagerhantering 1.0\n=================================\n");
  printf("\n[L]ägga till en vara \n[T]a bort en vara \n[R]edigera en vara \nÅn[g]ra senaste ändringen \nLista [h]ela varukatalogen \n[A]vsluta\n \n");
  return;
}
 

 int ask_question_menu()
{
  
  print_menu();
  answer_t a = ask_question("Vad vill du göra?" , is_correct_char , (convert_func) str_to_upper);
  return a.i;
}
 
int main()
{
  action_t *undo = calloc(1, sizeof(action_t));
  undo->type = 0;
  bool b = true;
  tree_t *tree = tree_new();
  while(b)
    {
      char a = 0;
      a = ask_question_menu();
      switch(a)
        {
        case 'A':
          b = false;
          break;
        case 'L':
          // no_items = add_item_to_db(items , no_items);
          insert_item_to_tree(tree);
          break;
        case 'T':
          //no_items = remove_item_from_db(items , no_items);
         
          break;
      
        case 'H':
          if(tree_size(tree) == 0)
            {
              printf("Det finns inga items\n");
            }
          else
            {
              print_list(tree , 1 , 0);
              char *a = ask_question_string("\nTryck A för att gå tillbaka till menyn");
              while(strchr("aA" , *a) == false)
                {
                  printf("felaktig inmatning\n");
                  a = ask_question_string("Tryck A för att gå tillbaka till menyn");

                }
            }
          break;
           case 'R':
             if(tree_size(tree) == 0)
               {
                 printf("Det finns inga varor att redigera\n"); 
               }
             else
               {
                 edit_item(tree, undo);
               }
             // edit_db(items , no_items);
             break;

               case 'G':

                
                 
          if(undo->type == 0)
            {
              puts("finns intget att ångra");
            }

          if(undo->type == 1)
            {
              puts("inte implementerad än");
            }
          
          if(undo->type == 2)
            {
              puts("inte implementerad än");
            }

          if(undo->type == 3)
            {
              *undo->merch = undo->copy;
              undo->type = 0;
            }
           
             break;
             
        default:
          puts("error");
          break;
        }
    }
}
