#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "handlePinMappingDesc.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handleScanPortIdentification.h"


void setUp(void){}
void tearDown(void){}

// Input: hello world
//test it when until eof
void test_getTokenFromFile_expect_return_EndOfFile_token_when_read_file_test_for_getTokenFromFile_file_unitl_end_of_file(void){
  char *filename = "test\\file_to_test\\file_test_for_getTokenFromFile.txt";
  FileTokenizer *fileTokenizer;
  Token *token;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hello",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("world",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_NULL_TYPE,token->type);
  freeToken(token);

  TEST_ASSERT_EQUAL(2,fileTokenizer->readLineNo);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_EOF_TYPE,token->type);
  freeToken(token);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

/* Input:
    hello world
    hi

    my name is
    asd
    asd
*/
void test_getTokenForMultipleLine(void){
  char *filename = "test\\file_to_test\\file_test_getToken.txt";
  FileTokenizer *fileTokenizer;
  Token *token;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);

  TEST_ASSERT_EQUAL(1,fileTokenizer->readLineNo);   //hello wrold
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL(2,fileTokenizer->readLineNo);   //hi

  token = getTokenFromFile(fileTokenizer);      //get "hi"
  TEST_ASSERT_EQUAL_STRING("hi",token->str);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);    //get "\n"
  TEST_ASSERT_EQUAL(1,token->type);
  freeToken(token);

  token = getTokenFromFile(fileTokenizer);    //get null token
  TEST_ASSERT_EQUAL(4,fileTokenizer->readLineNo);
  printf("tokenizer str is %s\n",fileTokenizer->tokenizer->str);
  freeToken(token);
                                            //my name is
  token = getTokenFromFile(fileTokenizer);  //get 'my'
  TEST_ASSERT_EQUAL(8,token->type);
  freeToken(token);

}
