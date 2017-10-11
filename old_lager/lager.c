#include <stdio.h>
#include "utils.h"
#include "list.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct shelf shelf_t;
typedef struct item item_t;
typedef struct action action_t;

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

/////////////////////////////////// functions used by list_apply /////////////////////////////////
void print_storage(L shelf, void *data){
  printf("Hylla: %s, Antal: %d\n", ((shelf_t *)shelf)->shelf_name, ((shelf_t *)shelf)->amount);
}

void edit_item_shelf(L shelf, void *data){
  if(strcmp(((shelf_t *)shelf)->shelf_name, (char *)data) == 0){
    ((shelf_t *)shelf)->amount = ask_question_int("Hyllan finns redan. Nytt antal:");
    strcpy(data, "change made");
  }
}

void shelf_exist_in_list(L shelf, void *data){
  if(strcmp(((shelf_t *)shelf)->shelf_name, (char *)data) == 0){
    strcpy(data, "change made");
    printf("Det ligger redan en annan vara på hyllan\n");
  }
}

void copy_shelves(L shelf, void *data){
  shelf_t *new_shelf = calloc(1, sizeof(shelf_t));
  *new_shelf = *(shelf_t *)shelf;
  list_append((list_t *)data, new_shelf);
}
//////////////////////////////////////////////////////////////////////////////////////////////////

// used by tree_apply
void tree_item_has_shelf(K name, T item, void *data){
  list_apply(((item_t *)item)->storage, shelf_exist_in_list, data);  
}


bool tree_has_shelf(tree_t *items, char *shelf_name){
  tree_apply(items, 0, tree_item_has_shelf, shelf_name);
  if(strcmp(shelf_name, "change made") == 0){
    return true;
  }
  return false;
}


bool edit_if_item_has_shelf(list_t *storage, char *data){
  list_apply(storage, edit_item_shelf, data);  
  if(strcmp(data, "change made") == 0){
    return true;
  }
  return false;
}


void print_item(item_t *item){
  printf("\nNamn:   %s\n", item->name);
  printf("Beskrivning:   %s\n", item->desc);
  printf("Pris:  %d kr\n", item->price);
  list_apply(item->storage, print_storage, NULL);
}


bool edit_shelf(tree_t *items, item_t *item){
  char *shelf_name = ask_question_shelf("Namn på lagerhyllan:");
  char *shelf_name_backup = calloc(1, sizeof(shelf_name));
  strcpy(shelf_name_backup, shelf_name);
  if(edit_if_item_has_shelf(item->storage, shelf_name)){
    return true;
  }
  else if(tree_has_shelf(items, shelf_name)){
    edit_shelf(items, item);
  }
  else{
    int amount = ask_question_int("Hur många varor ska finnas på hyllan:");  
    shelf_t *shelf = shelf_new();
    shelf->shelf_name = shelf_name;
    shelf->amount = amount;
    list_append(item->storage, shelf);
  }
  return true;
}


item_t *input_item(tree_t *items){
  item_t *item = item_new();
  list_t *storage = list_new();
  shelf_t *new_shelf = shelf_new();
  
  item->desc = ask_question_string("Beskrivning:");
  item->price = ask_question_int("Pris:");
  new_shelf->shelf_name = ask_question_shelf("Lagerhylla:");
  
  list_append(storage, new_shelf);
  item->storage = storage;

  while(tree_has_shelf(items, new_shelf->shelf_name)){
    new_shelf->shelf_name = ask_question_shelf("Lagerhylla:");
  }
  new_shelf->amount = ask_question_int("Antal:");
  return item;
}


void add_item_to_db(tree_t *items){
  char *name = ask_question_string("Namn på varan:");
  if(tree_has_key(items, name)){
    printf("Varan finns redan, Använder samma pris och beskrivning.\n");
    item_t *item = tree_get(items, name);
    edit_shelf(items, item);
    print_item(item);
    return;
  }
  item_t *new_item = input_item(items);
  new_item->name = name;
  print_item(new_item);
  char answer;
  while(true){
    answer = ask_question_want_to_add_item();
    if(answer == 'J'){
      tree_insert(items, name, new_item);
      return;
    }
    if(answer == 'N'){
      return;
    }
    if(answer == 'R'){
      add_item_to_db(items);
      return;
    }
  }
}

//////////////////////////////// functions used by list_db /////////////////////////////////
void list_20_items(tree_t *items, int *page_to_view){
  int item_amount = tree_size(items);
  char **item_names = tree_keys(items);
  int view_from_item = *page_to_view * 20;
  int cursor = view_from_item;
  for(; cursor < (view_from_item + 20) && cursor < item_amount; cursor++){
    printf("%d. %s\n", cursor+1, item_names[cursor]);
  }
}


