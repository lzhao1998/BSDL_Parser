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

char *getStringSymbol[] = {
  "-",    // 0 DASH
  ",",    // 1 COMMA
  "&",    // 2 AND
  ";",    // 3 SEMICOLON
  NULL
};

// treat " and & invisible
// " hello  world " & " hi bye"  ==> hello world  hi bye
Token *getStringToken(FileTokenizer *fileTokenizer){
  int i = 0, startColumn = 0;
  char errmsg[100];
  Token *token = NULL;

  while(1){
    //throw error when it is end of file
    if(fileTokenizer->tokenizer->str == NULL){
      token = getTokenFromFile(fileTokenizer);
      sprintf(errmsg,"Error on line: %d .Expect not NULL but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_INVALID_STRING_TYPE,token,errmsg);
    }

    i = fileTokenizer->tokenizer->index;
    while(fileTokenizer->tokenizer->str[i] == ' ' || fileTokenizer->tokenizer->str[i] == '\t'){
       i++;
    }

    //when reach the 2nd quote symbol, check for the end of string or continue string
    if(fileTokenizer->tokenizer->numOfQuote == 2){
      if(fileTokenizer->tokenizer->str[i] == '\"'){ //if there is triple quote
        char *symbol = malloc(sizeof(char) * 2);
        symbol[0] = fileTokenizer->tokenizer->str[i];
        symbol[1] = '\0';
        token = createOperatorToken(i,1,fileTokenizer->tokenizer->str,symbol);
        sprintf(errmsg,"Error on line: %d. Expect '&' and ';' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token), token->str);
        throwException(ERR_INVALID_STRING_TYPE,token,errmsg);
      }
      token = getTokenFromFile(fileTokenizer);
      // if it is not the operator token, throw error
      if(token->type != TOKEN_OPERATOR_TYPE){
        sprintf(errmsg,"Error on line: %d. Expect '&' and ';' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token), token->str);
        throwException(ERR_INVALID_STRING_TYPE,token,errmsg);
      }else{
        // if is '&' symbol, free the token and get next token
        if(strcmp(getStringSymbol[2],token->str) == 0){
          fileTokenizer->tokenizer->numOfQuote = 0;
          freeToken(token);
          continue;
        }else{ // else return the token
          return token;
        }
      }
    }



    // when number of quote is 0 ,throw error
    if(fileTokenizer->tokenizer->str[i] != '\"' && fileTokenizer->tokenizer->numOfQuote == 0){
      token = getTokenFromFile(fileTokenizer);
       if(token->type == TOKEN_NULL_TYPE){
         freeToken(token);
         continue;
       }else{
         sprintf(errmsg,"Error on line: %d .Expect \" symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
         throwException(ERR_INVALID_STRING_TYPE,token,errmsg);
       }
    }else if(fileTokenizer->tokenizer->str[i] == '\"'){ //if it is quote then index = i+1 and numOfQuote + 1
      fileTokenizer->tokenizer->index = i + 1;
      fileTokenizer->tokenizer->numOfQuote += 1;
      continue;
    }else{ //return token
      token = getTokenFromFile(fileTokenizer);
      if(token->type == TOKEN_NULL_TYPE && fileTokenizer->tokenizer->numOfQuote < 2){
        sprintf(errmsg,"Error on line: %d. Expect \" symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        throwException(ERR_INVALID_STRING_TYPE,token,errmsg);
      }else{
        return token;
      }
    }
  }

}

void freeStr(char *str){
  free(str);
}
