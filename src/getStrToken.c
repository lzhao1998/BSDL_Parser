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
}

Token *getStringFromFileTokenizer(FileTokenizer *fileTokenizer){
  int i = 0, startColumn = 0;
  Token *token = NULL;

  i = fileTokenizer->tokenizer->index;
  while(fileTokenizer->tokenizer->str[i] == ' ' || fileTokenizer->tokenizer->str[i] == '\t'){
    i++;
  }

  if(fileTokenizer->tokenizer->str[i] == '\"'){
    fileTokenizer->tokenizer->index = i;
    startColumn = i;
    char *temp = (char*)malloc((2) * sizeof (char));
    temp[0] = fileTokenizer->tokenizer->str[startColumn];
    temp[1] = '\0';
    // startcolumn,length , tokenizer string, string obtain
    token = createOperatorToken(startColumn, 1, tokenizer->str, temp);
  }else{
    token = getTokenFromFile(fileTokenizer);
  }

  return token;
}

/*
char *getStringFromFileTokenizer(FileTokenizer *fileTokenizer){
  char *orig, *temp;
  Token *token;
  int exitFlag = 0;
  int stringLength = 0;
  int oriLength = 0;
  orig = (char*)malloc(sizeof(char));

  while(exitFlag == 0){
    token = getTokenFromFile(fileTokenizer);

    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(getStringSymbol[0],token->str) == 0){
        checkAndSkipCommentLine(fileTokenizer);
        freeToken(token);
        continue;
      }else{
        throwException(ERR_INVALID_COMMEND_LINE,token,"Invalid comment line!!");
      }
    }else if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_STRING_TYPE){
      throwException(ERR_INVALID_TYPE,NULL,"Invalid type!!");
    }

    if(token->type == TOKEN_NULL_TYPE){
      freeToken(token);
      continue;
    }

    //here is string type token already
    //check for comma in the last of the string
    stringLength = strlen(token->str);
    if(stringLength < 1){
      throwException(ERR_INVALID_STRING,token,"String is empty!!");
    }

    if(token->str[stringLength-1] == ","){
      exitFlag = 0;   //still need continue
    }else{
      exitFlag = 1;   //last line for get string
    }

    oriLength = strlen(orig);
    //orig = realloc(orig, (oriLength + stringLength) * sizeof(char));
    temp = realloc(orig, (oriLength + stringLength) * sizeof(char));

    if (temp == NULL){
      temp = (char*)malloc((oriLength + stringLength) )* sizeof(char));
      strcpy(temp,orig);
      strcat(temp," " + token->str);
      freeStr(orig);
      orig = temp;
      freeStr(temp);
    }else{
      strcpy(temp,orig);
      strcat(temp," " + token->str);
      freeStr(orig);
      orig = temp;
      freeStr(temp);
    }

    strcat(orig,token->str);

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);

    //EXITFLAG = 1
    if(exitFlag == 1){
      if(token->type == TOKEN_OPERATOR_TYPE){ //check for ';'
        if(strcmp(getStringSymbol[3],token->str) == 0){
          freeToken(token);
          token = getTokenFromFile(fileTokenizer);
          if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
            throwException(ERR_INVALID_TYPE,NULL,"Suppose comment line or null only!!");
          }

          //after check ';', check for null token or comment line
          if(token->type == TOKEN_NULL_TYPE){
            freeToken(token);
            continue;
          }else{
            if(strcmp(getStringSymbol[0],token->str) == 0){
              checkAndSkipCommentLine(fileTokenizer);
              continue;
            }else{
              throwException(ERR_INVALID_COMMEND_LINE,token,"Invalid comment line!!");
            }
          }

        }else{
          throwException(ERR_INVALID_COMMEND_LINE,token,"Invalid comment line!!");
        }
      }else{
        throwException(ERR_INVALID_TYPE,NULL,"Suppose comment line or null only!!");
      }
    }

    //EXITFLAG = 0
    if(token->type == TOKEN_OPERATOR_TYPE){ //check for '&'
      if(strcmp(getStringSymbol[2],token->str)==0){
        freeToken(token);
      }else{
        throwException(ERR_INVALID_TYPE,NULL,"Suppose is '&' !!");
      }
    }else{
      throwException(ERR_INVALID_TYPE,NULL,"Suppose is '&' !!");
    }

    //after check '&', check for null or comment line
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(getStringSymbol[0],token->str) == 0){
        checkAndSkipCommentLine(fileTokenizer);
      }else{
        throwException(ERR_INVALID_COMMEND_LINE,token,"Invalid comment line!!");
      }
    }else if(token->type =! NULL){
      throwException(ERR_INVALID_TYPE,NULL,"Suppose comment line or null only!!");
    }

    freeToken(token);
  }

  //after that return the string
  return orig;
}*/



void freeStr(char *str){
  free(str);
}
