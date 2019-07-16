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

char *descriptionName[] = {
  "entity",         //0
  "generic",        //1
  "port",           //2
  "use",            //3
  "attribute",      //4
  "end"             //5
};

char *attributeName[] = {
  "COMPONENT_CONFORMANCE",  //0
  "PIN_MAP",                //1
  "TAP_SCAN_CLOCK",         //2
  "TAP_SCAN_IN",            //3
  "TAP_SCAN_MODE",          //4
  "TAP_SCAN_OUT",           //5
  "TAP_SCAN_RESET",         //6
  "COMPLIANCE_PATTERNS",    //7
  "INSTRUCTION_LENGTH",     //8
  "INSTRUCTION_OPCODE",     //9
  "INSTRUCTION_CAPTURE",    //10
  "IDCODE_REGISTER",        //11
  "REGISTER_ACCESS",        //12
  "BOUNDARY_LENGTH",        //13
  "BOUNDARY_REGISTER",      //14
  "DESIGN_WARNING"          //15
};


void checkAndSkipCommentLine(FileTokenizer *fileTokenizer){
  Token *token;
  token = getTokenFromFile(fileTokenizer);

  if(token->type == TOKEN_OPERATOR_TYPE){
    if(token->str == '-'){  //45 in ASCII is '-'
      skipLine(fileTokenizer);
      return;
    }else{
      throwException(ERR_INVALID_COMMEND_LINE,token,("SUPPOSE TO BE '-' but is %s",token->str));
    }
  }else{
    throwException(ERR_INVALID_COMMEND_LINE,token,"SUPPOSE TO BE '-' but is not");
  }

  return;
}

int compareDescriptionName(char *str){
  int i = 0;
  int totalLength = sizeof(descriptionName)/sizeof(char*);

  while(i < totalLength){
    if(strcmp(str,descriptionName[i]) == 0){
      return i;
    }
    i++;
  }
  return -1;
}

void handleDescSelector(int i, FileTokenizer *fileTokenizer, BSinfo *bsinfo){
  switch (i) {
    /*case 0:
      handleComponentNameDesc(fileTokenizer); //BSINFO
      break;
    case 1:
      handleGenericParameterDesc(fileTokenizer); //BSINFO
      break;*/
    case 2:
      handlePortDesc(fileTokenizer,bsinfo);
    default:
      skipLine(fileTokenizer);
      break;
  }
  return;
}

void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  int i;
  Token *token;
  token = getTokenFromFile(fileTokenizer);

  if(token->type == TOKEN_EOF_TYPE){
    //return bsinfo;
    return;
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if (token->str == "-"){
      checkAndSkipCommentLine(fileTokenizer);
      //continue;
    }else{
      throwException(ERR_INVALID_LINE,token,"Do you mean '-'?");
    }
  }else if(token->type == TOKEN_IDENTIFIER_TYPE){
    i = compareDescriptionName(token->str);
    return;
    //blabalblabla
  }
}

//check existing of file
int checkFileExists(char *file_name){
  // F_OK use to test for existence of file.
  if( access( file_name, F_OK ) != -1 ) {
    return 1;
  }
  else{
    return 0;
  }
}

FileTokenizer *createFileTokenizer(char *filename){
  FileTokenizer *fileTokenizer;
  fileTokenizer = (FileTokenizer*)malloc(sizeof(FileTokenizer));

  if (checkFileExists(filename) == 1){
    fileTokenizer->fileHandler = fopen(filename, "r");

    if(fileTokenizer->fileHandler == NULL){
      //throw error for cant open the file
      freeFileTokenizer(fileTokenizer);
      throwException(ERR_FILE_INVALID, NULL, "ERROR!! INVALID FILE!!");
    }

    char line[4096];
    fgets(line,sizeof(line),fileTokenizer->fileHandler);
    fileTokenizer->tokenizer = initTokenizer(line);
  }else{
    //throw error due to file does not exists
    freeFileTokenizer(fileTokenizer);
    throwException(ERR_FILE_NOT_EXISTS, NULL, "ERROR!! FILE DOES NOT EXISTS!!");
  }
  fileTokenizer->filename = filename;
  fileTokenizer->readLineNo = 0;
  return fileTokenizer;
}

Token *getTokenFromFile(FileTokenizer *fileTokenizer){
  Token *token;

  //tokenizer is null, return invalid token due to it reach End of File
  if (fileTokenizer->tokenizer->str == NULL){
    token = createEndOfFileToken();
    return token;
  }

  token = getToken(fileTokenizer->tokenizer);
  //token->type is NULL, then replace the tokenizer with the next line
  //If next line is EOF, tokenizer = NULL to signal that it reach EOF next getToken
  if(token->type == TOKEN_NULL_TYPE){
    freeTokenizer(fileTokenizer->tokenizer);
    char line[4096];
    int i = 0;

    while(fgets(line,sizeof(line),fileTokenizer->fileHandler) != NULL){
      i++;
      if (i > fileTokenizer->readLineNo){
        break;
      }
    }

    fileTokenizer->readLineNo++;
    //when i is not reach readline counter
    if(i < fileTokenizer->readLineNo){
      fileTokenizer->tokenizer = initTokenizer(NULL);
      return token;
    }

    if (strlen(line) != 0){
      fileTokenizer->tokenizer = initTokenizer(line);
    }else{
      fileTokenizer->tokenizer = initTokenizer(NULL);
    }
  }
  return token;
}


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

  }else{
    throwException(ERR_PORT_DESCRIPTION,token,"Expect identifier but is not");
  }

}

