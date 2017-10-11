#include "list.h"
#include <stdlib.h>

typedef struct link link_t;

struct list{
  element_copy_fun copy;
  element_free_fun free;
  element_comp_fun compare;
  link_t *first;
  link_t *last;
};

struct link{
  elem_t  element;
  link_t *next;
};

list_t *list_new(element_copy_fun copy, element_free_fun free, element_comp_fun compare)
{
  list_t *new_list = calloc(1, sizeof(list_t));
  new_list->copy = copy;
  new_list->free = free;
  new_list->compare = compare;

  return new_list;
}

int list_length(list_t *list)
{
  if(!list){
    return 0;
  }
  if (list->first == NULL){
    return 0;
  }
  if (list->last == NULL){
    return 0;
  }

  link_t *link_pointer = list->first;
  int count = 0;
  while (link_pointer != NULL){
    link_pointer = link_pointer->next;
    ++count;
  }

  return count;
}

void list_append(list_t *list, elem_t elem)
{
  if(!list){
    return;
  }
  
  element_copy_fun copy = list->copy;
  if (copy != NULL)
    {
      elem = copy(elem);
    }
  
  link_t *link = calloc(1, sizeof(link_t));
  link->element = elem;

  if(list->first == NULL){
    list->first = link;
  }
  if(list->last != NULL){
    link_t *last = list->last;
    last->next = link;
  }  
  list->last = link;  
}

void list_prepend(list_t *list, elem_t elem)
{
  if(!list){
    return;
  }
  
  element_copy_fun copy = list->copy;
  if (copy != NULL)
    {
      elem = copy(elem);
    }
  
  link_t *link = calloc(1, sizeof(link_t));
  link->element = elem;

  if(list->last == NULL){
    list->last = link;
  }
  if(list->first != NULL){
    link->next = list->first;
  }
  list->first = link;
}

link_t *list_get_link(list_t *list, int index)
{
  if(list == NULL)
    {
    return NULL;
    }
  link_t *link_pointer = list->first;

  for(int i = 0; i < index; ++i)
    {
    link_pointer = link_pointer->next;                      
    }
  return link_pointer;
}

bool list_insert(list_t *list, int index, elem_t elem)
{ 
  if(!list){
    return false;
  }
  
  element_copy_fun copy = list->copy;
  if (copy != NULL)
    {
      elem = copy(elem);
    }

  int length = list_length(list);
  if (index < 0){
    index = length + 1 + index;   
  }

  if (index == 0 || index < 0){
    list_prepend(list, elem);
    return true;
  }
  else if (index == length || index > length)
    {
    list_append(list, elem);
    return true;
  }

  link_t *link = calloc(1, sizeof(link_t));
  link->element = elem;

  link_t *link_pointer = list_get_link(list, index-1);
  link->next = link_pointer->next;
  link_pointer->next = link;

  return true;
}

void list_remove(list_t *list, int index, bool delete)
{
  if(!list){
    return;
  }

  int length = list_length(list);
  if (index < 0){
    index = length + 1 + index;
  }
  
  element_free_fun free_fun = list->free;
  if (index == 0 || index < 0)
    {
    link_t *tmp = list->first;
    list->first = list->first->next;
    if (delete == true)
      {
        free_fun(tmp->element);
        free(tmp);
        return;
      }
    }

  else if (index == length-1 || index > length-1)
    {
    link_t *tmp = list->last;
    link_t *link_pointer = list_get_link(list, index-1);
    list->last = link_pointer;
    list->last->next = NULL;
    if (delete == true)
      {
        free_fun(tmp->element);
        free(tmp);
        return;
      }
  }

  link_t *link_pointer = list_get_link(list, index-1);
  link_t *tmp = link_pointer->next;
  link_t *index_pointer = link_pointer->next;
  link_pointer->next = index_pointer->next;
  if (delete == true)
    {
      free_fun(tmp->element);
      free(tmp);
      return;
    }
  
  return;
}

bool list_get_orig(list_t *list, int index, elem_t *result)
{
  if(!list)
    {
      return false;
    }
  
  int length = list_length(list);
  if(index > length){
    return false;
  }
  
  if (index < 0){
    index = length + 1 + index;
  }
  
  if(index > length-1 || index < 0)
    {
      return false;
    }
  
  *result = (list_get_link(list, index))->element;

  return true;
}

bool list_get_node(list_t *list, int index, elem_t *result)
{
  if(list == NULL)
    {
      return false;
    }
  link_t *link_pointer = list->first;

  for(int i = 0; i < index; ++i)
    {
      link_pointer = link_pointer->next;
    }
  
  *result = link_pointer->element;

  return true;
}

bool list_get(list_t *list, int index, elem_t *result)
{ 
  int length = list_length(list);
  if (index < 0){
    index = length + index;   
  }
  
  if(index >= list_length(list) || index < 0)
    {
      return false;
    }
  else
    {
      list_get_node(list, index, result);
    }
  return true;
}

bool list_first(list_t *list, elem_t *result)
{
  return list_get(list, 0, result); 
}

bool list_last(list_t *list, elem_t *result)
{
  return list_get(list, -1, result);
}

void list_delete(list_t *list, bool delete)
{
  if(!list || list->first == NULL)
    {
    return;
  }
  if (delete == true)
    {
      link_t *tmp1 = list->first;
      link_t *tmp2;
      
      while(tmp1){
        tmp2 = tmp1;
        tmp1 = tmp1->next;
    
        element_free_fun free_fun = list->free;
        free_fun(tmp2->element);
        free(tmp2);
      }
    }
  return;
}

bool list_apply(list_t *list, elem_apply_fun fun, void *data)
{
  bool return_value = false;
    if(list_length(list) != 0){

    for(int index = 0; index < list_length(list); index++)
      {
        elem_t tmp;
        list_get(list, index, &tmp);   
        bool fun_result = fun(tmp, data);
        if (fun_result == true)
          {
            return_value = true;
          }           
      }
    }
    
    return return_value;
}

int list_contains(list_t *list, elem_t elem)
{
  int result = -1;
  element_comp_fun compare = list->compare;
  
  for(int index = 0; index < list_length(list); index++)
    {
      elem_t *tmp = calloc(1, sizeof(elem_t));
      
      list_get(list, index, tmp);   
      result = compare(*tmp, elem);
      if (result != 0)
        {
          return result;
        }
      
      free(tmp);
    }
  return result;
}
