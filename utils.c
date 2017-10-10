#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int read_string(char *buf, int buf_siz){

  int pos = 0;
  char c;

  do{
    c = getchar();
    buf[pos] = c;
    if (c != '\n'){
      pos++;
    }
  }
  while(c != '\n' && c != EOF && ((buf_siz - 1) > pos));
  buf[pos] = '\0';
  return(pos);
}


bool is_number(char *str){

  int length = strlen(str);  

  if ((str[0] == '-') || (isdigit(str[0]))){
  
    for(int i = 1; i < length; i++){
      if(isdigit(str[i]) == false){
        return false;
      }
    }
  }
  else{
    return false;
  }
  return true;
}


bool is_shelf(char *shelf){
  if(isalpha(shelf[0])){
    for(int i = 1; i < strlen(shelf); i++){

      if(!(isdigit(shelf[i]))){
	return false;
      }
    }
  }
  else{
    return false;
  }
  return true;
}


bool is_menu_option(char *option){
  if(strlen(option) > 1){
    return false;
  }
  else if(option[0] == 'L' || option[0] == 'l' ||
	  option[0] == 'T' || option[0] == 't' ||
	  option[0] == 'R' || option[0] == 'r' ||
          option[0] == 'G' || option[0] == 'g' ||
	  option[0] == 'H' || option[0] == 'h' ||
	  option[0] == 'A' || option[0] == 'a'){
    return true;
  }
  else{
    return false;
  }
}


bool is_edit_item_option(char *option){
  if(strlen(option) > 1){
    return false;
  }
  else if(option[0] == 'B' || option[0] == 'b' ||
	  option[0] == 'P' || option[0] == 'p' ||
	  option[0] == 'L' || option[0] == 'l' ||
	  option[0] == 'A' || option[0] == 'a'){
    return true;
  }
  else{
    return false;
  }
}


bool is_view_item_option(char *option){
  if(strlen(option) > 1){
    return false;
  }
  else if(option[0] == 'N' || option[0] == 'n' ||
	  option[0] == 'T' || option[0] == 't' ||
	  option[0] == 'V' || option[0] == 'v' ||
	  option[0] == 'A' || option[0] == 'a'){
    return true;
  }
  else{
    return false;
  }
}


bool is_want_to_add_item_option(char *option){
  if(strlen(option) > 1){
    return false;
  }
  else if(option[0] == 'J' || option[0] == 'j' ||
	  option[0] == 'N' || option[0] == 'n' ||
	  option[0] == 'R' || option[0] == 'r'){
    return true;
  }
  else{
    return false;
  }
}


char str_to_char(char *str){
  return str[0];
}


answer_t ask_question(char *question, check_func check, convert_func convert){
  int buf_siz = 255;
  char buf[buf_siz];

  printf("%s ", question);
  read_string(buf, buf_siz);

  while(check(buf) == false){
    printf("Felaktig inmatning %s\n%s", buf, question);
    read_string(buf, buf_siz);
  }

  return (convert(buf));
}


char *ask_question_string(char *question){
  return ask_question(question, not_empty, (convert_func) strdup).s;
}


int ask_question_int(char *question){
  answer_t answer = ask_question(question, is_number, (convert_func) atoi);
  return answer.i; 
}


char *ask_question_shelf(char *question){
  return ask_question(question, is_shelf, (convert_func) strdup).s;
}


char ask_question_menu(void){
  char *menu = "\n[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukorgen\n[A]vsluta\n";
  answer_t answer = ask_question(menu, is_menu_option, (convert_func) str_to_char);
  return toupper (answer.c);
}


char ask_question_edit_item(void){
  char *menu = "\n[B]eskrivning\n[P]ris\n[L]agerhylla & antal\n[A]vbryt\n";
  answer_t answer = ask_question(menu, is_edit_item_option, (convert_func) str_to_char);
  return toupper (answer.c);
}


char ask_question_view_item(void){
  char *menu = "\n[N]ästa sida med varor\n[T]idigare sida med varor\n[V]isa en vara\n[A]vbryt\n";
  answer_t answer = ask_question(menu, is_view_item_option, (convert_func) str_to_char);
  return toupper (answer.c);
}


char ask_question_list_items_to_edit(void){
  char *menu = "\n[N]ästa sida med varor\n[T]idigare sida med varor\n[V]älj vara att ändra\n[A]vbryt\n";
  answer_t answer = ask_question(menu, is_view_item_option, (convert_func) str_to_char);
  return toupper (answer.c);
}


char ask_question_want_to_add_item(void){
  char *menu = "\nVill du lägga till varan?\n[J]a\n[N]ej\n[R]edigera\n";
  answer_t answer = ask_question(menu, is_want_to_add_item_option, (convert_func) str_to_char);
  return toupper (answer.c);
}


void print(char *str){  
  for(int n = 0; str[n] != '\0'; n++){
    putchar(str[n]);
  }
}


void println(char *str){
  print(str);
  print("\n");
}


bool not_empty(char *str){
  return strlen(str) > 0;
}
