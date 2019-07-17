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



void handlePortDesc(FileTokenizer *fileTokenizer,BSinfo *bsinfo){
  Token *token;
  token = getTokenFromFile(fileTokenizer);

  if(token->type == TOKEN_OPERATOR_TYPE){  // check '('
    if(token->str == '('){
      freeToken(token);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,("Expect '(' but is %s",token->str));
    }
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect '(' but is not.");
  }

  token = getTokenFromFile(fileTokenizer);
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == '('){
      checkAndSkipCommentLine(fileTokenizer);
    }
  }else if(token->type == TOKEN_NULL_TYPE){
    skipLine(fileTokenizer);
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect null but is not.");
  }
  freeToken(token);
  handlePinSpec(fileTokenizer,bsinfo);

  //blablabla
  //check for );
}

void handlePinSpec(FileTokenizer *fileTokenizer, BSinfo *bsinfo){
  Token *token;
  portDesc *portdesc;
  portdesc = (portDesc*)malloc(sizeof(portDesc));
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
