#include "linkedList.h"
#include "read_file.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

LinkedList *listInit(){
  LinkedList *list;
  list = (LinkedList*)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;  // Len is how many item in the list

  return list;
}

void listAdd(LinkedList *list, Item *item){
  if(list->len == 0)
  {
    list->head = item;
    list->tail = item;
	  item->next = NULL;
  }
  else
  {
    list->tail->next = item;
    item->next = NULL;
	   list->tail = item;
  }
  list->len += 1;
}

Item *initItem(void *data){
  Item *item;
  item = (Item*)malloc(sizeof(Item));
  item->next = (Item *)-1;
  item->data = (void*)data;
  return item;
}