void view_item(T *item_elems){
  int item_to_view = ask_question_int("Välj en siffra för att visa en vara");
  print_item(item_elems[item_to_view - 1]);
}


int edit_item(tree_t *items){
  int edit_item_num = 0;
  while(edit_item_num > tree_size(items) || edit_item_num < 1){
    edit_item_num = ask_question_int("\nVilken vara vill du ändra?");
  }
  return edit_item_num;
}
////////////////////////////////////////////////////////////////////////////////////////////

// input view_or_edit: 0 if used to view. 1 if used to edit
int list_db(tree_t *items, int view_or_edit){
  int item_amount = tree_size(items);
  int page_to_view = 0;
  if(item_amount < 1){
    printf("No items\n");
    return 0;
  }
  T *item_elems = tree_elements(items);
  list_20_items(items, &page_to_view);
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
      view_from_item = page_to_view * 20;
      if(view_from_item > item_amount){
        page_to_view -= 1;
        list_20_items(items, &page_to_view);
        printf("Du är redan på sista sidan!\n");
      }
      else{
        list_20_items(items, &page_to_view);
      }
    }
    if(answer == 'T'){
      page_to_view -= 1;
      if(page_to_view < 0){
        page_to_view = 0;
        list_20_items(items, &page_to_view);
        printf("Du är redan på första sidan!\n");
      }
      else{
        list_20_items(items, &page_to_view);
      }
    }
    if(answer == 'V'){
      if(view_or_edit == 0){
        view_item(item_elems);
        return 0;
      }
      else{
        return edit_item(items);
      }
    }
    if(answer == 'A'){
      return 0;
    }
  }
}


int edit_db(tree_t *items, action_t *undo, item_t *copy_p){
  int edit_item_num = list_db(items, 1);
  if(edit_item_num == 0){
    return 0;
  }
  T *all_items = tree_elements(items);
  item_t *item_to_edit = all_items[edit_item_num - 1];
  char answer;
  print_item(item_to_edit);
  
  while(true){
    answer = ask_question_edit_item();
    if(answer == 'B'){
      printf("Nuvarande beskrivning: %s\n---------------------------\nNy beskrivning:", item_to_edit->desc);
      *copy_p = *item_to_edit;
      item_to_edit->desc = ask_question_string("");
    }
    else if(answer == 'P'){
      printf("Nuvarande pris: %d kr\n----------------------\nNytt pris:", item_to_edit->price);
      *copy_p = *item_to_edit;
      item_to_edit->price = ask_question_int("");
    }
    else if(answer == 'L'){
      list_apply(item_to_edit->storage, print_storage, NULL);
      undo->copy.name = item_to_edit->name;
      undo->copy.desc = item_to_edit->desc;
      undo->copy.price = item_to_edit->price;
      list_t *copy_storage = list_new();
      list_apply(item_to_edit->storage, copy_shelves, copy_storage);
      undo->copy.storage = copy_storage;
      edit_shelf(items, item_to_edit);
    }
    else if(answer == 'A'){
      return 0;
    }
    undo->merch = item_to_edit;
    return 1;
  }  
}


void regret_action(item_t *copy_pointer, action_t *undo){
  if (undo->type == 0){
    printf("Det finns inget att ångra!\n");
  }
  //undo add
  else if (undo->type == 1){
    printf("Not yet implemented!\n");       
    //        undo->merch = NULL;  
    undo->type = 0;
  }
  else if (undo->type == 2){
    printf("Not yet implemented!\n");
  }
  //undo edit
  else if (undo->type == 3){
    if(undo->copy.price != undo->merch->price || strcmp(undo->copy.desc, undo->merch->desc) != 0){
      undo->copy = *copy_pointer;
      *undo->merch = undo->copy;     
      undo->type = 0;
    }
    else{
      *undo->merch = undo->copy;
    }
  }
}



int event_loop(tree_t *items){
  char answer;

  item_t *copy_pointer = calloc(1, sizeof(item_t));
  action_t *undo = calloc(1, sizeof(action_t));

  printf("\nVälkommen till lagerhantering 1.0\n=================================\n");
  while(true){
    answer = ask_question_menu();
    
    if(answer == 'L'){
      add_item_to_db(items);
      undo->type = 1;
    }
    else if(answer == 'T'){
      printf("Not yet implemented!\n");
      undo->type = 2;
    }
    else if(answer == 'R'){
      edit_db(items, undo, copy_pointer);
      undo->type = 3;
    }
    else if(answer == 'G'){
      regret_action(copy_pointer, undo);
    }
    else if(answer == 'H'){
      list_db(items, 0);
    }
    else if(answer == 'A'){
      return 0;
    }  
  }
}


