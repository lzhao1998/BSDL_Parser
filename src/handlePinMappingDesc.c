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

/*
void handlePinMapping(blabalal, LinkedList *pinMapping){
  //blablbalba
 listAddPinMapping(pinMapping,token->str);
 //blablabla
 handleMapString(fileTokenizer,((pinMappingName*)pinMapping->head->data)->mapString);
 //port name do looping and calling the handlePortMap to insert portname
 //blabalbal
 //end
}*/


/*
void listAddPortName(LinkedList *mapString, char* name){
  portName *portname;
  portname = initPortName();

  portname->portNaming = malloc(sizeof(char));
  strcpy(portname->portNaming, name);

  Item *item;
  item = initItem(portname);
  listAdd(mapString,item);
}

portName *initPortName(){
  portName *portname = malloc(sizeof(portName));
  portname->portNaming = "";
  portname->pindesc = listInit();
  return portname;
}*/


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

    //listaddportmap need to be implement
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
