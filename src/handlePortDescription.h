#ifndef _HANDLEPORTDESCRIPTION_H
#define _HANDLEPORTDESCRIPTION_H

# include "Tokenizer.h"
# include "read_file.h"


void handlePortDesc(FileTokenizer *fileTokenizer,LinkedList *port);
void handlePinSpec(FileTokenizer *fileTokenizer, LinkedList *port);
int getTypeNo(Token *token, int errorCode, char *strArr[]);
int *getRange(FileTokenizer *fileTokenizer);
void listAddPortDesc(LinkedList *port, char *portName,int pinType,int bitType,int int1, int int2, int rangeType);
void printPortDesc(LinkedList *list);
portDesc *initPortDesc();

#endif // _HANDLEPORTDESCRIPTION_H
