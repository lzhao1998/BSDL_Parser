#ifndef _HANDLEBOUNDARYREGISTERDESC_H
#define _HANDLEBOUNDARYREGISTERDESC_H

# include <stdio.h>
# include "Tokenizer.h"
# include "read_file.h"
# include "linkedList.h"

void handleBoundaryRegister(FileTokenizer *fileTokenizer, LinkedList *list);
void listAddBoundaryRegister(LinkedList *list,char* cellNum,char*cellname,char*portid,char*function,char*safebit,char*ccell,char*disV,char*disR);
boundaryRegister *initBR();
int checkFunctionName(char *str);
int checkInputSpec(char *str);
int checkDisableResult(char *str);
char *getPortId(FileTokenizer *fileTokenizer);
char *getSubString(char *oriStr, int startPos, int endPos);

#endif // _HANDLEBOUNDARYREGISTERDESC_H
