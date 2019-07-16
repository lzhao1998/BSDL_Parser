#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H

# include <stdio.h>


typedef struct Item Item;
typedef struct LinkedList LinkedList;


struct Item{
  Item *next;   // Item is pointing the next Item. Eg, Item1 pointing to Item2
  void *data; //it is the pointer that we dont know which is the type of data so we put "void" not "student"
};

struct LinkedList{
  Item *head;
  Item *tail;
  int len;
};

LinkedList *listInit();
void listAdd(LinkedList *list, Item *item);
Item *initItem(void *data);

#endif // _LINKEDLIST_H
