#ifndef Tokenizer_H
#define Tokenizer_H

#include "Common.h"
#include "Token.h"

typedef struct {
  char *str;
  int index;
  int callBackTokenFlag;
  int numOfQuote;
  Token *currentToken;
} Tokenizer;

Tokenizer *initTokenizer(char *stringToTokenize);
Token *getToken(Tokenizer *tokenizer);

void freeString(void *str);
int checkFor0x(char *str);
int checkForHexDigit(char *str);
int checkforInt(char *str);
int checkForOctal(char *str);

#endif // Tokenizer_H
