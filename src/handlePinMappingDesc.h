#ifndef _HANDLEPINMAPPINGDESC_H
#define _HANDLEPINMAPPINGDESC_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"
# include "read_file.h"
# include "handleScanPortIdentification.h"

typedef struct{
  char *pinMapName;
  LinkedList *mapString;
} pinMappingName;

typedef struct{
  char *portName;
  LinkedList *pindesc;
} portMap;

typedef struct{
  char *pinDesc;
} pinDescription;

void printPinMapping(char *str, LinkedList *list);

void handlePinMappingStatementDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void handlePinMapping(FileTokenizer *tokenizer, LinkedList *pinMapping);
void listAddPinMapping(LinkedList *parentList, char *str, LinkedList *childList);
LinkedList *handlePortMap(FileTokenizer *fileTokenizer);
void listAddPortMap(LinkedList *parentList, char *str, LinkedList *childList);
LinkedList *handlePinDescOrList(FileTokenizer *fileTokenizer);
void listAddPortDescOrList(LinkedList *list, char *str);


#endif // _HANDLEPINMAPPING_H
