#ifndef _GETSTRTOKEN_H
#define _GETSTRTOKEN_H

# include "Tokenizer.h"

//char *getStringFromFileTokenizer(FileTokenizer *fileTokenizer);
Token *getStringFromFileTokenizer(FileTokenizer *fileTokenizer);
void freeStr(char *str);

#endif // _GETSTRTOKEN_H
