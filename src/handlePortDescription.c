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
#include "linkedList.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"

char errmsg[100];

char *pinT[] = {
  "in",                 //0
  "out",                //1
  "inout",              //2
  "buffer",             //3
  "linkage",            //4
  "LINKAGE_INOUT",      //5
  "LINKAGE_BUFFER",     //6
  "LINKAGE_IN",         //7
  "LINKAGE_OUT",        //8
  "LINKAGE_MECHANICAL", //9
  "POWER_0",            //10
  "POWER_POS",          //11
  "POWER_NEG",          //12
  "VREF_IN",            //13
  "VREF_OUT",           //14
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

void handlePortDesc(FileTokenizer *fileTokenizer,LinkedList *port){
  Token *token;
  token = getTokenFromFile(fileTokenizer);

  //check for '('
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[0]) == 0){  // if = '('
      freeToken(token);
    }else{
      sprintf(errmsg,"Error on line: %d. Expect '(' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
  }else{
    sprintf(errmsg,"Error on line: %d. Expect '(' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }

  handlePinSpec(fileTokenizer, port); // this function will return when reach ';' in of the line
  token = getTokenFromFile(fileTokenizer);
  while (token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[2]) == 0){ //if = ";"
      handlePinSpec(fileTokenizer,port);
    }else if(strcmp(token->str,symbolChr[5]) == 0){    //check , "-" the last pin spec if got comment line, skip it
      checkAndSkipCommentLine(fileTokenizer);
    }else{
      break;
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }

  //skip the empty line and comment line
  while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_NULL_TYPE){
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
      continue;
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
    if(strcmp(token->str,symbolChr[1])==0){  // if  = ')'
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      sprintf(errmsg,"Error on line: %d. Expect ')' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
  }else{
    sprintf(errmsg,"Error on line: %d. Expect ')' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }

  //CHECK FOR ;
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[2])==0){ //if = ';'
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
    }else{
      sprintf(errmsg,"Error on line: %d. Expect ';' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
  }else{
    sprintf(errmsg,"Error on line: %d. Expect ';' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }

  //check end of line and return
  if(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_EOF_TYPE){
    freeToken(token);
    return;
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChr[5]) == 0){ // if = '-'
      checkAndSkipCommentLine(fileTokenizer);
      freeToken(token);
      return;
    }else{
      sprintf(errmsg,"Error on line: %d. Expect comment line or end of line but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
  }else{
    sprintf(errmsg,"Error on line: %d. Expect comment line or end of line but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }
}

void handlePinSpec(FileTokenizer *fileTokenizer, LinkedList *port){
  Token *token;
  char *temp = malloc(sizeof(char)*100);
  char *emptyString = "";
  strcpy(temp,emptyString);   //let the temp string is empty to avoid unnessary item inside it
  int pinTypeBit = 0;
  int portDimensionBit = 0;
  int rangeTypeBit = 0;
  int integer1 = 0;
  int integer2 = 0;
  token = getTokenFromFile(fileTokenizer);

  while(token->type == TOKEN_NULL_TYPE || token->type == TOKEN_OPERATOR_TYPE){
    if(token->type == TOKEN_NULL_TYPE){
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
      continue;
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

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    sprintf(errmsg,"Error on line: %d. Expect portName but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }

  //get all the portname
  while(token->type == TOKEN_IDENTIFIER_TYPE){
     if (checkVHDLidentifier(token->str)== 0){
       sprintf(errmsg,"Error on line: %d. %s is not a valid portname",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
       throwException(ERR_INVALID_PORTNAME,token,errmsg);
     }
    if(strlen(temp) == 0){
      strcpy(temp,token->str);
    }else{
      strcat(temp,token->str);
    }

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[4]) == 0){  //compare ','
        strcat(temp,token->str);
      }else{
        break;
      }
    }else{
      sprintf(errmsg,"Error on line: %d. Expect ',' or ':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }

  if(strcmp(token->str,symbolChr[3]) != 0){  //if not = ":"
  sprintf(errmsg,"Error on line: %d. Expect':' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(ERR_PORT_DESCRIPTION,token,errmsg);
  }

  freeToken(token);
  token = getTokenFromFile(fileTokenizer);
  pinTypeBit = getTypeNo(fileTokenizer,token,ERR_INVALID_PINTYPE,pinT);
  freeToken(token);
  token = getTokenFromFile(fileTokenizer);
  portDimensionBit = getTypeNo(fileTokenizer,token,ERR_INVALID_PORTDIMENSION,portDimension);

  if(portDimensionBit == 1){
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[0])!=0){  //if != "("
      sprintf(errmsg,"Error on line: %d. Expect '(' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        throwException(ERR_PORT_DESCRIPTION,token,errmsg);
      }
    }else{
      sprintf(errmsg,"Error on line: %d. Expect '(' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }

    int *intArr = getRange(fileTokenizer);
    integer1 = intArr[0];
    rangeTypeBit = intArr[1];
    integer2 = intArr[2];

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
    if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp(token->str,symbolChr[1]) != 0){
        sprintf(errmsg,"Error on line: %d. Expect ')' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        throwException(ERR_PORT_DESCRIPTION,token,errmsg);
      }
    }else{
      sprintf(errmsg,"Error on line: %d. Expect ')' but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      throwException(ERR_PORT_DESCRIPTION,token,errmsg);
    }
  }else{
    rangeTypeBit = 2; //let it become null
  }
  freeToken(token);

  Tokenizer *tokenizer;
  Token *portNameToken;
  tokenizer = initTokenizer(temp);
  portNameToken = getToken(tokenizer);

  while(portNameToken->type != TOKEN_NULL_TYPE){
    if(portNameToken->type == TOKEN_IDENTIFIER_TYPE){
      if (checkPortNameAppearance(port,portNameToken->str) == 1){
        char errmsg[50];
        sprintf(errmsg,"Error on line: %d. %s is declare more than one!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        throwException(ERR_MULTIPLE_DECLARE,portNameToken,errmsg);
      }

      listAddPortDesc(port,portNameToken->str,pinTypeBit,portDimensionBit,integer1,integer2,rangeTypeBit);
    }

    freeToken(portNameToken);
    portNameToken = getToken(tokenizer);
  }

  freeToken(portNameToken);
  freeTokenizer(tokenizer);
  free(temp);
}

int getTypeNo(FileTokenizer *fileTokenizer,Token *token, int errorCode, char *strArr[]){
  int i = 0;

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    sprintf(errmsg,"Error on line: %d. Expect is VHDL Identifier but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    throwException(errorCode,token,errmsg);
  }

  while(strArr[i] != NULL){
    if(strcasecmp(strArr[i],token->str) == 0){
      return i;
    }
    i++;
  }
  sprintf(errmsg,"Error on line: %d. Invalid type for %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  throwException(errorCode,token,errmsg);
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
        rangeTypeVal[1] = getTypeNo(fileTokenizer,token,ERR_INVALID_RANGETYPE,rangeT);  //range type
      }else{
        rangeTypeVal[2] = atoi(token->str);    //integer2
      }
    }else{
      sprintf(errmsg,"Error on line: %d. %s is not having valid range",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->originalStr);
      throwException(ERR_INVALID_RANGETYPE,token,errmsg);
    }
    freeToken(token);
    i++;
  }

  if(rangeTypeVal[0] == rangeTypeVal[2]){
    sprintf(errmsg,"Error on line: %d. Integer 1 and Integer2 cannot be same value.",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
    throwException(ERR_VALUE,token,errmsg);
  }

  if(rangeTypeVal[1] == 0){
    if(rangeTypeVal[0] > rangeTypeVal[2]){
      sprintf(errmsg,"Error on line: %d. Integer1 should be smaller than Integer2.",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      throwException(ERR_VALUE,token,errmsg);
    }
  }else{
    if(rangeTypeVal[0] < rangeTypeVal[2]){
      sprintf(errmsg,"Error on line: %d. Integer2 should be smaller than Integer1.",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      throwException(ERR_VALUE,token,errmsg);
    }
  }
  return rangeTypeVal;
}

void listAddPortDesc(LinkedList *port, char *portName,int pinType,int bitType,int int1, int int2, int rangeType){
  portDesc *portD;
  portD = initPortDesc();

  portD->portName = malloc(sizeof(char));
  strcpy(portD->portName,portName);
  portD->pinType = pinType;
  portD->bitType = bitType;
  portD->integer1 = int1;
  portD->integer2 = int2;
  portD-> rangeType = rangeType;

  Item *item;
  item = initItem(portD);
  listAdd(port,item);
}

void printPortDesc(LinkedList *list){
  Item *current;
  portDesc *port;
  current=list->head;

  if(current == NULL){
    Token *token;
    token = createNullToken(0,NULL);
    throwException(ERR_PRINTING_PORTDESC,token,"Port description is empty!!");
  }
  printf("port (\n" );
  while(current != NULL){
    port = ((portDesc*)current->data);
    if(port->bitType == 1){
      printf("\t %s : %s %s (%d %s %d)", port->portName,pinT[port->pinType],\
      portDimension[port->bitType],port->integer1,rangeT[port->rangeType],port->integer2);
    }else{
      printf("\t %s : %s %s", port->portName,pinT[port->pinType],\
      portDimension[port->bitType]);
    }

		current=current->next;
    if(current == NULL){
      printf("\n");
    }else{
      printf(";\n");
    }
  }
  printf(");\n" );
}

// 1 : overlap, 0 : no overlap
int checkPortNameAppearance(LinkedList *list, char *str){
  Item *previous,*current;
  portDesc *port;
  current=list->head;

  if(current == NULL){
    return 0;
  }
  while(current != NULL){
    port = ((portDesc*)current->data);
    if(strcmp(port->portName,str) == 0){
      return 1;
    }
    current=current->next;
  }
  return 0;
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
