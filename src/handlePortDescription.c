#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "handlePortDescription.h"

char *pinT[] = {
  "in",                 //0
  "out",                //1
  "buffer",             //2
  "linkage",            //3
  "LINKAGE_INOUT",      //4
  "LINKAGE_BUFFER",     //5
  "LINKAGE_IN",         //6
  "LINKAGE_OUT",        //7
  "LINKAGE_MECHANICAL", //8
  "POWER_0",            //9
  "POWER_POS",          //10
  "POWER_NEG",          //11
  "VREF_IN",            //12
  "VREF_OUT"            //13
};

char *bitT[] = {
  "bit",          //0
  "bit_vector"    //1
};

char *rangeType[] = {
  "to",          //0
  "downto"       //1
};

//might be changed to return Linked list
void handlePortDesc(FileTokenizer *fileTokenizer,BSinfo *bsinfo){
  Token *token;
  LinkedList *port;
  token = getTokenFromFile(fileTokenizer);
  port = (LinkedList*)malloc(sizeof(LinkedList))

  //check for '('
  if(token->type == TOKEN_OPERATOR_TYPE){  // check '('
    if(token->str == '('){
      freeToken(token);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,("Expect '(' but is %s",token->str));
    }
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect '(' but is not.");
  }


  handlePinSpec() //->this function will return when reach ';' in of the line
  while (token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == ";"){
      handlePinSpec()
      freeToken;
      token = getTokenFromFile(fileTokenizer);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ';'.");
    }
  }

  //skip the empty line and comment line
  while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_NULL_TYPE){
      skipLine(fileTokenizer);
    }else{
      if(token->str == "-"){
        checkAndSkipCommentLine(fileTokenizer);
      }else{
        break;
      }
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }

  //CHECK FOR )
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == ')'){
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ')'.")
    }
  }
  //CHECK FOR ;
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == ';'){
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ';'.")
    }
  }

  //check end of line and return
  if(token->type == TOKEN_NULL_TYPE){
    freeToken(token);
    return;                                     //MIGHT CHANGE FOR RETURN SOMETHING
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == '-'){
      checkAndSkipCommentLine(fileTokenizer);
      freeToken(token);
      return;                                 //MIGHT CHANGE FOR RETURN SOMETHING
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect end of line or comment line.")
    }
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect end of line or comment line.")
  }
}

void handlePinSpec(FileTokenizer *fileTokenizer, BSinfo *bsinfo){
  Token *token;
  initPortDesc(portdesc);
  char temp[256];
  token = getTokenFromFile(fileTokenizer);

  while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(token->str == '-'){
        checkAndSkipCommentLine(fileTokenizer);
      }
    }else{
      skipLine(fileTokenizer);
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }

  if(token->type == TOKEN_IDENTIFIER_TYPE){
    strcpy(portdesc->portName,token->str);
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);

  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect identifier but is not");
  }
  //STILL LACKING...
}

/*
int getPinType(Token *token){
  int i = 0;
  int pinTypeLength = sizeof(pinT)/sizeof(char*);

  while(i < pinTypeLength){
    if(strcmp(token->str, pinT[i]) == 0){
      return i;
    }
    i++;
  }

  throwException(ERR_INVALID_PINTYPE,token,"Invalid pinType");
}

int getBitType(Token *token){
  int i = 0;
  int bitTypeLength = sizeof(bitT)/sizeof(char*);

  while(i < bitTypeLength){
    if(strcmp(token->str, bitT[i]) == 0){
      return i;
    }
    i++;
  }

  throwException(ERR_INVALID_BITTYPE,token,"Invalid bitType");
}

int getRangeType(Token *token){
  int i = 0;
  int rangeTypeLength = sizeof(rangeType)/sizeof(char*);

  while(i < rangeTypeLength){
    if(strcmp(token->str, rangeType[i]) == 0){
      return i;
    }
    i++;
  }

  throwException(ERR_INVALID_RANGETYPE,token,"Invalid rangeType");
}*/

void initPortDesc(portDesc *portdesc){
  portdesc->portName = "";
  portdesc->pinType = 0;
  portdesc->bitType = 0;
  portdesc->integer1 = 0;
  portdesc->integer2 = 0;
  portdesc->upDown = 0;
  return;
}