void initPortDesc(portDesc *portdesc){
  portdesc->portName = "";
  portdesc->pinType = 0;
  portdesc->bitType = 0;
  portdesc->integer1 = 0;
  portdesc->integer2 = 0;
  portdesc->upDown = 0;
  return;
}

//FORMAT: entity <component name> is
char *handleComponentNameDesc(FileTokenizer *fileTokenizer){
  Token *token;
  char *format[4] = {"entity","componentName","is","NULL"};
  int tokenType[4] = {8,8,8,1}; //8->identifier token, 1->NULL token
  char *componentName;
  int i = 0;

  token = getTokenFromFile(fileTokenizer);
  while(i < 4){
    //if the token type is same
    if(tokenType[i] == token->type){
      if(i == 3){ //for null token, i++
        i++;
      }else if(i == 1){ //for generic parameter, copy it to the componentName
        componentName = (char *)malloc(strlen(token->str));
        strcpy(componentName,(token->str));
        i++;
      }else if(strcmp(format[i],token->str) == 0){  //compare string but need also exclude the NULL
        i++;
      }else{ //string not same, throw error
        throwException(ERR_COMPONENT_NAME_FORMAT, token, "ERROR!! INVALID COMPONENT NAME FORMAT");
      }
    }else{  //throw error when the token type is different
      throwException(ERR_COMPONENT_NAME_FORMAT, token, "ERROR!! INVALID COMPONENT NAME FORMAT");
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }

  freeToken(token);
  return componentName;
}

//FORMAT: use <user package name><period>all<semicolon>
char *handleUseStatementDesc(FileTokenizer *fileTokenizer){
  Token *token;
  char *format[6] = {"use","componentName",".","all",";","NULL"};
  int tokenType[6] = {8,8,4,8,4,1}; //8->identifier token, 1->NULL token, 4->operator token
  char *useStatement;
  int i = 0;

  token = getTokenFromFile(fileTokenizer);
  while(i < 6){
    if (tokenType[i] == token->type){
      if(i == 5){  // for null token
        i++;
      }else if(i == 1){  // for user package name, store it
        useStatement = (char *)malloc(strlen(token->str));
        strcpy(useStatement,(token->str));
        i++;
      }else if(strcmp(format[i],token->str)==0){ //compare the string
        i++;
      }else{
        throwException(ERR_USE_STATEMENT, token, "ERROR!! INVALID USE STATEMENT FORMAT");
      }
    }else{ //token type not same, throw error
      throwException(ERR_USE_STATEMENT, token, "ERROR!! INVALID USE STATEMENT FORMAT");
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }
  freeToken(token);
  return useStatement;
}

char *handleGenericParameterDesc(FileTokenizer *fileTokenizer){
  Token *token;
  char *format[11] = {"generic","(","PHYSICAL_PIN_MAP",":","string",":","=","defaultDevicePackageType",")",";","nullToken"};
  int tokenType[11] = {8,4,8,4,8,4,4,6,4,4,1}; // 1->NULL token, 4->operator token, 6->string token, 8->identifier token
  char *genericParameter;
  int i = 0;

  token = getTokenFromFile(fileTokenizer);
  while(i < 11){
    if(tokenType[i] == token->type){
      if(i == 10){ //when get the null token, i+1
        i++;
      }else if(i == 7){   //store default device package type into genericParameter
        genericParameter = (char *)malloc(strlen(token->str));
        strcpy(genericParameter,(token->str));
        i++;
      }else if(strcmp(format[i],token->str)==0){  //compare all string
        i++;
      // if the string of i=5 is not same, it might be generic default type
      }else if((i == 5) && (strcmp(format[i+3],token->str)==0)){
        genericParameter = "";
        i = i + 4;
      }else{
        throwException(ERR_GENERIC_PARAMETER, token, "ERROR!! INVALID GENERIC PARAMETER FORMAT");
      }
    }else{ //if token type is not same
      throwException(ERR_GENERIC_PARAMETER, token, "ERROR!! INVALID GENERIC PARAMETER FORMAT");
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }
  freeToken(token);
  return genericParameter;
}

void skipLine(FileTokenizer *fileTokenizer){
  int i = 0;
  char line[4096];

  if (fgets(line,sizeof(line),fileTokenizer->fileHandler) != NULL){
    fileTokenizer->tokenizer = initTokenizer(line);
    fileTokenizer->readLineNo++;
    return;
  }else{
    fileTokenizer->tokenizer = initTokenizer(NULL);
    fileTokenizer->readLineNo++;
    return;
  }
}

//check for the VHDL identifier format
int checkVHDLidentifier(char *str){
  int strLength = strlen(str);
  int underScoreFlag = 0;
  int i = 0;

  if (str[strLength-1] == 95){  //95 => in ASCII is '_'
    return 0;
  }

  while(i < strLength){
    if(str[i] == 95){
      i++;
      underScoreFlag++;
    }else if(isalnum(str[i]) != 0){
      i++;
      underScoreFlag = 0;
    }else{
      return 0;
    }

    if (underScoreFlag == 2){
      return 0;
    }
  }

  return 1;
}

/*
char *getVhdlErrMsg(char *vhdl, int position, char *errMsg){
  char *msg  = ("%s\n%s\n%*s^\n",errMsg,vhdl,position+1);
  //printf("%s\n",errMsg);
  //printf("%s\n", vhdl);
  //printf("%*s\n", position + 1, "^");
}*/


void freeFileTokenizer(FileTokenizer *tokenizer) {
  if(tokenizer) {
    //if(tokenizer->fileHandler != NULL){
    //  fclose(tokenizer->fileHandler);
    //}
    free(tokenizer);
  }
}
