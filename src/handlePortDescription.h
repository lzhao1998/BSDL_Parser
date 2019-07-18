#ifndef _HANDLEPORTDESCRIPTION_H
#define _HANDLEPORTDESCRIPTION_H

# include "Tokenizer.h"
# include "read_file.h"



void handlePortDesc(FileTokenizer *fileTokenizer,BSinfo *bsinfo);
void handlePinSpec(FileTokenizer *fileTokenizer);
void initPortDesc(portDesc *portdesc);
int getTypeNo(Token *token, int errorCode, char *strArr[]);
int *getRange(FileTokenizer *fileTokenizer);

#endif // _HANDLEPORTDESCRIPTION_H
