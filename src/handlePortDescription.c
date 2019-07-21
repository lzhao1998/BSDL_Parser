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
#include "linkedList.h"
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
  "VREF_OUT",           //13
  NULL,
};

char *portDimension[] = {
  "bit",          //0
  "bit_vector",   //1
  NULL,
};

char *rangeT[] = {
  "to",          //0
  "downto",      //1
  NULL,
};

char *symbolChr[] = {
  "(",  //0 LEFTPAREN
  ")",  //1 RIGHTPAREN
  ";",  //2 SEMICOLON
  ":",  //3 COLON
  ",",  //4 COMMA
  "-",  //5 DASH
};

//might be changed to return Linked list
void handlePortDesc(FileTokenizer *fileTokenizer,LinkedList *port){
  Token *token;
  //LinkedList *portLinkedList;
  token = getTokenFromFile(fileTokenizer);
  //portLinkedList = (LinkedList*)malloc(sizeof(LinkedList));
  //listInit(portLinkedList);

  //check for '('
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[0]) == 0){  // if = '('
      freeToken(token);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,("Expect '(' but is %s",token->str));
    }
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect '(' but is not.");
  }

  handlePinSpec(fileTokenizer, port); //->this function will return when reach ';' in of the line
  token = getTokenFromFile(fileTokenizer);
  while (token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[2]) == 0){ //if = ";"
      handlePinSpec(fileTokenizer,port);
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else if(strcmp(token->str,symbolChr[5]) == 0){    //check , "-" the last pin spec if got comment line, skip it
      checkAndSkipCommentLine(fileTokenizer);
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
      break;
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect TOKEN_NULL TYPE but is not.");
    }
  }

  //skip the empty line and comment line
  while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_NULL_TYPE){
      skipLine(fileTokenizer);
    }else{
      if(strcmp(token->str,symbolChr[5]) == 0){ //if = '-'
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
    if(strcmp(token->str,symbolChr[0])==0){  // if  = '('
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ')'.");
    }
  }


  //CHECK FOR ;
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[2])==0){ //if = ';'
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ';'.");
    }
  }

  //check end of line and return
  if(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_EOF_TYPE){
    freeToken(token);
    return;                                     //MIGHT CHANGE FOR RETURN SOMETHING
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[5]) == 0){ // if = '-'
      checkAndSkipCommentLine(fileTokenizer);
      freeToken(token);
      return;                                 //MIGHT CHANGE FOR RETURN SOMETHING
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect end of line or comment line.");
    }
  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect end of line or comment line.");
  }
}

void handlePinSpec(FileTokenizer *fileTokenizer, LinkedList *port){
  Token *token;
  char temp[256];
  int pinTypeBit = 0;
  int portDimensionBit = 0;
  int rangeTypeBit = 0;
  int integer1 = 0;
  int integer2 = 0;
  token = getTokenFromFile(fileTokenizer);

  /*while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_NULL_TYPE){
      skipLine(fileTokenizer);
    }else{
      if(strcmp(token->str,symbolChr[5]) == 0){ //if = '-'
        checkAndSkipCommentLine(fileTokenizer);
      }else{
        break;
      }
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    printf("token type old is :%d\n",token->type );
  }*/
  freeToken(token);
  token = getTokenFromFile(fileTokenizer);

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    printf("string is :%s\n", fileTokenizer->tokenizer->str);
    printf("token typee is %d\n",token->type );
    throwException(ERR_PORT_DESCRIPTION,token,"Expect portName but its not");
  }

  while(token->type == TOKEN_IDENTIFIER_TYPE){
    strcat(temp,token->str);
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[4]) == 0){  //compare ','
        strcat(temp,token->str);
      }else{
        break;
      }
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ',' or ':' but it is not.");
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }


  if(strcmp(token->str,symbolChr[3]) != 0){  //if not = ":"
    throwException(ERR_PORT_DESCRIPTION,token,"Expect ':' but it is not.");
  }

  freeToken(token);
  token = getTokenFromFile(fileTokenizer);
  pinTypeBit = getTypeNo(token,ERR_INVALID_PINTYPE,pinT);
  freeToken(token);
  token = getTokenFromFile(fileTokenizer);
  portDimensionBit = getTypeNo(token,ERR_INVALID_PORTDIMENSION,portDimension);

  if(portDimensionBit == 1){
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[0])!=0){  //if != "("
        throwException(ERR_PORT_DESCRIPTION,token,"Expect '('.");
      }
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect '('.");
    }

    int *intArr = getRange(fileTokenizer);
    integer1 = intArr[0];
    rangeTypeBit = intArr[1];
    integer2 = intArr[2];

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[1]) != 0){
        throwException(ERR_PORT_DESCRIPTION,token,"Expect ')'.");
      }
    }else{
      throwException(ERR_PORT_DESCRIPTION,token,"Expect ')'.");
    }
  }
  freeToken(token);

  Tokenizer *tokenizer;
  Token *portNameToken;
  tokenizer = initTokenizer(temp);
  portNameToken = getToken(tokenizer);

  while(portNameToken->type != TOKEN_NULL_TYPE){
    if(portNameToken->type == TOKEN_IDENTIFIER_TYPE){
      listAddPortDesc(port,portNameToken->str,pinTypeBit,portDimensionBit,integer1,integer2,rangeTypeBit);
    }

    freeToken(portNameToken);
    portNameToken = getToken(tokenizer);
  }
  freeTokenizer(tokenizer);
}

