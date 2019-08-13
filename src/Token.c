#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "Token.h"
#include "Common.h"
#include "Tokenizer.h"

Token *createNullToken(int startColumn, char *originalString) {
  Token *token = malloc(sizeof(Token));

  token->type = TOKEN_NULL_TYPE;
  token->str = NULL;
  token->originalStr = originalString;
  token->startColumn = startColumn;
  token->length = 1;

  return token;
}

Token *createEndOfFileToken(int startColumn, char *originalString) {
  Token *token = malloc(sizeof(Token));

  token->type = TOKEN_EOF_TYPE;
  token->str = NULL;
  token->originalStr = originalString;
  token->startColumn = startColumn;
  token->length = 1;

  return token;
}

Token *createIntegerToken(int startColumn, int length, char *originalString, char *str, int value) {
  IntegerToken *token = malloc(sizeof(IntegerToken));

  token->type = TOKEN_INTEGER_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = str;
  token->value = value;

  return (Token *)token;
}

Token *createFloatToken(int startColumn, int length, char *originalString, char *str, double value) {
  FloatToken *token = malloc(sizeof(FloatToken));

  token->type = TOKEN_FLOAT_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = str;
  token->value = value;

  return (Token *)token;
}

Token *createIdentifierToken(int startColumn, int length, char *originalString, char *str) {
  IdentifierToken *token = malloc(sizeof(IdentifierToken));

  token->type = TOKEN_IDENTIFIER_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = str;
  return (Token *)token;
}

Token *createStringToken(int startColumn, int length, char *originalString, char *str) {
  StringToken *token = malloc(sizeof(StringToken));

  token->type = TOKEN_STRING_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = str;

  return (Token *)token;
}

Token *createCharToken(int startColumn, int length, char *originalString, char *str) {
  CharConstToken *token = malloc(sizeof(CharConstToken));

  token->type = TOKEN_CHAR_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = str;

  return (Token *)token;
}

Token *createOperatorToken(int startColumn, int length, char *originalString, char *symbol) {
  OperatorToken *token = malloc(sizeof(OperatorToken));

  token->type = TOKEN_OPERATOR_TYPE;
  token->startColumn = startColumn;
  token->length = length;
  token->originalStr = originalString;
  token->str = symbol;

  return (Token *)token;
}

Token *createInvalidToken(char *originalString, int start, int len) {
  char *endPtr;
  endPtr = NULL;
  Token *token = malloc(sizeof(Token));

  token->type = TOKEN_INVALID_TYPE;
  token->originalStr = originalString;
  endPtr = skipNonWhiteSpaces(&originalString[start + len]);
  len = (int)(endPtr - originalString) - start;
  token->str = createSubstring(originalString, start, len);
  token->startColumn = start;
  token->length = len;
  return token;
}

void freeTokenizer(void *tokenizer) {
  if(tokenizer) {
    free(tokenizer);
  }
}

void freeToken(void *token) {
  if(token) {
    if(((Token *)token)->str)
      free(((Token *)token)->str);
    free(token);
  }
}

void dumpTokenErrorMessage(CEXCEPTION_T ex) {
  Token *token = (Token *)ex->data;
  int i = token->length - 1;
  if(i < 0) i = 0;

  printf("%s\n", ex->msg);
  printf("%s\n", token->originalStr);
  printf("%*s", token->startColumn + 1, "^");
  while(i--)
    printf("~");
  putchar('\n');
}

int getCorrectReadLineNo(int linenumber, Token *token){
  if(token->type != TOKEN_NULL_TYPE){
    return linenumber;
  }else{
    if(linenumber == 1){
      return 1;
    }else{
      return linenumber - 1;
    }
  }
}
