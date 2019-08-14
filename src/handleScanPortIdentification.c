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
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "handleScanPortIdentification.h"

char *dashSymbol = "-";

char *tapScan[] = {
  "of",         // 0
  NULL,         // 1    PORT ID
  ":",          // 2
  "signal",     // 3
  "is",         // 4
  "true",       // 5
  ";"           // 6
};

int tapScanType[] = {8,8,4,8,8,8,4};

// Note : type is use to choose which scan port to use; (TDI,TMS,TDO,TRST)
//                                                        0   1   2   3
void handleScanPortDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer, int type){
  Token *token;
  char *result;
  int length = sizeof(tapScanType)/sizeof(tapScanType[0]);


  // check the type value and check for the scan port to ensure that is 1st declare at here
  if((type > 3) || (type < 0)){
    token = getTokenFromFile(fileTokenizer);
    throwException(ERR_INVALID_SCAN_PORT_TYPE,token, \
        "Invalid type when calling handleScanPortDesc function, type value : %d\n",type);
  }

  // obtain the correct error code so that it will be easy for printing out the message
  int errorVal;
  switch(type){
    case 0:
      errorVal = ERR_INVALID_TAP_SCAN_IN_FORMAT;
      break;
    case 1:
      errorVal = ERR_INVALID_TAP_SCAN_MODE_FORMAT;
      break;
    case 2:
      errorVal = ERR_INVALID_TAP_SCAN_OUT_FORMAT;
      break;
    case 3:
      errorVal = ERR_INVALID_TAP_SCAN_RESET_FORMAT;
      break;
    default:
      errorVal = 0;
      break;
  }

  //use while to checking the format and store the port ID into temp string array
  int i = 0;
  while(i < length){
    token = getTokenFromFile(fileTokenizer);
    if(token->type == tapScanType[i]){
      if(i == 1){
        if(checkVHDLidentifier(token->str) != 1){
          throwException(errorVal,token, \
            "Error on line: %d. %s is not a valid Port ID.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
        result = malloc(sizeof(char) * strlen(token->str));
        strcpy(result,token->str);
      }else if(strcmp(tapScan[i],token->str)!=0){
        throwException(errorVal,token, \
          "Error on line: %d. Expect %s but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),tapScan[i],token->str);
      }

    }else{
      throwException(errorVal,token, \
        "Error on line: %d. Expect %s but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),tapScan[i],token->str);
    }
    i++;
    freeToken(token);

  }

  // check the end of scan tap identification string is null of comment line, else throw error
  token = getTokenFromFile(fileTokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
    throwException(errorVal,token, \
      "Error on line: %d. Expect null or comment line but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(dashSymbol,token->str) == 0){
      checkAndSkipCommentLine(fileTokenizer);
    }else{
      throwException(errorVal,token, \
        "Error on line: %d. Expect null or comment line but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }
  }

  freeToken(token);
  if(type == 0){
    bsinfo->tapScanIn = result;
  }else if(type == 1){
    bsinfo->tapScanMode = result;
  }else if(type == 2){
    bsinfo->tapScanOut = result;
  }else if(type == 3){
    bsinfo->tapScanReset = result;
  }
}

void handleTapScanClockDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer){
  Token *token;
  char *id,*haltState,*clk;
  //                        portId                         clock      haltState
  //                 0        1     2      3     4    5      6    7     8        9   10
  char *strArr[]  = {"of","portID",":","signal","is","(","Clock",",","HaltState",")",";"};
  int tokenType[] = {8,8,4,8,8,4,3,4,8,4,4};
  int length = sizeof(tokenType)/sizeof(tokenType[0]);

  int i = 0;
  while(i < length){
    token = getTokenFromFile(fileTokenizer);
    if(i == 6){   // get the clock value
      if(token->type != TOKEN_INTEGER_TYPE && token->type != TOKEN_FLOAT_TYPE){
        throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
          "Error on line: %d. Expect integer or float value but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
      }else{
        clk = malloc(sizeof(char) * strlen(token->str));
        strcpy(clk,token->str);
      }
    }else if(tokenType[i] == token->type){
      if(i == 1){   //get the port id
        if(checkVHDLidentifier(token->str) != 1){
          throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
            "Error on line: %d. %s is not a valid Port Id.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }
        id = malloc(sizeof(char) * strlen(token->str));
        strcpy(id,token->str);
      }else if(i == 8){   //get the halt state
        if((strcmp("LOW",token->str)) != 0 && (strcmp("BOTH",token->str)!=0)){
          throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
            "Error on line: %d. Expect LOW or BOTH but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
        }else{
          haltState = malloc(sizeof(char) * strlen(token->str));
          strcpy(haltState,token->str);
        }
      }else if(strcmp(strArr[i],token->str)!= 0){    // compare the string
        throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
          "Error on line: %d. Expect %s but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
      }
    }else{
      throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
        "Error on line: %d. Expect %s but is %s\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),strArr[i],token->str);
    }

    i++;
    freeToken(token);
  }

  // check the end of scan tap identification string is null of comment line, else throw error
  token = getTokenFromFile(fileTokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_OPERATOR_TYPE){
    throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
      "Error on line: %d. Expect null or comment line but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
  }else if(token->type == TOKEN_OPERATOR_TYPE){
    if(strcmp(dashSymbol,token->str) == 0){
      checkAndSkipCommentLine(fileTokenizer);
    }else{
      throwException(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT,token, \
        "Error on line: %d. Expect null or comment line but is %s.\n",getCorrectReadLineNo(fileTokenizer->readLineNo,token),token->str);
    }
  }
  freeToken(token);

  bsinfo->tapScanClk->portId = id;
  bsinfo->tapScanClk->haltState = haltState;
  bsinfo->tapScanClk->clock = clk;
}