int getTypeNo(Token *token, int errorCode, char *strArr[]){
  int i = 0;

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    throwException(errorCode,token,"Expect is identifier type.");
  }

  while(strArr[i] != NULL){
    if(strcmp(strArr[i],token->str) == 0){
      return i;
    }
    i++;
  }
  throwException(errorCode,token,"INVALID TYPE");
}

int *getRange(FileTokenizer *fileTokenizer){
  Token *token;
  int i = 0;
  int tokenType[3] = {3,8,3};
  int *rangeTypeVal = (int*)malloc(sizeof(int) * 3);

  while(i < 3){
    token = getTokenFromFile(fileTokenizer);
    if(token->type == tokenType[i]){
      if(i == 0){
        rangeTypeVal[0] = atoi(token->str);     //integer 1
      }else if(i == 1){
        rangeTypeVal[1] = getTypeNo(token,ERR_INVALID_RANGETYPE,rangeT);  //range type
      }else{
        rangeTypeVal[2] = atoi(token->str);    //integer2
      }
    }else{
      throwException(ERR_INVALID_RANGETYPE,token,"INVALID RANGE!!");
    }
    freeToken(token);
    i++;
  }

  if(rangeTypeVal[0] == rangeTypeVal[2]){
    throwException(ERR_VALUE,token,"Integer1 and Integer2 cannot be the same value.");
  }

  if(rangeTypeVal[1] == 0){
    if(rangeTypeVal[0] > rangeTypeVal[2]){
      throwException(ERR_VALUE,token,"Integer1 should be smaller than Integer2.");
    }
  }else{
    if(rangeTypeVal[0] < rangeTypeVal[2]){
      throwException(ERR_VALUE,token,"Integer2 should be smaller than Integer1.");
    }
  }

  return rangeTypeVal;
}

//add linkedlist into it
void listAddPortDesc(LinkedList *port, char *portName,int pinType,int bitType,int int1, int int2, int rangeType){
  portDesc *portD;
  portD = initPortDesc();

  portD->portName = portName;
  portD->pinType = pinType;
  portD->bitType = bitType;
  portD->integer1 = int1;
  portD->integer2 = int2;
  portD-> rangeType = rangeType;
  // Token *token;
  // token = createNullToken();
  // throwException(ERR_PORT_DESCRIPTION,token,"blabla");

  Item *item;
  item = initItem(portD);
  listAdd(port,item);
}

portDesc *initPortDesc(){
  portDesc *portdesc = malloc(sizeof(portDesc));
  portdesc->portName = "";
  portdesc->pinType = 0;
  portdesc->bitType = 0;
  portdesc->integer1 = 0;
  portdesc->integer2 = 0;
  portdesc->rangeType = 0;
  return portdesc;

}
