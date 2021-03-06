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
#include "createAndGetTokenFromFile.h"
#include "handlePortDescription.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"

FileTokenizer *createFileTokenizer(char *filename){
  Token *token;
  token = createEndOfFileToken(0,NULL);
  FileTokenizer *fileTokenizer;
  fileTokenizer = (FileTokenizer*)malloc(sizeof(FileTokenizer));

  if (checkFileExists(filename) == 1){
    fileTokenizer->fileHandler = fopen(filename, "r");

    if(fileTokenizer->fileHandler == NULL){
      //throw error for cant open the file
      throwException(ERR_FILE_INVALID, token, "ERROR!! INVALID FILE!!");
    }

    char line[3000];
    fgets(line,sizeof(line),fileTokenizer->fileHandler);
    fileTokenizer->tokenizer = initTokenizer(line);
  }else{
    throwException(ERR_FILE_NOT_EXIST, token, "ERROR!! FILE DOES NOT EXISTS!!");
  }
  fileTokenizer->filename = filename;
  fileTokenizer->readLineNo = 1;
  return fileTokenizer;
}

Token *getTokenFromFile(FileTokenizer *fileTokenizer){
  Token *token;
  char line[3000];

  token = getToken(fileTokenizer->tokenizer);
  //token->type is NULL, then replace the tokenizer with the next line
  //If next line is EOF, tokenizer = NULL to signal that it reach EOF next getToken
  if(token->type == TOKEN_NULL_TYPE){
    fileTokenizer->tokenizer = NULL;
    if(fgets(line,sizeof(line),fileTokenizer->fileHandler) != NULL){
      fileTokenizer->tokenizer = initTokenizer(line);
    }else{
      fileTokenizer->tokenizer = initTokenizer(NULL);
    }
    fileTokenizer->readLineNo++;
  }

  return token;
}

void skipLine(FileTokenizer *fileTokenizer){
  char line[3000];
  freeTokenizer(fileTokenizer->tokenizer);

  if (fgets(line,sizeof(line),fileTokenizer->fileHandler) != NULL){
    fileTokenizer->tokenizer = initTokenizer(line);
  }else{
    fileTokenizer->tokenizer = initTokenizer(NULL);

  }

  fileTokenizer->readLineNo++;
}
