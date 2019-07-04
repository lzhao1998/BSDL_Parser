#include "read_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "unity.h"
#include "Common.h"
#include "Token.h"
#include "Error.h"
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

    char line[512];
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

  //tokenizer is null, return invalid token due to it
  //is reach End of File
  if (fileTokenizer->tokenizer == NULL){
    token = createInvalidToken("EndOfFile",0,0);
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
      printf("hdere\n");
      if (i > fileTokenizer->readLineNo){
        printf("readLineNo is %d\n",fileTokenizer->readLineNo);
        break;
      }
    }
    fileTokenizer->readLineNo++;

    printf("line is %s\n",line);
    printf("strlen is %d\n",strlen(line));

    if (strlen(line) != 0){
      fileTokenizer->tokenizer = initTokenizer(line);
    }else{
      printf("here in\n");
      //if(fileTokenizer->tokenizer){
        printf("zz\n");
      //}
      fileTokenizer->tokenizer = initTokenizer(line);
      //freeTokenizer(fileTokenizer->tokenizer);
    }
  }
  return token;
}


//test for read file only
char* read_file(char *file_name){
  //FILE is an object type suitable for storing information for a file stream.
   FILE *file_read;

   // r = read mode, w = write mode
   file_read = fopen(file_name, "r");

   if (file_read == NULL){
     return NULL;
   }
   else{
     //just temporary set the size of the line
     char line [ 128 ];
     int i = 0;
     //read each line and display
     //fgets use to read the file until it meet '\n' , NULL or EOF
     while (fgets(line, sizeof(line), file_read) != NULL){
       //display line
       if(i == 10){
         continue;
       }
       fputs (line, stdout);
       i++;
     }
     fclose (file_read); //close file
     return "Not empty";
   }
}
BSinfo *getBSinfo(char *filename){
  FILE *file_read;

  //Check for existing of file
  //If not exists, throw exception
  if(checkFileExists(filename)==1){
    file_read = fopen(filename,"r");
  }else{
    throwException(ERR_FILE_NOT_EXISTS, NULL, "ERROR!! FILE DOES NOT EXISTS!!");
  }

  //Check for file read wether it can read or not and the file is not empty
  if (file_read == NULL){
    throwException(ERR_FILE_INVALID, NULL, "ERROR!! FILE CANT READ!!");
  }

  char line [ 256 ];
  Token *token;
  Tokenizer *tokenizer;
  BSinfo *info = NULL;
  info = (BSinfo*)malloc(sizeof(BSinfo));
  char *header[]={"entity","generic","port","use","attribute","end"};
  while(fgets(line,sizeof(line),file_read) != NULL){
    if (isCommentLine(line) == 1){
      continue;
    }

    tokenizer = initTokenizer(line);
    token = getToken(tokenizer);
    if(token->type == TOKEN_NULL_TYPE){
      freeToken(token);
      freeTokenizer(tokenizer);
      continue;
    }else if(token->type == TOKEN_IDENTIFIER_TYPE){
      if(strcmp(token->str,header[0]) == 0){
        info->modelName = obtainComponentNameFromLine(line);
      }
    }

    freeToken(token);
    freeTokenizer(tokenizer);
  } //(while loop for read)

  return info;
}

char *obtainComponentNameFromLine(char *str){
  Token *token;
  Tokenizer *tokenizer;
  //Format for entity to get component name
  char *format[4] = {"entity","componentName","is","NULL"};
  int tokenType[4] = {8,8,8,1}; //8->identifier token, 1->NULL token
  int i = 0;
  char *compName = "None";

  tokenizer = initTokenizer(str);
  token = getToken(tokenizer);
  while(i < 4){
    // if same token type is same
    if((token->type == tokenType[i])){
      if(i == 3){ //if is NULL token at last
        i++;
        //get component name and store it
      }else if(i == 1){
        compName = (char *)malloc(strlen(token->str));
        strcpy(compName,(token->str));
        i++;
        // compare the string with the format
      }else if(strcmp(token->str,format[i]) == 0){
        i++;
        // string is not same, throw error
      }else{
        throwException(ERR_COMPONENT_NAME, token, "ERROR!! INVALID ENTITY FORMAT");
      }
      freeToken(token);
      token = getToken(tokenizer);
  // if token type or format is not the same, throw error
  }else{
      throwException(ERR_COMPONENT_NAME, token, "ERROR!! INVALID ENTITY FORMAT");
    }
  }

  freeToken(token);
  freeTokenizer(tokenizer);
  return compName;
}

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
