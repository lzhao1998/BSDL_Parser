#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "fakeFunc.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "getStrToken.h"
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

void test_getStrToken_get_quote_symbol_as_operator_token(void){
  setupFake();
  char *string[] = {" \" hello world \"",NULL};
  FileTokenizer *fileTokenizer;
  Token *token;
  char *filename = "file.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("\"",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hello",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("world",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("\"",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE,token->type);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}
