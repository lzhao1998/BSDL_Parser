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

char* getModelName(char *filename){
  FILE *file_read;

  if (check_file_exists(filename) == NULL){
    return NULL;
  }

  // r = read mode, w = write mode
  file_read = fopen(filename, "r");

  if (file_read == NULL){
    return NULL;
  }
  else{
    // set the size of the line
    char line [ 256 ];
    while (fgets(line, sizeof(line), file_read) != NULL)
    {
      Token *token;
      Tokenizer *tokenizer;
      tokenizer = initTokenizer(line);
      token = getToken(tokenizer);

      //token->type = 8 = identifier
      if (token->type == 8){
        char *temp1 = (char *)malloc(strlen(token->str));
        strcpy(temp1,(token->str));

        if(stringCompare(&temp1, "entity") == 1){
        //if (strcmp(temp1,item) == 1){
          freeToken(token);
          token = getToken(tokenizer);
          if (token->type == 8){
            char* packageName = (char *)malloc(strlen(token->str));
            strcpy(packageName,(token->str));
            freeToken(token);
            freeTokenizer(tokenizer);
            fclose (file_read);
            return packageName;
          }
        }
      }
      freeToken(token);
      freeTokenizer(tokenizer);
    }
    fclose (file_read); //close file
    return NULL;  //not found
  }
}

char *getPackageName(char *filename){
  FILE *file_read;

  if (check_file_exists(filename) == NULL){
    return NULL;
  }

  // r = read mode, w = write mode
  file_read = fopen(filename, "r");

  if (file_read == NULL){
    return NULL;
  }
  else{
    // set the size of the line
    char line [ 256 ];
    while (fgets(line, sizeof(line), file_read) != NULL)
    {
      Token *token;
      Tokenizer *tokenizer;
      tokenizer = initTokenizer(line);
      token = getToken(tokenizer);

      //token->type = 8 = identifier
      if (token->type == 8){
        char *temp1 = (char *)malloc(strlen(token->str));
        strcpy(temp1,(token->str));
        if(stringCompare(&temp1, "generic") == 1){
          freeToken(token);
          token = getToken(tokenizer);
          while (token->type != 1){ //when token not NULL type
            if (token->type == 6){  //when found string type token
              char *packageName = (char *)malloc(strlen(token->str));
              strcpy(packageName,(token->str));
              freeToken(token);
              freeTokenizer(tokenizer);
              fclose (file_read);
              return packageName;
            }
            freeToken(token);
            token = getToken(tokenizer);
          }
        }
      }
      freeToken(token);
      freeTokenizer(tokenizer);
    }
    fclose (file_read); //close file
    return NULL;  //not found
  }
}

int stringCompare(char **str1, char *str2)
{
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
