#ifndef _HANDLEPINMAPPINGDESC_H
#define _HANDLEPINMAPPINGDESC_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"
# include "read_file.h"

typedef struct{
  char *pinMapName;
  LinkedList *mapString;
} pinMappingName;

typedef struct{
  char *portNaming;
  LinkedList *pindesc;
} portName;

typedef struct{
  char *pinDesc;
} pinDescription;

LinkedList *handlePinDescOrList(FileTokenizer *fileTokenizer);
void listAddPortDescOrList(LinkedList *list, char *str);

#endif // _HANDLEPINMAPPING_H
