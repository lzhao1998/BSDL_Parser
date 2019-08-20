#ifndef _HANDLEBOUNDARYREGISTERDESC_H
#define _HANDLEBOUNDARYREGISTERDESC_H

# include <stdio.h>
# include "linkedList.h"
# include "Tokenizer.h"
# include "read_file.h"


void printBoundaryRegister(LinkedList *list);
void handleBoundaryRegisterD(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void handleBoundaryRegister(FileTokenizer *fileTokenizer, LinkedList *list);
void listAddBoundaryRegister(LinkedList *list,char* cellNum,char*cellname,char*portid,char*function,char*safebit,char*ccell,char*disV,char*disR);
boundaryRegister *initBR();
int checkFunctionName(char *str);
int checkInputSpec(char *str);
int checkDisableResult(char *str);
char *getPortId(FileTokenizer *fileTokenizer);
char *getSubString(char *oriStr, int startPos, int endPos);
void freeStr(void *str);

#endif // _HANDLEBOUNDARYREGISTERDESC_H
