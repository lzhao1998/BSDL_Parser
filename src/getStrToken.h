#ifndef _GETSTRTOKEN_H
#define _GETSTRTOKEN_H

# include "Tokenizer.h"

Token *getStringToken(FileTokenizer *fileTokenizer);
void freeStr(void *str);

#endif // _GETSTRTOKEN_H
