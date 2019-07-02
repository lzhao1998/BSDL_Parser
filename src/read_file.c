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
char* check_file_exists(char *file_name){
  // F_OK use to test for existence of file.
  if( access( file_name, F_OK ) != -1 ) {
    return "File Exists";
  }
  else{
    return NULL;
  }
}

//read file
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
     while (fgets(line, sizeof(line), file_read) != NULL)
     {
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

char *getPackageName(char *filename){
  FILE *file_read;
  if (check_file_exists(filename) == NULL){
    return NULL;}

  file_read = fopen(filename, "r");

  if (file_read == NULL){
    return NULL;}
  else{
    char line [ 256 ];
    Token *token;
    Tokenizer *tokenizer;
    int maxSize = 8;
    //maxSize =7, maxLength = 20
    char array[9][20] = {"(","PHYSICAL_PIN_MAP",":","string",":","=","NULL",")",";"};
    // 4 = operator, 8 = identifier, 1 = NULL
    int tokenType[] = {4,8,4,8,4,4,6,4,4};
    int i = 0;
    while (fgets(line, sizeof(line), file_read) != NULL){
      tokenizer = initTokenizer(line);
      token = getToken(tokenizer);

      //token->type = 8 = identifier
      if (token->type == 8){
        char *temp1 = (char *)malloc(strlen(token->str));
        strcpy(temp1,(token->str));
        if(strcmp(temp1, "generic") == 0){
          freeToken(token);
          token = getToken(tokenizer);

          //When "generic" found, keep loop and check for the tokenType and array is same
          //with the line or not
          while (token->type != 1 && i != maxSize){
            if ((i == 6) && (token->type == tokenType[i])){
              char *packageName = (char *)malloc(strlen(token->str));
              strcpy(packageName,(token->str));
              freeToken(token);
              token = getToken(tokenizer);
              char *temp2 = (char *)malloc(strlen(token->str));
              strcpy(temp2,(token->str));
              i++;
              freeToken(token);
              token = getToken(tokenizer);
              //check ")"
              if((strcmp(temp2,array[i])==0) && (token->type == tokenType[i])){
                i++;
                char *temp3 = (char *)malloc(strlen(token->str));
                strcpy(temp3,(token->str));
                if(strcmp(temp3,array[i])==0){ // check ";"
                  freeToken(token);
                  token = getToken(tokenizer);
                  if(token->type == 1){ //if null
                    freeToken(token);
                    freeTokenizer(tokenizer);
                    fclose (file_read);
                    return packageName;
                  }
                }
              }
            }else if((token->type == tokenType[i]) ){
              char *temp3 = (char *)malloc(strlen(token->str));
              strcpy(temp3,(token->str));
              if (strcmp(temp3,array[i]) != 0){ //if not same...
                i = 0;
                break;
              }
            }else if(token->type != tokenType[i]){
              i = 0;
              break;
            }
            freeToken(token);
            token = getToken(tokenizer);
            i++;
          }
          i = 0;
        }
      }
      freeToken(token);
      freeTokenizer(tokenizer);
    }
    fclose (file_read); //close file
    return NULL;  //not found
  }
}
char* getUseStatement(char *filename){
  FILE *file_read;

  if (check_file_exists(filename) == NULL){
    return NULL;}
  file_read = fopen(filename, "r"); // r = read mode, w = write mode

  if (file_read == NULL){
    return NULL;}
  else{
    // set the size of the line
    char line [ 256 ];
    Token *token;
    Tokenizer *tokenizer;
    int maxsize = 6;
    char array[6][10] = {"use","NULL",".","all",";","null"};
    int tokenType[6] = {8,8,4,8,4,1};
    int i = 0;
    while (fgets(line, sizeof(line), file_read) != NULL){
      tokenizer = initTokenizer(line);
      token = getToken(tokenizer);
      if (token->type == tokenType[i]){  //check 'use' in front of each line
        char *temp1 = (char *)malloc(strlen(token->str));
        strcpy(temp1,(token->str));
        if(strcmp(temp1,array[i]) == 0){
          i++;
          freeToken(token);
          token = getToken(tokenizer);
          if(token->type == tokenType[i]){  // check the type of 2nd word / useStatement
            char *useStatement = (char *)malloc(strlen(token->str));
            strcpy(useStatement,(token->str));
            i++;
            while(i < maxsize){
              freeToken(token);
              token = getToken(tokenizer);
              if((token->type == tokenType[i])){ //check the rest whether is it in correct order
                if((i != 5) && (strcmp(token->str,array[i])==0)){ //to prevent bad memory access for null type
                  i++;
                }else if(i == 5){
                  i++;
                }else{
                  i = 0;
                  break;
                }
              }else if((i == 5) && (token->type == tokenType[i])){
                i++;
              }else{
                i = 0;
                break;
              }
            }
            if (i == maxsize){
              i = 0;
              freeToken(token);
              freeTokenizer(tokenizer);
              fclose (file_read);
              return useStatement;
            }
          }
        }
      }
      freeToken(token);
      freeTokenizer(tokenizer);
      i = 0;
    }
    fclose (file_read); //close file
    return NULL;  //not found
  }
}

char *obtainComponentNameFromLine(char *str){
  Token *token;
  Tokenizer *tokenizer;
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
