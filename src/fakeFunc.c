#include "fakeFunc.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"
#include "cmock.h"
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"

char *stringArray[50];

FileTokenizer *fake_createFileTokenizer(char *filename, int count){
  FileTokenizer *fileTokenizer;
  fileTokenizer = (FileTokenizer*)malloc(sizeof(FileTokenizer));
  //init tokenizer how to get string
  fileTokenizer->tokenizer = initTokenizer(stringArray[0]);
  fileTokenizer->filename = filename;
  fileTokenizer->readLineNo = 1;
  return fileTokenizer;
}

Token *fake_getTokenFromFile(FileTokenizer *fileTokenizer, int count){
  Token *token;

  token = getToken(fileTokenizer->tokenizer);
  //token->type is NULL, then replace the tokenizer with the next line
  //If next line is EOF, tokenizer = NULL to signal that it reach EOF next getToken
  if(token->type == TOKEN_NULL_TYPE){
    freeTokenizer(fileTokenizer->tokenizer);
    if(stringArray[fileTokenizer->readLineNo] != NULL){
      fileTokenizer->tokenizer = initTokenizer(stringArray[fileTokenizer->readLineNo]);
    }else{
      fileTokenizer->tokenizer = initTokenizer(NULL);
    }
    fileTokenizer->readLineNo++;
  }

  return token;
}

void fake_skipLine(FileTokenizer *fileTokenizer, int count){
  freeTokenizer(fileTokenizer->tokenizer);

  if (stringArray[fileTokenizer->readLineNo] != NULL){
    fileTokenizer->tokenizer = initTokenizer(stringArray[fileTokenizer->readLineNo]);
  }else{
    fileTokenizer->tokenizer = initTokenizer(NULL);
  }

  fileTokenizer->readLineNo++;
}

void putStringArray(char *strArr[]){
  int i = 0;
  while(strArr[i] != NULL && i < 50){
    stringArray[i] = strArr[i];
    i++;
  }
  stringArray[i] = NULL;
}
