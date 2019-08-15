#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "getStrToken.h"
#include "linkedList.h"
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "handleScanPortIdentification.h"
#include "handleBoundaryRegisterDesc.h"

void handleBoundaryRegister(FileTokenizer *fileTokenizer, LinkedList *list){
  Token *token;
  char *cellNumber,*cellName,*portId,*function,*safeBit,*ccell, *disval,*disres;
  int intFlag = 0;

  while(1){
    intFlag = 0;
    token = getStringToken(fileTokenizer);

    // get the cellNumber
    if(token->type != TOKEN_INTEGER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect integer but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      cellNumber = (char *)malloc(strlen(token->str));
      strcpy(cellNumber,(token->str));
    }

    // check for '(' symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d .Expect '(' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp("(",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d .Expect '(' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // get the cellName
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. %s is not a valid Cell Name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      cellName = (char *)malloc(strlen(token->str));
      strcpy(cellName,(token->str));
    }

    // check "," symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp(",",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // get portId
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE && token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. %s is not a valid port id\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if(token->type == TOKEN_OPERATOR_TYPE){
      if(strcmp("*",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect \"*\" symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }else{
      portId = (char *)malloc(strlen(token->str));
      strcpy(portId,(token->str));
    }

    // check "," symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp(",",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // get the function
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. %s is not a valid Function Name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(checkFunctionName(token->str) != 1){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. %s is not a valid Function Name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
      function = (char *)malloc(strlen(token->str));
      strcpy(function,(token->str));
    }

    // check "," symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp(",",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    //get safe bit
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE && token->type != TOKEN_INTEGER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect 0/1/x but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if((strcmp("0",token->str) == 0) || (strcmp("1",token->str) == 0) || (strcmp("X",token->str) == 0)){
        safeBit = (char *)malloc(strlen(token->str));
        strcpy(safeBit,(token->str));
      }else{
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect 0/1/x but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // check "," and ")" symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' or ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if((strcmp(",",token->str) != 0) && (strcmp(")",token->str) != 0)){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' or ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if(strcmp(")",token->str) == 0){
      freeToken(token);
      token = getStringToken(fileTokenizer);
      if(token->type != TOKEN_OPERATOR_TYPE){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }else if(strcmp(",",token->str) == 0){
        listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,"","","");
        freeToken(token);
        continue;
      }else if(strcmp(";",token->str) == 0){
        listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,"","","");
        freeToken(token);
        break;
      }else{
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // get ccell => input spec or fisable spec
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_INTEGER_TYPE && token->type != TOKEN_IDENTIFIER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect integer or input spec but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if(token->type == TOKEN_IDENTIFIER_TYPE){
      if(checkInputSpec(token->str) != 1){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. %s is not a valid input spec\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }else{
      intFlag = 1;
    }
    ccell = (char *)malloc(strlen(token->str));
    strcpy(ccell,(token->str));

    // check "," and ")" symbol
    // , for disable spec and ) for input spec
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' or ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if((strcmp(",",token->str) != 0) && (strcmp(")",token->str) != 0)){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' or ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if(strcmp(")",token->str) == 0){  //for input spec only
      if(intFlag == 1){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
      freeToken(token);
      token = getStringToken(fileTokenizer);
      if(token->type != TOKEN_OPERATOR_TYPE){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }else if(strcmp(",",token->str) == 0){
        listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,ccell,"","");
        freeToken(token);
        continue;
      }else if(strcmp(";",token->str) == 0){
        listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,ccell,"","");
        freeToken(token);
        break;
      }else{
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }else{  // if ","
      if(intFlag == 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    //get disable value
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_INTEGER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect 0 or 1 but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if((strcmp("0",token->str) != 0) && (strcmp("1",token->str) != 0)){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect 0 or 1 but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }
    disval = (char *)malloc(strlen(token->str));
    strcpy(disval,(token->str));

    // check "," symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp(",",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    //get disable result
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_IDENTIFIER_TYPE && token->type != TOKEN_IDENTIFIER_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. %s is not a valid disable result.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else if(checkDisableResult(token->str) != 1){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. %s is not a valid disable result.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }
    disres = (char *)malloc(strlen(token->str));
    strcpy(disres,(token->str));

    // check ")" symbol
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if(strcmp(")",token->str) != 0){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ')' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    // check wether is continue or not
    freeToken(token);
    token = getStringToken(fileTokenizer);
    if(token->type != TOKEN_OPERATOR_TYPE){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect ',' or ';' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      if((strcmp(";",token->str)) != 0 && (strcmp(",",token->str) != 0)){
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect ',' or ';' but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }

    if(strcmp(";",token->str) == 0){
      listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,ccell,disval,disres);
      freeToken(token);
      break;
    }else{
      listAddBoundaryRegister(list,cellNumber,cellName,portId,function,safeBit,ccell,disval,disres);
      freeToken(token);
      continue;
    }

  }
}

void listAddBoundaryRegister(LinkedList *list,char* cellNum,char*cellname,char*portid,char*function,char*safebit,char*ccell,char*disV,char*disR){
  boundaryRegister *boundR;
  boundR = initBR();

  boundR->cellNumber = malloc(sizeof(char));
  strcpy(boundR->cellNumber,cellNum);
  boundR->cellName = malloc(sizeof(char));
  strcpy(boundR->cellName,cellname);
  boundR->portId = malloc(sizeof(char));
  strcpy(boundR->portId,portid);
  boundR->function = malloc(sizeof(char));
  strcpy(boundR->function,function);
  boundR->safeBit = malloc(sizeof(char));
  strcpy(boundR->safeBit,safebit);
  boundR->ccell = malloc(sizeof(char));
  strcpy(boundR->ccell,ccell);
  boundR->disval = malloc(sizeof(char));
  strcpy(boundR->disval,disV);
  boundR->disres = malloc(sizeof(char));
  strcpy(boundR->disres,disR);

  Item *item;
  item = initItem(boundR);
  listAdd(list,item);
}


boundaryRegister *initBR(){
  boundaryRegister *boundaryR;
  boundaryR = (boundaryRegister*)malloc(sizeof(boundaryRegister));
  boundaryR->cellNumber = "";
  boundaryR->cellName = "";
  boundaryR->portId = "";
  boundaryR->function = "";
  boundaryR->safeBit = "";
  boundaryR->ccell = "";
  boundaryR->disval = "";
  boundaryR->disres = "";
  return boundaryR;
}

int checkFunctionName(char *str){
  char *funcN[] = {"INPUT","OUTPUT2","OUTPUT3","CONTROL","CONTROLR","INTERNAL","CLOCK","BIDIR","OBSERVE_ONLY",NULL};
  int i = 0;

  while(funcN[i] != NULL){
    if(strcmp(funcN[i],str) == 0){
      return 1;
    }
    i++;
  }
  return 0;
}

int checkInputSpec(char *str){
  char *inputSpecName[] = {"EXTERN0","EXTERN1","PULL0","PULL1","OPEN0","OPEN1","KEEPER","OPENX","EXPECT1","EXPECT0",NULL};
  int i = 0;

  while(inputSpecName[i] != NULL){
    if(strcmp(inputSpecName[i],str) == 0){
      return 1;
    }
    i++;
  }
  return 0;
}

int checkDisableResult(char *str){
  char *disableResultName[] = {"WEAK0","WEAK1","PULL0","PULL1","OPEN0","OPEN1","KEEPER","Z",NULL};
  int i = 0;

  while(disableResultName[i] != NULL){
    if(strcmp(disableResultName[i],str) == 0){
      return 1;
    }
    i++;
  }
  return 0;
}
