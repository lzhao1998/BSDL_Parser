#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "getStrToken.h"
#include "createAndGetTokenFromFile.h"

// "BOOT0      : D6," &
// "JTCK       : A14," &
// "JTDO       : A10," &
// "JTDI       : A13," &
// "JTMS       : A15," &
// "JTRST      : A9," &
// "RESET :A0"

char *getStringSymbol[] = {
  "-",    // 0 DASH
  ",",    // 1 COMMA
  "&",    // 2 AND
  ";",    // 3 SEMICOLON
  NULL
};

// treat " as operator token
Token *getStringToken(FileTokenizer *fileTokenizer){
  int i = 0, startColumn = 0;
  Token *token = NULL;

  if(fileTokenizer->tokenizer->callBackTokenFlag == 1){
    token = fileTokenizer->tokenizer->currentToken;
    fileTokenizer->tokenizer->callBackTokenFlag = 0;
    return token;
  }

  i = fileTokenizer->tokenizer->index;
  while(fileTokenizer->tokenizer->str[i] == ' ' || fileTokenizer->tokenizer->str[i] == '\t'){
    i++;
  }

  if(fileTokenizer->tokenizer->str[i] == '\"'){
    fileTokenizer->tokenizer->index = i+1;
    startColumn = i;
    char *temp = (char*)malloc(2 * sizeof (char));
    temp[0] = fileTokenizer->tokenizer->str[startColumn];
    temp[1] = '\0';

    // startcolumn,length , tokenizer string, string obtain
    token = createOperatorToken(startColumn, 1, fileTokenizer->tokenizer->str, temp);
  }else{
    token = getTokenFromFile(fileTokenizer);
  }

  return token;
}

void freeStr(char *str){
  free(str);
}
