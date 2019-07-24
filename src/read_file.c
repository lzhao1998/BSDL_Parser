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

char *symbolChar[] = {
  "(",  //0 LEFTPAREN
  ")",  //1 RIGHTPAREN
  ";",  //2 SEMICOLON
  ":",  //3 COLON
  ",",  //4 COMMA
  "-",  //5 DASH
};


void checkAndSkipCommentLine(FileTokenizer *fileTokenizer){
  Token *token;
  char *temp; ///***SAME PROBLEM IN BSDL PARSER THERE**//
  temp = "-";
  token = getTokenFromFile(fileTokenizer);

  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,temp) == 0){  //45 in ASCII is '-'
      skipLine(fileTokenizer);
      //return;
    }else{
      throwException(ERR_INVALID_COMMEND_LINE,token,("SUPPOSE TO BE '-' but is %s",token->str));
    }
  }else{
    throwException(ERR_INVALID_COMMEND_LINE,token,"SUPPOSE TO BE '-' but is not");
  }
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
    case 0:
      handleComponentNameDesc(bsinfo, fileTokenizer);
      break;
    case 1:
      handleGenericParameterDesc(bsinfo,fileTokenizer); //BSINFO
      break;
    case 2:
      handlePortDesc(fileTokenizer,bsinfo->port);
      break;
    case 3:
      handleUseStatementDesc(bsinfo, fileTokenizer);
      break;
    default:
      skipLine(fileTokenizer);
      break;
  }
  return;
}

void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  int i = 0;
  Token *token;
  token = getTokenFromFile(fileTokenizer);
char *temp;  //********TRY TO FIGURE OUT WHY CANNOT*********///
temp = "-"; //*****CANNOT STRAIGHT TOKEN->STR=="-" ****///

  while(token->type != TOKEN_EOF_TYPE){     //when is not EOF loop it
    if(token->type == TOKEN_NULL_TYPE){ //when it is NULL type at the first token of then line, skip line
      //skipLine(fileTokenizer);
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
      continue;
    }else if(token->type == TOKEN_OPERATOR_TYPE){ //if it is comment line, skip the line
      if (strcmp(token->str,temp) == 0){  ////HERe
        checkAndSkipCommentLine(fileTokenizer);
        //continue;
      }else{  // when the comment Line format not is not fullfill, throw error
        throwException(ERR_INVALID_LINE,token,"Do you mean '-'?");
      }
    }else if(token->type == TOKEN_IDENTIFIER_TYPE){ //if it is identifier, check it and do something
      i = compareDescriptionName(token->str);
      handleDescSelector(i, fileTokenizer, bsinfo);
    }else{                                          //else skip the Line
      skipLine(fileTokenizer);
    }

    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
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

    char line[5000];
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
    char line[3000];
    if(fgets(line,sizeof(line),fileTokenizer->fileHandler) != NULL){
      fileTokenizer->tokenizer = initTokenizer(line);
    }else{
      fileTokenizer->tokenizer = initTokenizer(NULL);
    }

    fileTokenizer->readLineNo++;

  }
  return token;
}

//FORMAT: entity <component name> is
void handleComponentNameDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  if(strlen(bsinfo->modelName) > 0){
    throwException(ERR_USE_STATEMENT,NULL,"Use statement appear more than one!!");
  }

  char *format[3] = {NULL,"is", NULL};
  int tokenType[3] = {8,8,1}; //8->identifier token, 1->NULL token
  int length = sizeof(tokenType)/sizeof(tokenType[0]);
  bsinfo->modelName = getString(fileTokenizer,format,tokenType,ERR_USE_STATEMENT,length);
}

//FORMAT: use <user package name><period>all<semicolon>
void handleUseStatementDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  if(strlen(bsinfo->useStatement) > 0){
    throwException(ERR_USE_STATEMENT,NULL,"Use statement appear more than one!!");
  }

  char *format[5] = {NULL,".","all",";",NULL};
  int tokenType[5] = {8,4,8,4,1}; //8->identifier token, 1->NULL token, 4->operator token
  int length = sizeof(tokenType)/sizeof(tokenType[0]);
  bsinfo->useStatement = getString(fileTokenizer,format,tokenType,ERR_USE_STATEMENT,length);
}