int main(void){
  tree_t *items = tree_new();
  
  item_t *item1 = item_new();   list_t *storage1 = list_new();   shelf_t *shelf1 = shelf_new();
  item_t *item2 = item_new();   list_t *storage2 = list_new();   shelf_t *shelf2 = shelf_new();
  item_t *item3 = item_new();   list_t *storage3 = list_new();   shelf_t *shelf3 = shelf_new();
  item_t *item4 = item_new();   list_t *storage4 = list_new();   shelf_t *shelf4 = shelf_new();
  item_t *item5 = item_new();   list_t *storage5 = list_new();   shelf_t *shelf5 = shelf_new();
  item_t *item6 = item_new();   list_t *storage6 = list_new();   shelf_t *shelf6 = shelf_new();
  item_t *item7 = item_new();   list_t *storage7 = list_new();   shelf_t *shelf7 = shelf_new();
  item_t *item8 = item_new();   list_t *storage8 = list_new();   shelf_t *shelf8 = shelf_new();
  item_t *item9 = item_new();   list_t *storage9 = list_new();   shelf_t *shelf9 = shelf_new();
  item_t *item10 = item_new();  list_t *storage10 = list_new();  shelf_t *shelf10 = shelf_new();
  item_t *item11 = item_new();  list_t *storage11 = list_new();  shelf_t *shelf11 = shelf_new();
  item_t *item12 = item_new();  list_t *storage12 = list_new();  shelf_t *shelf12 = shelf_new();
  item_t *item13 = item_new();  list_t *storage13 = list_new();  shelf_t *shelf13 = shelf_new();
  item_t *item14 = item_new();  list_t *storage14 = list_new();  shelf_t *shelf14 = shelf_new();
  item_t *item15 = item_new();  list_t *storage15 = list_new();  shelf_t *shelf15 = shelf_new();
  item_t *item16 = item_new();  list_t *storage16 = list_new();  shelf_t *shelf16 = shelf_new();
  item_t *item17 = item_new();  list_t *storage17 = list_new();  shelf_t *shelf17 = shelf_new();
  item_t *item18 = item_new();  list_t *storage18 = list_new();  shelf_t *shelf18 = shelf_new();
  item_t *item19 = item_new();  list_t *storage19 = list_new();  shelf_t *shelf19 = shelf_new();
  item_t *item20 = item_new();  list_t *storage20 = list_new();  shelf_t *shelf20 = shelf_new();
  item_t *item21 = item_new();  list_t *storage21 = list_new();  shelf_t *shelf21 = shelf_new();
  item_t *item22 = item_new();  list_t *storage22 = list_new();  shelf_t *shelf22 = shelf_new();
  item_t *item23 = item_new();  list_t *storage23 = list_new();  shelf_t *shelf23 = shelf_new();
  item_t *item24 = item_new();  list_t *storage24 = list_new();  shelf_t *shelf24 = shelf_new();

  item1->name = "vara a";    item1->desc = "detta är en beskrivning";   item1->price = 50;
  item2->name = "vara b";    item2->desc = "detta är en beskrivning";   item2->price = 50;
  item3->name = "vara c";    item3->desc = "detta är en beskrivning";   item3->price = 50;
  item4->name = "vara d";    item4->desc = "detta är en beskrivning";   item4->price = 50;
  item5->name = "vara e";    item5->desc = "detta är en beskrivning";   item5->price = 50;
  item6->name = "vara f";    item6->desc = "detta är en beskrivning";   item6->price = 50;
  item7->name = "vara g";    item7->desc = "detta är en beskrivning";   item7->price = 50;
  item8->name = "vara h";    item8->desc = "detta är en beskrivning";   item8->price = 50;
  item9->name = "vara i";    item9->desc = "detta är en beskrivning";   item9->price = 50;
  item10->name = "vara j";  item10->desc = "detta är en beskrivning";  item10->price = 50;
  item11->name = "vara k";  item11->desc = "detta är en beskrivning";  item11->price = 50;
  item12->name = "vara l";  item12->desc = "detta är en beskrivning";  item12->price = 50;
  item13->name = "vara m";  item13->desc = "detta är en beskrivning";  item13->price = 50;
  item14->name = "vara n";  item14->desc = "detta är en beskrivning";  item14->price = 50;
  item15->name = "vara o";  item15->desc = "detta är en beskrivning";  item15->price = 50;
  item16->name = "vara p";  item16->desc = "detta är en beskrivning";  item16->price = 50;
  item17->name = "vara q";  item17->desc = "detta är en beskrivning";  item17->price = 50;
  item18->name = "vara r";  item18->desc = "detta är en beskrivning";  item18->price = 50;
  item19->name = "vara s";  item19->desc = "detta är en beskrivning";  item19->price = 50;
  item20->name = "vara t";  item20->desc = "detta är en beskrivning";  item20->price = 50;
  item21->name = "vara u";  item21->desc = "detta är en beskrivning";  item21->price = 50;
  item22->name = "vara v";  item22->desc = "detta är en beskrivning";  item22->price = 50;
  item23->name = "vara w";  item23->desc = "detta är en beskrivning";  item23->price = 50;
  item24->name = "vara x";  item24->desc = "detta är en beskrivning";  item24->price = 50;

  shelf1->shelf_name = "A1";    shelf1->amount = 6;
  shelf2->shelf_name = "A2";    shelf2->amount = 6;
  shelf3->shelf_name = "A3";    shelf3->amount = 6;
  shelf4->shelf_name = "A4";    shelf4->amount = 6;
  shelf5->shelf_name = "A5";    shelf5->amount = 6;
  shelf6->shelf_name = "A6";    shelf6->amount = 6;
  shelf7->shelf_name = "A7";    shelf7->amount = 6;
  shelf8->shelf_name = "A8";    shelf8->amount = 6;
  shelf9->shelf_name = "A9";    shelf9->amount = 6;
  shelf10->shelf_name = "A10";  shelf10->amount = 6;
  shelf11->shelf_name = "A11";  shelf11->amount = 6;
  shelf12->shelf_name = "A12";  shelf12->amount = 6;
  shelf13->shelf_name = "A13";  shelf13->amount = 6;
  shelf14->shelf_name = "A14";  shelf14->amount = 6;
  shelf15->shelf_name = "A15";  shelf15->amount = 6;
  shelf16->shelf_name = "A16";  shelf16->amount = 6;
  shelf17->shelf_name = "A17";  shelf17->amount = 6;
  shelf18->shelf_name = "A18";  shelf18->amount = 6;
  shelf19->shelf_name = "A19";  shelf19->amount = 6;
  shelf20->shelf_name = "A20";  shelf20->amount = 6;
  shelf21->shelf_name = "A21";  shelf21->amount = 6;
  shelf22->shelf_name = "A22";  shelf22->amount = 6;
  shelf23->shelf_name = "A23";  shelf23->amount = 6;
  shelf24->shelf_name = "A24";  shelf24->amount = 6;

  list_append(storage1, shelf1);    item1->storage = storage1;    tree_insert(items, item1->name, item1);
  list_append(storage2, shelf2);    item2->storage = storage2;    tree_insert(items, item2->name, item2);
  list_append(storage3, shelf3);    item3->storage = storage3;    tree_insert(items, item3->name, item3);
  list_append(storage4, shelf4);    item4->storage = storage4;    tree_insert(items, item4->name, item4);
  list_append(storage5, shelf5);    item5->storage = storage5;    tree_insert(items, item5->name, item5);
  list_append(storage6, shelf6);    item6->storage = storage6;    tree_insert(items, item6->name, item6);
  list_append(storage7, shelf7);    item7->storage = storage7;    tree_insert(items, item7->name, item7);
  list_append(storage8, shelf8);    item8->storage = storage8;    tree_insert(items, item8->name, item8);
  list_append(storage9, shelf9);    item9->storage = storage9;    tree_insert(items, item9->name, item9);
  list_append(storage10, shelf10);  item10->storage = storage10;  tree_insert(items, item10->name, item10);
  list_append(storage11, shelf11);  item11->storage = storage11;  tree_insert(items, item11->name, item11);
  list_append(storage12, shelf12);  item12->storage = storage12;  tree_insert(items, item12->name, item12);
  list_append(storage13, shelf13);  item13->storage = storage13;  tree_insert(items, item13->name, item13);
  list_append(storage14, shelf14);  item14->storage = storage14;  tree_insert(items, item14->name, item14);
  list_append(storage15, shelf15);  item15->storage = storage15;  tree_insert(items, item15->name, item15);
  list_append(storage16, shelf16);  item16->storage = storage16;  tree_insert(items, item16->name, item16);
  list_append(storage17, shelf17);  item17->storage = storage17;  tree_insert(items, item17->name, item17);
  list_append(storage18, shelf18);  item18->storage = storage18;  tree_insert(items, item18->name, item18);
  list_append(storage19, shelf19);  item19->storage = storage19;  tree_insert(items, item19->name, item19);
  list_append(storage20, shelf20);  item20->storage = storage20;  tree_insert(items, item20->name, item20);
  list_append(storage21, shelf21);  item21->storage = storage21;  tree_insert(items, item21->name, item21);
  list_append(storage22, shelf22);  item22->storage = storage22;  tree_insert(items, item22->name, item22);
  list_append(storage23, shelf23);  item23->storage = storage23;  tree_insert(items, item23->name, item23);
  list_append(storage24, shelf24);  item24->storage = storage24;  tree_insert(items, item24->name, item24);

  return event_loop(items);
}
