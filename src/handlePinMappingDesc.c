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
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"

char *symbolC[] = {
  "(",  //0 LEFTPAREN
  ")",  //1 RIGHTPAREN
  ";",  //2 SEMICOLON
  ":",  //3 COLON
  ",",  //4 COMMA
  "-",  //5 DASH
  "\""  //6 QUOTE
};

char *pinMappingFormat[] = {
  "constant",         // 0
  NULL,               // 1
  ":",                // 2
  "PIN_MAP_STRING",   // 3
  ":",                // 4
  "="                 // 5
};

void printPinMapping(char *str, LinkedList *list){
  Item *current1,*current2,*current3;
  pinMappingName *pinMappingN;
  portMap *portM;
  pinDescription *pinD;
  int flag = 0;

  current1 = list->head;
  if(current1 == NULL){
    printf("ERROR!! PIN MAPPING IS EMPTY!!\n" );
    return;
  }
  printf("attribute PIN_MAP of %s: entity is PHYSICAL_PIN_MAP;\n", str);
  while(current1 != NULL){
    pinMappingN = ((pinMappingName*)current1->data);
    printf("constant %s:PIN_MAP_STRING:=\n", pinMappingN->pinMapName);
    current2 = pinMappingN->mapString->head;
    if(current2 == NULL){
      printf("ERROR!! PORT NAME IS EMPTY!!\n" );
      return;
    }

    while(current2 != NULL){
      portM = ((portMap*)current2->data);
      printf("\" %s: ", portM->portName);
      current3 = portM->pindesc->head;
      if(current3 == NULL){
        printf("ERROR!! PINDESC IS EMPTY!!\n" );
        return;
      }

      if(current3->next != NULL){
        printf("(");
        flag = 1;
      }

      while(current3 != NULL){
        pinD = ((pinDescription*)current3->data);
        printf("%s", pinD->pinDesc);

        current3 = current3->next;
        if(current3 == NULL && flag == 1){
          flag = 0;
          printf(")");
        }else if(current3 != NULL){
          printf(",");
        }
      }

      current2=current2->next;
      if(current2 == NULL){
        printf("\";\n");
      }else{
        printf(",\"&\n");
      }
    }

    current1=current1->next;
  }

  printf("\n" );
}

// input : bsinfo, fileTokenizer
void handlePinMappingStatementDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  char errmsg[100];
  Token *token;
  int i = 0;
  char *arr[7] = {"of",bsinfo->modelName,":","entity","is","PHYSICAL_PIN_MAP",";"};
  int tokenType[7] = {8,8,4,8,8,8,4};

  //compare all the string, throw error if fail
  while(i < 7){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(strcmp(arr[i],token->str) != 0){   //string compare...if not same, throw error
        sprintf(errmsg,"Error on line: %d. Expect %s but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),arr[i],token->str);
        throwException(ERR_INVALID_PIN_MAP_STATEMENT,token,errmsg);
      }
    }else{
      sprintf(errmsg,"Error on line: %d. Expect %s but is not",getCorrectReadLineNo(fileTokenizer->readLineNo,token),arr[i]);
      throwException(ERR_INVALID_PIN_MAP_STATEMENT,token,errmsg);
    }
    i++;
    freeToken(token);
  }

  //check for NULL and comment line, if it is not then throw error
  token = getTokenFromFile(fileTokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
    sprintf(errmsg,"Error on line: %d. Expect Null or CommentLine but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_INVALID_PIN_MAP_STATEMENT,token,errmsg);
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(symbolC[5],token->str) == 0){
      checkAndSkipCommentLine(fileTokenizer);
    }else{
      sprintf(errmsg,"Error on line: %d. Invalid comment line!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      throwException(ERR_INVALID_PIN_MAP_STATEMENT,token,errmsg);
    }
  }
  freeToken(token);
  handlePinMapping(fileTokenizer,bsinfo->pinMapping);
}


