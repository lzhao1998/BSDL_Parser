#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"

void setUp(void){}
void tearDown(void){}

/***  file_test_for_getTokenFromFile contain:
          hello world 123
***/

void test_getTokenFromFile_expect_return_identifier_token_when_read_file_test_for_getTokenFromFile_file(void){
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
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
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

void test_getTokenFromFile_expect_return_NULL_token_when_read_file_test_for_getTokenFromFile_file_unitl_end_of_line(void){
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  FileTokenizer *fileTokenizer;
  Token *token;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);
  TEST_ASSERT_EQUAL_STRING("hello world",fileTokenizer->tokenizer->str);

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
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//test it when until eof
void test_getTokenFromFile_expect_return_EndOfFile_token_when_read_file_test_for_getTokenFromFile_file_unitl_end_of_file(void){
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\file_test_for_getTokenFromFile.txt";
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

  TEST_ASSERT_EQUAL(1,fileTokenizer->readLineNo);

  token = getTokenFromFile(fileTokenizer);
  TEST_ASSERT_NOT_NULL(token);
  TEST_ASSERT_EQUAL(TOKEN_EOF_TYPE,token->type); 
  freeToken(token);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
