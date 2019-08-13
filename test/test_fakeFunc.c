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
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

void test_fakeFunc_test_getTokenFromFile_func(void){
  setupFake();
  char *string[] = {"entity STM32F469_F479_WLCSP168 is",NULL};
  Token *token;
  FileTokenizer *fileTokenizer;
  char *filename = "normal_name.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("entity",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("is",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE,token->type);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_EOF_TYPE,token->type);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}

void test_fakeFunc_test_get_mordelName(void){
  setupFake();
  char *string[] = {"entity STM32F469 is",NULL};
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "normal_name.txt";

  putStringArray(string);
  bsinfo = initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469",bsinfo->modelName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_fakeFunc_test_skipLine_function(void){
  setupFake();
  char *string[] = {"entity STM32F469 is","hello world",NULL};
  FileTokenizer *fileTokenizer;
  char *filename = "normal_name.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_EQUAL_STRING("entity STM32F469 is",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("hello world",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING(NULL,fileTokenizer->tokenizer->str);

  freeFileTokenizer(fileTokenizer);
}
