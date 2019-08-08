#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <error.h>
#include <ctype.h>
#include "Common.h"
#include "Error.h"
#include "Tokenizer.h"
#include "CException.h"
#include "Exception.h"

/**
*  Initialise the tokenizer....
*  Copy the stringToTokenize to tokenizer->string
*  Put tokenizer->index to 0 (for start counting)
**/
Tokenizer *initTokenizer(char *stringToTokenize) {
  Tokenizer *token = NULL;
  token = (Tokenizer*)malloc(sizeof(Tokenizer));
  token->str = stringToTokenize;
  token->index = 0;
  token->callBackTokenFlag = 0;
  token->currentToken = NULL;
  return (Tokenizer*)token;
}

Token *getToken(Tokenizer *tokenizer) {
  int i = 0, startColumn = 0, length = 0, value = 0;
  double floatValue = 0;
  Token *token = NULL;
  token = (Token*)malloc(sizeof(Token));
  /**
  *   If the current character is NULL, create NULL token
  *   Else skip it
  **/
  if(tokenizer->str[tokenizer->index] == '\0')
  {
    token = createNullToken(tokenizer->index, tokenizer->str);
    return token;
  }
  /**skip the unwanted space**/
  while(tokenizer->str[tokenizer->index] == ' ' || tokenizer->str[tokenizer->index] == '\t')
  {
    tokenizer->index++;
  }

  //  After skip space and current character become NULL, create NULL token
  if(tokenizer->str[tokenizer->index] == '\0')
  {
    token = createNullToken(tokenizer->index, tokenizer->str);
  }
  else if(tokenizer->str[tokenizer->index] == '\"') //if detect " , check it is string or invalid token
  {
    /**
    *  tokenizer->index + 1 because to skip the "
    *  Current tokenizer->index is the startColumn of this token
    *
    *  While it is not hitting another ", keep checking
    *  Else exit loop
    *  If is still not hitting " but hit NULL, throw error
    **/
    tokenizer->index++;
    startColumn = tokenizer->index;
    while (tokenizer->str[tokenizer->index] != '\"') {
      if(tokenizer->str[tokenizer->index] == '\0')
      {
        length = i;
        /**
        * ONLY FOR CREATING INVALID TOKEN
        * startColumn - 1 is because needed include " sign
        * length + 1 is because needed include " sign
        **/
        token = createInvalidToken(tokenizer->str, startColumn-1, length+1);
        throwException(ERR_INVALID_STRING, token, "ERROR!! INVALID TOKEN");
      }
      tokenizer->index++;
      i++;
    }
    /**
    *  Length of string is same as i because i is starting from 0 and length of string is not included "
    *  Malloc temp to the size of the needed string
    *  Copy needed string to temp
    *  If reached end of needed string, insert NULL after end of string
    *  Create String token
    **/
    char *temp = (char*)malloc((i+1) * sizeof (char));	//get the length of the string and last is for NULL/end of string
    tokenizer->index++;
    length = i;
    for(int k = 0; k < (i+1); k = k + 1)
    {
      if(k == i)
      {
        temp[k] = '\0';
      }
      else
      {
        temp[k] = tokenizer->str[startColumn + k];
      }
    }
    token = createStringToken(startColumn, length, tokenizer->str, temp);
  }
  else if(tokenizer->str[tokenizer->index] == '\'')//if detect ' , check it is string, character or invalid token
  {
    /**
    *  tokenizer->index + 1 because to skip the '
    *  Current tokenizer->index is the startColumn of this token
    *
    *  While it is not hitting another ', keep checking
    *  Else exit loop
    *  If is still not hitting " but hit NULL, throw error
    **/
    tokenizer->index++;
    startColumn = tokenizer->index;
    while (tokenizer->str[tokenizer->index] != '\'') {
      if(tokenizer->str[tokenizer->index] == '\0')
      {
        length = i;
        /**
        * ONLY FOR CREATING INVALID TOKEN
        * startColumn - 1 is because needed include " sign
        * length + 1 is because needed include " sign
        **/
        token = createInvalidToken(tokenizer->str, startColumn-1, length+1);
        throwException(ERR_INVALID_STRING, token, "ERROR!! INVALID STRING TOKEN");
      }
      tokenizer->index++;
      i++;
    }
    /**
    *  Length of string is same as i because i is starting from 0 and length of string is not included "
    *  Malloc temp to the size of the needed string
    *  Copy needed string to temp
    *  If reached end of needed string, insert NULL after end of string
    *
    *  If the length is 1 then create Character token
    *  Else if the length is 0, throw error
    *  Else create String token
    *
    * FOR CHARACTER TOKEN
    * If the character is not alphabet. throw error
    * Else throw error
    **/
    char *temp = (char*)malloc((i+1) * sizeof (char));	//get the length of the string and last is for NULL/end of string
    tokenizer->index++;
    length = i;
    if(length == 1)
    {
      if(isalpha(tokenizer->str[startColumn]) != 0){
        temp[0] = tokenizer->str[startColumn];
        temp[1] = '\0';
      }
      else{
        token = createInvalidToken(tokenizer->str, startColumn-1, length+1);
        throwException(ERR_INVALID_CHAR, token, "ERROR!! INVALID CHAR TOKEN");
      }
      token = createCharToken(startColumn, length, tokenizer->str, temp);
    }
    else if(length == 0)
    {
      token = createInvalidToken(tokenizer->str, startColumn-1, length+1);
      throwException(ERR_INVALID_CHAR, token, "ERROR!! INVALID CHAR TOKEN");
    }
    else
    {
      for(int k = 0; k < (i+1); k = k + 1)
      {
        if(k == i)
        {
          temp[k] = '\0';
        }
        else
        {
          temp[k] = tokenizer->str[startColumn + k];
        }
      }
      token = createStringToken(startColumn, length, tokenizer->str, temp);
    }
  }
  else if(isdigit(tokenizer->str[tokenizer->index]) != 0)   //if detect digit, check wether is integer, floating point or invalid token
  {
    /**
    *   Current tokenizer->index is the startColumn of this token
    *   i = 1 is for the digit
    *
    *   When there is hit the space or NULL, exit looping
    *   Else keep checking until it hit space or NULL
    **/
    startColumn = tokenizer->index;
    i = 1;
    while(isspace(tokenizer->str[tokenizer->index]) == 0 && tokenizer->str[tokenizer->index] != '\0' \
          && ispunct(tokenizer->str[tokenizer->index]) == 0 || tokenizer->str[tokenizer->index] == '.' \
        || tokenizer->str[tokenizer->index] == '-' || tokenizer->str[tokenizer->index] == '+') {
      tokenizer->index++;
      i++;
    }
    /**
    *  Because i start from 0
    *  For i = 1, length = 1  and For i > 1, length = i - 1; because i start from 0
    *
    *  Malloc the temp to the size same as the string needed
    *  Copy needed string to the temp;
    *  If reached end of needed string, insert NULL after end of string
    **/
    char *temp = (char*)malloc((i) * sizeof (char));
    if(i == 1) {length = i;}
    else{length = i - 1;}
    for(int k = 0; k < i; k++)
    {
      if(k == i - 1){
        temp[k] = '\0';
      }
      else{
        temp[k] = tokenizer->str[startColumn + k];
      }
    }
    //check the temp is integer or float
    if(checkFor0x(temp) == 1){
      //if it is hex create Integer token, else throw error
      if(checkForHexDigit(temp+2) == 1){
        //use sscanf to store the hex value into integer
        sscanf(temp,"%x",&value);
        token = createIntegerToken(startColumn, length, tokenizer->str, temp, value);
      }
      else{
      length = i;
      token = createInvalidToken(tokenizer->str, startColumn, length);
      throwException(ERR_INVALID_INTEGER, token, "ERROR!! INVALID INTEGER TOKEN(HEX)");
      }
    }
    //when it is integer value, create Integer token
     else if(checkforInt(temp) == 1){
       //use atoi to convert int(string) to int
       //check for octal and decimal
       if(temp[0] == '0')
       {
         if(checkForOctal(temp) == 1)
         {
           sscanf(temp,"%o",&value);
           token = createIntegerToken(startColumn, length, tokenizer->str, temp, value);
         }
         else
         {
           token = createInvalidToken(tokenizer->str, startColumn, length);
           throwException(ERR_INVALID_INTEGER, token, "ERROR!! INVALID INTEGER TOKEN(OCT)");
         }
       }
       else
       {
         sscanf(temp,"%d",&value);
         token = createIntegerToken(startColumn, length, tokenizer->str, temp, value);
       }
    }
    else{
      //use strtod(string to double) to convert float(string) to floating value
      char *endPtr;
      floatValue = strtod(temp, &endPtr);
      //if endPointer(endPtr) is NULL, which means converting float(string) to float is success create Float token
      //else throw error
      if(endPtr[0] == '\0'){
       token = createFloatToken(startColumn, length, tokenizer->str, temp, floatValue);
      }
      else{
        length = i;
        token = createInvalidToken(tokenizer->str, startColumn, length);
        throwException(ERR_INVALID_FLOAT, token, "ERROR!! INVALID INTEGER TOKEN(FLOAT)");
      }
    }
  }
  else if(isalpha(tokenizer->str[tokenizer->index]) != 0 || tokenizer->str[tokenizer->index] == '_') //if detect _ or alpha, check is identifier or not
  {
    /**
    *   Current tokenizer->index is the startColumn of this token
    *   i = 1 is for the first character/underscore
    *
    *   When there is hit the space or NULL or symbol(except underscore), exit looping
    *   Else keep checking until it hit space or NULL
    **/
    startColumn = tokenizer->index;
    i = 1;
    while((isspace(tokenizer->str[tokenizer->index]) == 0 && tokenizer->str[tokenizer->index] != '\0') \
          && (isalnum(tokenizer->str[tokenizer->index]) != 0 || tokenizer->str[tokenizer->index] == '_')) {
      tokenizer->index++;
      i++;
    }
    /**
    *  Because i start from 0
    *  For i = 1, length = 1  and For i > 1, length = i - 1; because i start from 0
    *
    *  Malloc the temp to the size same as the string needed
    *  Copy needed string to the temp;
    *  If reached end of needed string, insert NULL after end of string
    **/
    char *temp = (char*)malloc((i) * sizeof (char));
    length = i - 1;     //length = i , because i start from 0 so length no need to equal i - 1
    for(int k = 0; k < i; k = k + 1)
    {
      if(k == (i-1)){
        temp[k] = '\0';
      }
      else{
        temp[k] = tokenizer->str[startColumn + k];
      }
    }
    token = createIdentifierToken(startColumn, length, tokenizer->str, temp);
    //freeString(temp);
  }
  else if(ispunct(tokenizer->str[tokenizer->index]))   //if detect punctuation character, check wether is the operator or not
  {
    /**
    *   Current tokenizer->index is the startColumn of this token
    *   i = 1 is for the first symbol
    *
    *   Length = 1;
    *   Malloc the temp size same as symbol
    *   Copy needed symbol to temp
    *   Create Identifier token
    **/
    startColumn = tokenizer->index;
    i = 1;
    tokenizer->index++;
    char *temp = (char*)malloc((i+1) * sizeof (char));	//get the length of the string and last is for NULL/end of string
    length = i;
    temp[0] = tokenizer->str[startColumn];
    temp[1] = '\0';
    token = createOperatorToken(startColumn, length, tokenizer->str, temp);
  }
  else{
    token = createNullToken(tokenizer->index, tokenizer->str);
  }
  return token;
}

void freeString(void *str){
  if(str){
    free(str);
  }
}

// check for '0x'
int checkFor0x(char *str)
{
  for(int i = 0; i < 2; i++){
    if(i == 0){
      if(str[i] != '0'){return 0;}
    }
    if(i == 1){
        if(str[i] != 'x'){return 0;}
    }
  }
  return 1;
}

// check for hexa value in the string
int checkForHexDigit(char *str)
{
  int j = strlen(str);
  for(int i = 0; i < j; i++){
    if(isxdigit(str[i]) == 0){
      return 0;
    }
  }
  return 1;
}

// check for integer value
int checkforInt(char *str)
{
  int j = strlen(str);
  for(int i = 0; i < j; i++){
    if(isdigit(str[i]) == 0){
      return 0;
    }
  }
  return 1;
}

int checkForOctal(char *str)
{
  int j = strlen(str);
  int k;
  for(int i = 0; i < j; i++){
    k = str[i] - '0';
    if(k > 7){
      return 0;
    }
  }
  return 1;
}