void handleGenericParameterDesc(BSinfo *bsinfo,FileTokenizer *fileTokenizer){
  Token *token;
  char *format[10] = {"(","PHYSICAL_PIN_MAP",":","string",":","=",NULL,")",";",NULL};
  int tokenType[10] = {4,8,4,8,4,4,6,4,4,1}; // 1->NULL token, 4->operator token, 6->string token, 8->identifier token
  char *genericParameter;
  genericParameter = "";
  int i = 0;

  while(i < 10){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(i == 9){ //when get the null token, i+1
        i++;
      }else if(i == 6){   //store default device package type into genericParameter
        if(checkVHDLidentifier(token->str) == 0){
          throwException(ERR_GENERIC_PARAMETER,token,"Invalid package name!!");
        }
        genericParameter = (char *)malloc(strlen(token->str));
        strcpy(genericParameter,(token->str));
        i++;
      }else if(strcmp(format[i],token->str)==0){  //compare all string
        i++;
      // if the string of i=5 is not same, it might be generic default type
      }else if((i == 4) && (strcmp(format[i+3],token->str)==0)){
        genericParameter = "";
        i = i + 4;
      }else{
        throwException(ERR_GENERIC_PARAMETER, token, "ERROR!! INVALID GENERIC PARAMETER FORMAT");
      }
    }else{
      if(i == 9 && token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(token->str,symbolChar[5]) == 0){
          checkAndSkipCommentLine(fileTokenizer);
        }
        else{
          throwException(ERR_GENERIC_PARAMETER,token,"Expect null or comment line but it is not!!");
        }
      }else{
        throwException(ERR_GENERIC_PARAMETER, token, "ERROR!! INVALID GENERIC PARAMETER FORMAT");
      }
    }
    freeToken(token);
  }

  bsinfo->packageName =  genericParameter;
}

void skipLine(FileTokenizer *fileTokenizer){
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
// 0: fail, 1 pass
int checkVHDLidentifier(char *str){
  int strLength = strlen(str);
  int underScoreFlag = 0;
  int i = 0;
  char *underscore;
  underscore = "_";

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

void initBSinfo(BSinfo *bsinfo){
  bsinfo->modelName = "";
  bsinfo->packageName = "";
  bsinfo->useStatement = "";
  //bsinfo->port = (LinkedList*)malloc(sizeof(LinkedList));
  bsinfo->port = listInit();
}

//who can use: entity, useStatement
char *getString(FileTokenizer *fileTokenizer, char *strArr[], int *tokenType, int errorCode, int length){
  Token *token;
  char *errmsg;
  int i = 0;
  char *str;

  while(i < length){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(token->type == TOKEN_IDENTIFIER_TYPE){
        if(strArr[i] == NULL){  //store string when strArr is NULL
          if(checkVHDLidentifier(token->str) == 0){ //check VHDL identifier is  correct or not
            throwException(errorCode,token,"Invalid VHDL Identifier");
          }
          str = (char *)malloc(strlen(token->str));
          strcpy(str,(token->str));
        //compare token->str with strArr[i]. Throw error when not same, else continue
        }else if(strcmp(strArr[i],token->str) != 0){
          sprintf(errmsg,"Expect %s but is %s",strArr[i],token->str);
          throwException(errorCode,token,errmsg);
        }
      //check for the operator type character
      }else if(token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(strArr[i],token->str) != 0){
          sprintf(errmsg,"Expect %s but is %s",strArr[i],token->str);
          throwException(errorCode,token,errmsg);
        }
      }
    }else{
      if(i == (length - 1) && token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(token->str,symbolChar[5]) == 0){
          checkAndSkipCommentLine(fileTokenizer);
        }
        else{
          throwException(errorCode,token,"Expect null or comment line but it is not!!");
        }
      }
      else{
        throwException(errorCode,token,"Invalid format!!");
      }
    }

    i++;
    freeToken(token);
  }
  return str;
}

void freeBsInfo(BSinfo *bsinfo){
  if(bsinfo){
    free(bsinfo);
  }
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
