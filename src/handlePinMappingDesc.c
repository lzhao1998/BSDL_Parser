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

void handlePortMap(FileTokenizer *fileTokenizer, LinkedList *mapString){ //blablabla
  Token *token;
  char errmsg[100];
  token = getStringToken(fileTokenizer);

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    //need change the throw value
    throwException(ERR_INVALID_TYPE,token,"Expect port name but is not!!");
  }

  /****CHECK1: CHECK FROM THE PORT DESCRIPTION IS SIMILAR OR NOT**/
  /****CHECK2: CHECK FOR THAT PORT NAME IS NOT DECLARE YET ***/

  //list add to port Name
  listAddPortName(mapString,token->str);

  // check for column ':'
  freeToken(token);
  token = getStringToken(fileTokenizer);
  if(token->type != TOKEN_OPERATOR_TYPE){
    //need change throw msg and value
    throwException(ERR_INVALID_TYPE,token,"Expect : but is not!!");
  }

  if(strcmp(symbolC[3],token->str) != 0){
    sprintf(errmsg,"Expect : but is %s",token->str);
    //need change the err value
    throwException(ERR_INVALID_TYPE,token,errmsg);
  }

  freeToken(token);

}

void handleDescOrList(FileTokenizer *fileTokenizer, LinkedList *pindesc){
  Token *token;
  char errmsg[100];
  //blabalbal
}

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
}



LinkedList *handlePinDescOrList(FileTokenizer *fileTokenizer){
  Token *token;
  char errmsg[100];
  LinkedList *list;
  int checkListNum = 0;
  list = listInit();
  token = getStringToken(fileTokenizer);

  //throw error when it is not integer,operator and identifier type
  if(token->type != TOKEN_OPERATOR_TYPE && token->type != TOKEN_IDENTIFIER_TYPE && \
     token->type != TOKEN_INTEGER_TYPE){
     //need change throw msg and value
    throwException(ERR_INVALID_TYPE,token,"Expect : but is not!!");
  }

  if(token->type == TOKEN_IDENTIFIER_TYPE || token->type == TOKEN_INTEGER_TYPE){
    listAddPortDescOrList(list, token->str);
    freeToken(token);
  }else{
    if(strcmp(symbolC[0],token->str)!= 0){ // if is a list, '(' symbol appear
      sprintf(errmsg,"Expect ( but is %s",token->str);
      //need change the err value
      throwException(ERR_INVALID_TYPE,token,errmsg);
    }
    freeToken(token);
    token = getStringToken(fileTokenizer);

    while(1){
      if(token != TOKEN_IDENTIFIER_TYPE){ //if is not identifier type
        //sprintf(errmsg,"Expect pin description but is but is %s",token->str);
        //need change the err value
        throwException(ERR_INVALID_TYPE,token,errmsg);
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
          freeToken(token);
          //need to check list num?
          break;
        }else{  // else throw error
          sprintf(errmsg,"Expect ',' or ')' symbol but is %s",token->str);
          //need change the err value
          throwException(ERR_INVALID_TYPE,token,errmsg);
        }
      }else{
        //sprintf(errmsg,"Expect pin description but is but is %s",token->str);
        //need change the err value
        throwException(ERR_INVALID_TYPE,token,errmsg);
      }
    }
  }

  return list;
}

void listAddPortDescOrList(LinkedList *list, char *str){
  pinDescription *pinD;
  pinD->pinDesc = (char*)malloc(sizeof(char) * strlen(str));
  strcpy(pinD->pinDesc, str);

  Item *item;
  item = initItem(pinD);
  listAdd(list,item);
}
