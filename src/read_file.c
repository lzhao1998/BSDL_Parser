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
#include "linkedList.h"
#include "handleBoundaryRegisterDesc.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "handlePinMappingDesc.h"
#include "handleScanPortIdentification.h"


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
  "-"   //5 DASH
};

char *standardPackageName[] = {
  "STD_1149_1_1990",
  "STD_1149_1_1994",
  "STD_1149_1_2001",
  "STD_1149_1_2013",
  NULL
};

// Handle the description. match = call specific function to handle it
// If description not match with any string in descriptionName array, throw error
void handleDescSelector(char *str, FileTokenizer *fileTokenizer, BSinfo *bsinfo){
  Token *token;
  int i = 0;

  while(descriptionName[i] != NULL){
    if(strcmp(descriptionName[i],str)== 0){
      break;
    }else{
      i++;
    }
  }

  switch (i) {
    case ENTITY:   // component name
      if(strlen(bsinfo->modelName) > 0){
        token = getTokenFromFile(fileTokenizer);
        throwException(ERR_COMPONENT_NAME_FORMAT,token, \
          "Error on line: %d. Component name appear more than one!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleComponentNameDesc(bsinfo, fileTokenizer);
      break;
    case GENERIC: // generic parameter
      handleGenericParameterDesc(bsinfo,fileTokenizer); //BSINFO
      break;
    case PORT: // port description
      handlePortDesc(fileTokenizer,bsinfo->port);
      break;
    case USE: // use statement
      if(strlen(bsinfo->useStatement) > 0){
        token = getTokenFromFile(fileTokenizer);
        throwException(ERR_USE_STATEMENT,token, \
          "Error on line: %d. Use statement appear more than one!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleUseStatementDesc(bsinfo, fileTokenizer);
      break;
    case ATTRIBUTE: //go to attribute selector
      handleAttributeSelector(bsinfo, fileTokenizer);
      break;
    case ERROR_TYPE: //throw error due to not match/ invalid description
      token = getTokenFromFile(fileTokenizer);
      throwException(ERR_INVALID_DESCRIPTION,token, \
        "Error on line: %d. %s is not a valid description.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),str);
      break;
    default:
      skipLine(fileTokenizer);
      break;
  }
  return;
}

// Handle the attribute. match = call specific function to handle it
// If attribute not match with any string in attributeName array, throw error
void handleAttributeSelector(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  Token *token;
  int i = 0;
  token = getTokenFromFile(fileTokenizer);

  if(token->type != TOKEN_IDENTIFIER_TYPE){
    throwException(ERR_INVALID_ATTRIBUTE,token, \
      "Error on line: %d. Expect valid attribute name but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }

  while(attributeName[i] != NULL){
    if(strcmp(attributeName[i],token->str)== 0){
      break;
    }else{
      i++;
    }
  }

  switch (i) {
    case COMPONENT_CONFORMANCE:
      if(strlen(bsinfo->componentConformance) != 0){
        throwException(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT,token, \
          "Error on line: %d. COMPONENT_CONFORMANCE is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleComponentConformanceDesc(bsinfo, fileTokenizer);
      break;
    case PIN_MAP:
      handlePinMappingStatementDesc(bsinfo, fileTokenizer);
      break;
    case TAP_SCAN_CLOCK:
      if(strlen(bsinfo->tapScanClk->portId) != 0 || strlen(bsinfo->tapScanClk->haltState) != 0){
        throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
          "Error on line: %d. TAP_SCAN_CLOCK is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleTapScanClockDesc(bsinfo,fileTokenizer);
      break;
    case TAP_SCAN_IN:
      if(strlen(bsinfo->tapScanIn->portId) != 0){
        throwException(ERR_INVALID_TAP_SCAN_IN_FORMAT,token, \
          "Error on line: %d. TAP_SCAN_IN is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleScanPortDesc(bsinfo->tapScanIn,fileTokenizer,0);
      break;
    case TAP_SCAN_MODE:
      if(strlen(bsinfo->tapScanMode->portId) != 0){
        throwException(ERR_INVALID_TAP_SCAN_MODE_FORMAT,token, \
          "Error on line: %d. TAP_SCAN_MODE is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleScanPortDesc(bsinfo->tapScanMode,fileTokenizer,1);
      break;
    case TAP_SCAN_OUT:
      if(strlen(bsinfo->tapScanOut->portId) != 0){
        throwException(ERR_INVALID_TAP_SCAN_OUT_FORMAT,token, \
          "Error on line: %d. TAP_SCAN_OUT is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleScanPortDesc(bsinfo->tapScanOut,fileTokenizer,2);
      break;
    case TAP_SCAN_RESET:
      if(strlen(bsinfo->tapScanReset->portId) != 0){
        throwException(ERR_INVALID_TAP_SCAN_RESET_FORMAT,token, \
          "Error on line: %d. TAP_SCAN_RESET is declare more than one!!\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      handleScanPortDesc(bsinfo->tapScanReset,fileTokenizer,3);
      break;
    case INSTRUCTION_LENGTH:
      if(bsinfo->instructionLength != -1){
        throwException(ERR_INVALID_INSTRUCTION_LENGTH,token, \
          "Error on line: %d. Instruction Length appear more than one!!", getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      bsinfo->instructionLength = handleInstructionAndBoundaryLength(fileTokenizer, ERR_INVALID_INSTRUCTION_LENGTH, bsinfo->modelName, 0);
      break;
    case BOUNDARY_LENGTH:
      if(bsinfo->boundaryLength != -1){
        throwException(ERR_INVALID_BOUNDARY_LENGTH,token, \
          "Error on line: %d. Boudary Length appear more than one!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
      }
      bsinfo->boundaryLength = handleInstructionAndBoundaryLength(fileTokenizer, ERR_INVALID_BOUNDARY_LENGTH, bsinfo->modelName, 1);
      break;
    case BOUNDARY_REGISTER:
      handleBoundaryRegisterD(bsinfo,fileTokenizer);
      break;
    case ERROR_ATTRIBUTE:
      throwException(ERR_INVALID_ATTRIBUTE,token, \
        "Error on line: %d. %s is not a valid attribute name.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      break;
    default:
      skipLine(fileTokenizer);
      break;
  }
  freeToken(token);
  return;
}

// Main function for BSDL Parser
void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  int i = 0;
  Token *token;
  token = getTokenFromFile(fileTokenizer);


  while(token->type != TOKEN_EOF_TYPE){     //when is not EOF loop it
    if(token->type == TOKEN_NULL_TYPE){ //when it is NULL type at the first token of then line, skip line
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);
      continue;
    }else if(token->type == TOKEN_OPERATOR_TYPE){ //if it is comment line, skip the line
      if (strcmp(token->str,symbolChar[5]) == 0){
        checkAndSkipCommentLine(fileTokenizer);
      }else{  // when the comment Line format not is not fullfill, throw error
        throwException(ERR_INVALID_LINE,token, \
          "Error on line: %d. Do you mean '-'? But is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }
    }else if(token->type == TOKEN_IDENTIFIER_TYPE){ //if it is identifier, check it and do something
      handleDescSelector(token->str, fileTokenizer, bsinfo);
      freeToken(token);
      token = getTokenFromFile(fileTokenizer);

      continue;
    }else{                                     //else skip the Line
      skipLine(fileTokenizer);
    }
    freeToken(token);
    token = getTokenFromFile(fileTokenizer);
  }
  freeToken(token);
}

// Handle Component Name Description
// FORMAT: entity <component name> is
void handleComponentNameDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  char *format[3] = {NULL,"is", NULL};
  int tokenType[3] = {8,8,1}; //8->identifier token, 1->NULL token
  int length = sizeof(tokenType)/sizeof(tokenType[0]);
  bsinfo->modelName = getString(fileTokenizer,format,tokenType,ERR_COMPONENT_NAME_FORMAT,length, 0);
}

// Handle Use Statement Description
// FORMAT: use <user package name>.all;
void handleUseStatementDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  char *format[5] = {NULL,".","all",";",NULL};
  int tokenType[5] = {8,4,8,4,1}; //8->identifier token, 1->NULL token, 4->operator token
  int length = sizeof(tokenType)/sizeof(tokenType[0]);
  bsinfo->useStatement = getString(fileTokenizer,format,tokenType,ERR_USE_STATEMENT,length, 1);
}

// Handle Component Conformance description
// FORMAT: attribute COMPONENT_CONFORMANCE of <modelName> : entity is <componentConformance>;
void handleComponentConformanceDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  char *format[8] = {"of",bsinfo->modelName,":","entity","is",NULL,";",NULL};
  int tokenType[8] = {8,8,4,8,8,6,4,1}; //8->identifier token, 1->NULL token, 4->operator token
  int length = sizeof(tokenType)/sizeof(tokenType[0]);
  bsinfo->componentConformance = getString(fileTokenizer,format,tokenType,ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT,length, 1);
}

// Handle the Generic Parameter Description
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
          throwException(ERR_GENERIC_PARAMETER,token, \
            "Error on line %d. Invalid package name!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
        }

        if(strlen(bsinfo->packageName) > 0){
          printf("%d\n",strlen(bsinfo->packageName) );
          throwException(ERR_GENERIC_PARAMETER,token, \
            "Error on line: %d. Generic Parameter is call more than one!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
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
        throwException(ERR_GENERIC_PARAMETER, token, \
          "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),format[i],token->str);
      }
    }else{
      if(i == 9 && token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(token->str,symbolChar[5]) == 0){
          checkAndSkipCommentLine(fileTokenizer);
          i++;
        }
        else{
          throwException(ERR_GENERIC_PARAMETER,token, \
            "Error on line: %d. Expect Null or CommentLine but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
      }else{
        throwException(ERR_GENERIC_PARAMETER, token, \
          "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),format[i],token->str);
      }
    }
    freeToken(token);
  }

  bsinfo->packageName =  genericParameter;
}

// Called by handleComponentNameDesc and handleUseStatementDesc function to obtain the string
char *getString(FileTokenizer *fileTokenizer, char *strArr[], int *tokenType, int errorCode, int length, int type){
  Token *token;
  int i = 0;
  char *str;

  while(i < length){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(token->type == TOKEN_IDENTIFIER_TYPE){
        if(strArr[i] == NULL){  //store string when strArr is NULL
          if(checkVHDLidentifier(token->str) == 0){ //check VHDL identifier is  correct or not
            throwException(errorCode,token, \
              "Error on line: %d. Invalid VHDL identifier!!",getCorrectReadLineNo(fileTokenizer->readLineNo,token));
          }
          if (type == 1){
            if(checkStandardPackageName(token->str) == 0){
              throwException(errorCode,token, \
                "Error on line: %d. %s is not valid",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
            }
          }

          str = (char *)malloc(strlen(token->str));
          strcpy(str,(token->str));
        //compare token->str with strArr[i]. Throw error when not same, else continue
        }else if(strcmp(strArr[i],token->str) != 0){
          throwException(errorCode,token, \
            "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
        }
      //check for the operator type character
    }else if(token->type == TOKEN_STRING_TYPE){
      if (type == 1){
        if(checkStandardPackageName(token->str) == 0){
          throwException(errorCode,token, \
            "Error on line: %d. %s is not a valid component conformance name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
      }else{
        throwException(errorCode,token, \
          "Error on line: %d. %s is not a valid component conformance name\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }

      str = (char *)malloc(strlen(token->str));
      strcpy(str,(token->str));
    }else if(token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(strArr[i],token->str) != 0){
          throwException(errorCode,token, \
            "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
        }
      }
    }else{
      //check for comment line. yes = skipline, no = throw error
      if(i == (length - 1) && token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(token->str,symbolChar[5]) == 0){
          checkAndSkipCommentLine(fileTokenizer);
          i++;
        }
        else{
          throwException(errorCode,token, \
            "Error on line: %d. Expect Null or CommentLine but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
      }
      else{
        // Print error for modelname, packageName , etc...
        if(strArr[i] == NULL && i < length){
          if(tokenType[i] == TOKEN_STRING_TYPE){
            throwException(errorCode,token, \
              "Error on line: %d. Expect string type COMPONENT_CONFORMANCE but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }else{
            throwException(errorCode,token, \
              "Error on line: %d. Expect VHDL Identifier but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }
        }
        throwException(errorCode,token, \
          "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
      }
    }

    i++;
    freeToken(token);
  }
  return str;
}

// Use for Instruction Length and Boundary Length
// TYPE=> 0: instruction length, 1: boundary length
// Format: attribute <attribute name> of <component name> : entity is <value>;
int handleInstructionAndBoundaryLength(FileTokenizer *fileTokenizer,int errorCode, char *compName, int type){
  Token *token;
  char *strArr[8] = {"of",compName,":","entity","is",NULL,";",NULL};
  int tokenType[8] = {8,8,4,8,8,3,4,1};
  int length = 8;
  int i = 0;
  int value = -1;

  while(i < length){
    token = getTokenFromFile(fileTokenizer);
    if(tokenType[i] == token->type){
      if(token->type == TOKEN_INTEGER_TYPE && strArr[i] == NULL){
        value = ((IntegerToken*)token)->value;  //get the value
        if(type == 0){
          if(value < 2){
            throwException(errorCode,token, \
              "Error on line: %d. Expect greater than or equal to 2 but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }
        }else if(type == 1){
          if(value < 1){
            throwException(errorCode,token, \
              "Error on line: %d. Expect greater than 0 but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }
        }else{
          throwException(errorCode,token, \
            "Error on line: %d. Expect 0 or 1 but is %d.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),type);
        }
      // compare all the string except for the NULL type token
      }else if(token->type != TOKEN_NULL_TYPE){
        if(strcmp(strArr[i],token->str) != 0){
          throwException(errorCode,token, \
            "Error on line: %d. Expect %s but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
        }
      }

    }else{
      //check for comment line. If comment line, skipline. Else throw error
      if(i == (length - 1) && token->type == TOKEN_OPERATOR_TYPE){
        if(strcmp(token->str,symbolChar[5]) == 0){
          checkAndSkipCommentLine(fileTokenizer);
          i++;
        }
        else{
          throwException(errorCode,token, \
            "Error on line: %d. Expect Null or CommentLine but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
      }
      else{ //throw error
        if(tokenType[i] == 3){
          if(type == 0){
            throwException(errorCode,token, \
              "Error on line: %d. Expect Instruction Length but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }else{
            throwException(errorCode,token, \
              "Error on line: %d. Expect Boundary Length but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
          }
        }
        throwException(errorCode,token, \
          "Error on line: %d. Expect %s but is %s.",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
      }
    }

    i++;
    freeToken(token);
  }
  return value;
}


// Store the current token into tokenizer->current.
// When the getToken function is call, this token will taken out.
void createCallBackToken(Tokenizer *tokenizer, Token *token){
  char *str;

  tokenizer->currentToken = malloc(sizeof(Token));
  if(token->str == NULL){
    tokenizer->currentToken->str = NULL;
  }else{
    str = malloc(sizeof(char) * strlen(token->str));
    strcpy(str,token->str);
    tokenizer->currentToken->str = str;
  }
  tokenizer->currentToken->type = token->type;
  tokenizer->currentToken->originalStr = token->originalStr;
  tokenizer->currentToken->startColumn = token->startColumn;
  tokenizer->currentToken->length = token->length;
  tokenizer->callBackTokenFlag = 1;
}

// Check for the comment line and skip it
// If it is not meet the requirement for the comment line, throw exception
void checkAndSkipCommentLine(FileTokenizer *fileTokenizer){
  Token *token;
  token = getTokenFromFile(fileTokenizer);
  if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(token->str,symbolChar[5]) == 0){  //45 in ASCII is '-'
      freeToken(token);
      skipLine(fileTokenizer);
    }else{
      throwException(ERR_INVALID_COMMEND_LINE,token, \
        "Error on line: %d. Expect '-' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }
  }else{
    throwException(ERR_INVALID_COMMEND_LINE,token, \
      "Error on line: %d .Expect '-' symbol but is %s",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }
}

// Initialise the bsinfo struct
BSinfo *initBSinfo(){
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  bsinfo->modelName = "";
  bsinfo->packageName = "";
  bsinfo->port = listInit();
  bsinfo->pinMapping = listInit();
  bsinfo->useStatement = "";
  bsinfo->componentConformance = "";
  bsinfo->instructionLength = -1;
  bsinfo->boundaryLength = -1;
  bsinfo->tapScanClk = tapScanObjInit();
  bsinfo->tapScanIn = tapScanObjInit();
  bsinfo->tapScanMode = tapScanObjInit();
  bsinfo->tapScanOut = tapScanObjInit();
  bsinfo->tapScanReset = tapScanObjInit();
  bsinfo->boundaryReg = listInit();
  return bsinfo;
}

// Initialise the tapScanClock struct
tapScanObj *tapScanObjInit(){
  tapScanObj *tapScanC;
  tapScanC = (tapScanObj*)malloc(sizeof(tapScanObj));
  tapScanC->portId = "";
  tapScanC->haltState = "";
  tapScanC->clock = "";
  tapScanC->type = -1;
  return tapScanC;
}

// Check existing of file
// Not exists = 0 , Exists = 1
int checkFileExists(char *file_name){
  // F_OK use to test for existence of file.
  if( access( file_name, F_OK ) != -1 ) {
    return 1;
  }
  else{
    return 0;
  }
}

// Check the package name is same with the string or not
// True = 1, False = 0
int checkStandardPackageName(char *str){
  int i = 0;
  while(standardPackageName[i] != NULL){
    if(strcmp(standardPackageName[i], str) == 0){
      return 1;
    }
    i++;
  }

  return 0;
}

// Check for the VHDL identifier format
// Pass = 1, Fail = 0
int checkVHDLidentifier(char *str){
  int strLength = strlen(str);
  int underScoreFlag = 0;
  int i = 0;
  char *underscore;
  underscore = "_";

  if (str[strLength-1] == 95){  //95 => in ASCII is '_' symbol
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

void freeBsInfo(void *bsinfo){
  if(bsinfo){
    freeLinkedList(((BSinfo*)bsinfo)->port);
    freeLinkedList(((BSinfo*)bsinfo)->pinMapping);
    freeLinkedList(((BSinfo*)bsinfo)->boundaryReg);
    free(bsinfo);
  }
}

void freeFileTokenizer(void *fileTokenizer) {
  freeTokenizer(((FileTokenizer*)fileTokenizer)->tokenizer);
  if(fileTokenizer) {
    free(fileTokenizer);
  }
}

void freeLinkedList(void *linkedlist){
  if(linkedlist){
    free(linkedlist);
  }
}