// FORMAT: constant <PIN MAPPING NAME>:PIN_MAP_STRING:=
void handlePinMapping(FileTokenizer *fileTokenizer, LinkedList *pinMapping){
  Token *token;
  char errmsg[100];
  char *temp;

  token = getTokenFromFile(fileTokenizer);
  while(1){
    //if is null token or comment line, skip it
    while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
      if(token->type == TOKEN_NULL_TYPE){
        freeToken(token);
        token = getTokenFromFile(fileTokenizer);
        continue;
      }else if(strcmp(symbolC[5],token->str) == 0){
          checkAndSkipCommentLine(fileTokenizer);
          freeToken(token);
          token = getTokenFromFile(fileTokenizer);
          continue;
      }else{
        sprintf(errmsg,"Error on line: %d. Expect '-' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    // check for 'constant'
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect constant but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      if(strcmp(pinMappingFormat[0],token->str) != 0){
        sprintf(errmsg,"Error on line: %d. Expect constant but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    //get pin mapping name
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect Pin Mapping Name but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      temp = malloc(sizeof(char) * strlen(token->str));
      strcpy(temp,token->str);
    }

    //check ':' symbol
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      if(strcmp(pinMappingFormat[2],token->str) != 0){
        sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    //check for PIN_MAP_STRING
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect PIN_MAP_STRING but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      if(strcmp(pinMappingFormat[3],token->str) != 0){
        sprintf(errmsg,"Error on line: %d. Expect PIN_MAP_STRING but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    //check for ':'
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      if(strcmp(pinMappingFormat[4],token->str) != 0){
        sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    //check for '='
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect '=' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else{
      if(strcmp(pinMappingFormat[5],token->str) != 0){
        sprintf(errmsg,"Error on line: %d. Expect '=' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);

    if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect null or comment line but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
    }else if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(symbolC[5],token->str) == 0){
          checkAndSkipCommentLine(fileTokenizer);
      }else{
        sprintf(errmsg,"Error on line: %d. Expect '-' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    //listadd pin mapping
    listAddPinMapping(pinMapping, temp, handlePortMap(fileTokenizer));

    //search for next constant....
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);

    while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
      if(token->type == TOKEN_NULL_TYPE){
        freeToken(token);
        token = getTokenFromFile(fileTokenizer);
        continue;
      }else if(strcmp(symbolC[5],token->str) == 0){
          checkAndSkipCommentLine(fileTokenizer);
          freeToken(token);
          token = getTokenFromFile(fileTokenizer);
          continue;
      }else{
        sprintf(errmsg,"Error on line: %d. Expect '-' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PINMAPPING_FORMAT,token,errmsg);
      }
    }

    // if is identifier type and token->str = 'constant', continue doing..
    // else, createCallBackToken and quit the function
    if(token->type == TOKEN_IDENTIFIER_TYPE){
      if(strcmp(pinMappingFormat[0],token->str)==0){
        continue;
      }else{
        createCallBackToken(fileTokenizer->tokenizer,token);
        freeToken(token);
        break;
      }
    }else{
      createCallBackToken(fileTokenizer->tokenizer,token);
      freeToken(token);
      break;
    }

  }
}

void listAddPinMapping(LinkedList *parentList, char *str, LinkedList *childList){
  pinMappingName *portM = malloc(sizeof(pinMappingName));
  portM->pinMapName = malloc(sizeof(char) * strlen(str));
  strcpy(portM->pinMapName, str);
  portM->mapString = malloc(sizeof(LinkedList));
  portM->mapString = childList;

  Item *item;
  item = initItem(portM);
  listAdd(parentList,item);
}


LinkedList *handlePortMap(FileTokenizer *fileTokenizer){
  Token *token;
  char *tempStr;
  char errmsg[100];
  LinkedList *list;

  list = listInit();

  while(1){
    // obtain the portname
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect port name but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }else{
      //need check from port name from the port....
      if(checkVHDLidentifier(token->str) == 0){
        sprintf(errmsg,"Error on line: %d. %s is not a valid VHDL identifier",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
        throwException(ERR_INVALID_PORTNAME,token,errmsg);
      }
      tempStr = malloc(sizeof(char) * strlen(token->str));
      strcpy(tempStr,token->str);
    }

    freeToken(token);
    token = getStringToken(fileTokenizer);

    // Check for ':' symbol
    // If match, freeToken and inset the list. Else, throw error.
    if(token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }else if(strcmp(symbolC[3],token->str) != 0){
      sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }else{
      freeToken(token);
    }

    //add into list
    listAddPortMap(list, tempStr, handlePinDescOrList(fileTokenizer));

    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      sprintf(errmsg,"Error on line: %d. Expect ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }else if(strcmp(symbolC[4],token->str) == 0){
      freeToken(token);
      continue;
    }else if(strcmp(symbolC[2],token->str) == 0){
      freeToken(token);
      break;
    }else{
      sprintf(errmsg,"Error on line: %d. Expect ',' or ';' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token) ,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }
  }

  return list;
}

void listAddPortMap(LinkedList *parentList, char *str, LinkedList *childList){
  portMap *portM = malloc(sizeof(portMap));
  portM->portName = malloc(sizeof(char) * strlen(str));
  strcpy(portM->portName, str);
  portM->pindesc = malloc(sizeof(LinkedList));
  portM->pindesc = childList;

  Item *item;
  item = initItem(portM);
  listAdd(parentList,item);
}


// get pin desc and list
// pin desc =>  pinDesc
// pin list => (pinDesc,pinDesc,....)
LinkedList *handlePinDescOrList(FileTokenizer *fileTokenizer){
  Token *token;
  int lineNo = 0;
  char errmsg[100];
  LinkedList *list;
  int checkListNum = 0;
  list = listInit();
  token = getStringToken(fileTokenizer);

  //throw error when it is not integer,operator and identifier type
  if(token->type != TOKEN_OPERATOR_TYPE && token->type != TOKEN_IDENTIFIER_TYPE && \
     token->type != TOKEN_INTEGER_TYPE){
    lineNo = getCorrectReadLineNo(fileTokenizer->readLineNo,token);
    sprintf(errmsg,"Error on line: %d. Expect pin description or '(' but is %s",lineNo ,token->str);
    throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
  }

  if(token->type == TOKEN_IDENTIFIER_TYPE || token->type == TOKEN_INTEGER_TYPE){
    listAddPortDescOrList(list, token->str);
    freeToken(token);
  }else{
    if(strcmp(symbolC[0],token->str)!= 0){ // if is a list, '(' symbol appear
      lineNo = getCorrectReadLineNo(fileTokenizer->readLineNo,token);
      sprintf(errmsg,"Error on line: %d. Expect ( but is %s",lineNo,token->str);
      throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
    }
    freeToken(token);
    token = getStringToken(fileTokenizer);

    while(1){
      //if is not identifier type and integer type, throw error
      if(token->type != TOKEN_IDENTIFIER_TYPE && token->type != TOKEN_INTEGER_TYPE){
        lineNo = getCorrectReadLineNo(fileTokenizer->readLineNo,token);
        sprintf(errmsg,"Error on line: %d. Expect pin description but is %s",lineNo,token->str);
        throwException(ERR_INVALID_PINDESC,token,errmsg);
      }
      listAddPortDescOrList(list, token->str);
      checkListNum++;

      freeToken(token);
      token = getStringToken(fileTokenizer);
      if(token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(symbolC[4],token->str) == 0){ //if ',' continue loop
          freeToken(token);
          token = getStringToken(fileTokenizer);
          continue;
        }else if(strcmp(symbolC[1],token->str) == 0){ // if ')' , exit loop
          //need to check list num?
          freeToken(token);
          break;
        }else{  // throw error is not ',' and ')'
          lineNo = getCorrectReadLineNo(fileTokenizer->readLineNo,token);
          sprintf(errmsg,"Error on line: %d. Expect ',' or ')' symbol but is %s",lineNo,token->str);
          throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
        }
      }else{  //throw error if it is not operator type
        lineNo = getCorrectReadLineNo(fileTokenizer->readLineNo,token);
        sprintf(errmsg,"Error on line: %d. Expect ',' or ')' symbol but is %s",lineNo, token->str);
        throwException(ERR_INVALID_PINDESC_FORMAT,token,errmsg);
      }
    }
  }

  return list;
}

void listAddPortDescOrList(LinkedList *list, char *str){
  pinDescription *pinD = malloc(sizeof(pinDescription));
  pinD->pinDesc = malloc(sizeof(char));
  strcpy(pinD->pinDesc, str);

  Item *item;
  item = initItem(pinD);
  listAdd(list,item);
}
