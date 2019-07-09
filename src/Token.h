#ifndef Token_H
#define Token_H

#include <stdint.h>
//#include "Common.h"
#include "CException.h"
#include "Exception.h"

typedef enum {
  TOKEN_UNKNOWN_TYPE,   //0
  TOKEN_NULL_TYPE,      //1
  TOKEN_INVALID_TYPE,   //2
  TOKEN_INTEGER_TYPE,   //3
  TOKEN_OPERATOR_TYPE,  //4
  TOKEN_FLOAT_TYPE,     //5
  TOKEN_STRING_TYPE,    //6
  TOKEN_CHAR_TYPE,      //7
  TOKEN_IDENTIFIER_TYPE,//8
  TOKEN_EOF_TYPE,       //9
} TokenType;

typedef enum {
  NO_AFFIX,
  PREFIX,
  INFIX,
  POSTFIX,
} Affix;

typedef enum {
  NO_ASSOCIATIVITY,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
} Associativity;

typedef struct {
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
} Token;

typedef struct {
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  int value;
} IntegerToken;

typedef struct {
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  double value;
} FloatToken;

typedef struct {
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
} IdentifierToken, StringToken, CharConstToken;

typedef struct {
  TokenType type;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  Token *token[0];
} OperatorToken;

Token *createNullToken();
Token *createEndOfFileToken();
Token *createInvalidToken(char *originalString, int start, int len);
Token *createIntegerToken(int startColumn, int length, char *originalString, char *str, int value);
Token *createFloatToken(int startColumn, int length, char *originalString, char *str, double value);
Token *createIdentifierToken(int startColumn, int length, char *originalString, char *str);
Token *createStringToken(int startColumn, int length, char *originalString, char *str);
Token *createCharToken(int startColumn, int length, char *originalString, char *str);
Token *createOperatorToken(int startColumn, int length, char *originalString, char *symbol);

void freeToken(void *token);
void freeTokenizer(void *tokenizer);
void dumpTokenErrorMessage(CEXCEPTION_T ex, int lineNo);

#endif // Token_H
