#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdbool.h>

typedef union {
  int   i;
  float f;
  char *s;
  char  c;
} answer_t;
typedef answer_t(*convert_func)(char *s);
typedef bool(*check_func)(char *s);

extern char *strdup(const char *);

int read_string(char *buf, int buf_siz);
bool is_number(char *str);
int ask_question_int(char *question);
char *ask_question_string(char *question);
void print(char *str);
void println(char *str);
bool not_empty(char *str);
answer_t ask_question(char *question, check_func check, convert_func convert);
bool is_menu_option(char *option);
bool is_shelf(char *shelf);
char str_to_char(char *str);
char *ask_question_shelf(char *question);
char ask_question_menu(void);
char ask_question_edit_item(void);
char ask_question_view_item(void);
char ask_question_list_items_to_edit(void);
char ask_question_want_to_add_item(void);
char ask_question_remove_item(void);


#endif
