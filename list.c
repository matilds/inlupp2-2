#include "list.h"
#include <stdlib.h>


typedef struct link link_t;
typedef struct list list_t;

struct link
{
  elem_t element;
  link_t *next;
};

struct list
{
  element_comp_fun compare;
  element_free_fun free;
  element_copy_fun copy;
  link_t *first;
  link_t *last;
};


list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *new_list = calloc(1, sizeof(list_t));
  new_list->copy = copy;
  new_list->free = free;
  new_list->compare = compare;
  return new_list;
}

link_t *link_new(elem_t elem)
{
  link_t *new = calloc(1 , sizeof(link_t));
  new->element = elem;
  return new;
                       
}


bool list_insert(list_t *list, int index, elem_t elem)
{
  element_copy_fun copy = list->copy; 
  if( copy != NULL)
    {
      elem = copy(elem);
    }
link_t *new = calloc(1 , sizeof(link_t));
  int i = 0;
  link_t *current = list->first;
  link_t *pre;
  if(index < 0)
    {
      index = list_length(list)  + index;
    }
  if(index >= list_length(list))
    {
      return false;
    }
  if(index == 0)
    {
      list_prepend(list, elem);
      return true;
    }
 
   
  while(i < index)
    {
     
      
      pre = current;
      current = current->next;
      i++;
        
    }
   new->element = elem;
  new->next = current;
  pre->next = new;
  return true;

  

}

