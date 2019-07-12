#include <stdio.h>
#include <errno.h>
#include <ctype.h>
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


// Check for comment line
// 1 = is comment line
// 0 = not comment line
int isCommentLine(char *str){
  Token *token;
  Tokenizer *tokenizer;
  tokenizer = initTokenizer(str);
  token = getToken(tokenizer);
  int numberOfDash = 0;
  while(token->type != TOKEN_NULL_TYPE){
    // if the token is contain '-'
    if(token->type == TOKEN_OPERATOR_TYPE && (strcmp(token->str,"-") == 0)){
      numberOfDash++;
      if (numberOfDash == 2){
        break;
      }
      freeToken(token);
      token = getToken(tokenizer);
    }
    else{
      numberOfDash = 0;
      break;
    }
  }
  freeToken(token);
  freeTokenizer(tokenizer);
  if (numberOfDash == 2){
    return 1;
  }else{
    return 0;
  }

}

int stringCompare(char **str1, char *str2){
  int i = 0,j = 0;
  char *temp2;
  char *temp1 = (char *)malloc(strlen(*str1));
  strcpy(temp1,(*str1));
  temp2 = str2;
  while(temp1[i] != '\0' || temp2[j] != '\0') //if both is not NULL (still need some improvement here)
  {
    if(temp1[i] == temp2[j])  // if both are same, both pointer move forward 1
    {
      i++; j++;
    }
    else if(temp2[j] == '\0') //if the 1st word compare is all true, return 1;
    {
      (*str1) = (*str1) + i;
      free(temp1);
      return 1;
    }
    else if(temp1[i] == ' ' && isalpha(temp2[i-1])  && isalpha(temp2[i+1]))
    {
      (*str1) = (*str1) + i;
      return 0;
    }
    else if(temp2[i] == ' ' && isalpha(temp1[i-1])  && isalpha(temp1[i+1]))
    {
      (*str1) = (*str1) + i;
      return 0;
    }
    else if(temp2[j] == ' ')  //if 1 of then got 'space' move pointer forward
    {
      j++;
    }
    else if(temp1[i] == ' ')
    {
      i++;
    }
    else  //if wrong then return 0
    {
      (*str1) = (*str1) + i;
      free(temp1);
      return 0;
    }
  }
  (*str1) = (*str1) + i;
  free(temp1);
  return 1; //if pass, return 1
}

void freeFileTokenizer(FileTokenizer *tokenizer) {
  if(tokenizer) {
    //if(tokenizer->fileHandler != NULL){
    //  fclose(tokenizer->fileHandler);
    //}
    free(tokenizer);
  }
}
