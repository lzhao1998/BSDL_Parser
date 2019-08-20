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

void printBoundaryRegister(LinkedList *list){
  Item *current;
  boundaryRegister *bs;
  current=list->head;

  if(current == NULL){
    Token *token;
    token = createNullToken(0,NULL);
    throwException(ERR_PRINTING_BOUNDARY_REGISTER,token,"Boundary register is empty!!");
  }

  while(current != NULL){
    bs = ((boundaryRegister*)current->data);
    printf("%s ( %s, %s, %s, %s", bs->cellNumber,bs->cellName,bs->portId,bs->function,bs->safeBit);

    if(strlen(bs->ccell) > 0){
      printf(", %s", bs->ccell);
    }

    if(strlen(bs->disval) > 0){
      printf(", %s, %s",bs->disval,bs->disres );
    }

    current=current->next;
    if(current == NULL){
      printf("");
    }else{
      printf("),\n");
    }
  }
  printf(");\n" );
}

// input : bsinfo, fileTokenizer
void handleBoundaryRegisterD(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  Token *token;
  int i = 0;
  char *arr[5] = {"of",bsinfo->modelName,":","entity","is",};
  int tokenType[5] = {8,8,4,8,8};

  //compare all the string, throw error if fail
  while(i < 5){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(strcmp(arr[i],token->str) != 0){   //string compare...if not same, throw error
        throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
          "Error on line: %d. Expect %s but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),arr[i],token->str);
      }
    }else{
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect %s but is not",getCorrectReadLineNo(fileTokenizer->readLineNo,token),arr[i]);
    }
    i++;
    freeToken(token);
  }

  //check for NULL and comment line, if it is not then throw error
  token = getTokenFromFile(fileTokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect Null or CommentLine but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp("-",token->str) == 0){
      checkAndSkipCommentLine(fileTokenizer);
    }else{
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Invalid comment line!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
    }
  }
  freeToken(token);
  handleBoundaryRegister(fileTokenizer,bsinfo->boundaryReg);
}

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
    portId = getPortId(fileTokenizer);

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

char *getPortId(FileTokenizer *fileTokenizer){
  Token *token;
  char *result;
  int startPos,endPos;

  token = getStringToken(fileTokenizer);
  if(token->type != TOKEN_IDENTIFIER_TYPE && token->type != TOKEN_OPERATOR_TYPE){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. %s is not a valid port id\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }

  // If is '*' symbol, return the value
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp("*",token->str) != 0){
      throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
        "Error on line: %d. Expect \"*\" symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }else{
      result = malloc(sizeof(char*) * strlen(token->str));
      strcpy(result,token->str);
      free(token);
      return result;
    }
  }else if(checkVHDLidentifier(token->str) != 1){ //check for VHDL identifier
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. %s is not a valid port name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }

  // If it is identifier type
  startPos = token->startColumn;
  endPos = startPos + token->length - 1;
  freeToken(token);
  token = getStringToken(fileTokenizer);
  if(token->type != TOKEN_OPERATOR_TYPE){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect ',' or '(' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else if((strcmp(",",token->str) != 0) && (strcmp("(",token->str) != 0)){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect ',' or '(' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }

  // if is ',', createcallback token and get the portId and return
  if(strcmp(",",token->str) == 0){
    createCallBackToken(fileTokenizer->tokenizer,token);
    result = getSubString(token->originalStr,startPos,endPos);
    freeToken(token);
    return result;
  }

  // if is '(',  check for (<number>)
  freeToken(token);
  token = getStringToken(fileTokenizer);

  //check for <number>
  if(token->type != TOKEN_INTEGER_TYPE){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect integer but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }

  // check for ')'
  freeToken(token);
  token = getStringToken(fileTokenizer);
  if(token->type != TOKEN_OPERATOR_TYPE){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect ')' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else if(strcmp(")",token->str) != 0){
    throwException(ERR_INVALID_BOUNDARY_REGISTER,token, \
      "Error on line: %d. Expect ')' symbol but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else{
    endPos = token->startColumn;
  }

  // obtain the portId and return
  result = getSubString(token->originalStr,startPos,endPos);
  freeToken(token);
  return result;
}


// get substring by using pointer
char *getSubString(char *oriStr, int startPos, int endPos){
  int totalLength = endPos - startPos + 1; // + 1 because need include null
  char *result = malloc(sizeof(char*) * (totalLength+1));

  int i;
  for( i = 0; i < totalLength; i++){
   *(result+i) = *(oriStr+startPos+i);
  }

  *(result+i) = '\0';
  return result;
  // int i;
  // char res[totalLength+1];
  // for( i = 0; i < totalLength; i++){
  //   res[i] = oriStr[startPos+i];
  // }
  // res[i] = '\0';
  // strcpy(result,res);
  // return result;
}
