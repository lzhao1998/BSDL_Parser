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

void test_getStrToken_get_a_line_of_hello_world(void){
  char *string[] = {
    " \" hello world \"; \n",
    NULL
  };

  setupFake();
  Token *token;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
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
  TEST_ASSERT_EQUAL_STRING(";",token->str);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_get_list_of_string(void){
  char *string[] = {
    " \" hello world \" & \n",
    " \" feels tired \" & \n",
    "\" hey \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hello",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("world",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("feels",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("tired",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hey",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(";",token->str);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_get_list_of_string_and_some_string_is_empty(void){
  char *string[] = {
    " \" hello world \" & \n",
    " \"   \" & \n",
    " \"   \" & \n",
    "         ",
    "         ",
    " \" feels \" & \n",
    "\" tired \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hello",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("world",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("feels",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("tired",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(";",token->str);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_no_open_quote_expect_throw_error(void){
  char *string[] = {
    " \" hello world \" & \n",
    "  feels tired \" & \n",
    "\" hey \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  Try{
    putStringArray(string);
    fileTokenizer = createFileTokenizer(filename);
    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("hello",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("world",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but not!!");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_no_close_quote_expect_throw_error(void){
  char *string[] = {
    " \" hello world \" & \n",
    " \" feels tired  \n",
    "\" hey \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  Try{
    putStringArray(string);
    fileTokenizer = createFileTokenizer(filename);
    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("hello",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("world",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("feels",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("tired",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but not!!");

  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_after_end_is_endOfFile_expect_throw_error(void){
  char *string[] = {
    " \" hello world \" & \n",
    NULL
  };

  setupFake();
  Token *token;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  Try{
    putStringArray(string);
    fileTokenizer = createFileTokenizer(filename);
    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("hello",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("world",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but not!!");

  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_insert_identifer_after_close_quote_expect_throw_error(void){
  char *string[] = {
    " \" hello world \" errorHappen &\n",
    "\" hey \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  Try{
    putStringArray(string);
    fileTokenizer = createFileTokenizer(filename);
    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("hello",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("world",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but not!!");

  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}


void test_getStrToken_get_list_of_string_in_a_line(void){
  char *string[] = {
    " \" hello world \" &  \" feels tired \" & \" hey \" ;\n",
    NULL
  };

  setupFake();
  Token *token;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hello",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("world",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("feels",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("tired",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING("hey",token->str);
  freeToken(token);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(";",token->str);
  freeToken(token);

  freeFileTokenizer(fileTokenizer);
}

void test_getStrToken_triplequote_expect_throw_error(void){
  char *string[] = {
    " \" hello world \" \" \n",
    NULL
  };

  setupFake();
  Token *token;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "file.txt";

  Try{
    putStringArray(string);
    fileTokenizer = createFileTokenizer(filename);
    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("hello",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_ASSERT_EQUAL(TOKEN_IDENTIFIER_TYPE,token->type);
    TEST_ASSERT_EQUAL_STRING("world",token->str);
    freeToken(token);

    token = getStringToken(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but not!!");

  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}
