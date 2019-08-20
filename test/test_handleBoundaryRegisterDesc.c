#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Token.h"
#include "Error.h"
#include "unity.h"
#include "Common.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"
#include "fakeFunc.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handleScanPortIdentification.h"
#include "handleBoundaryRegisterDesc.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

void test_handleBoundaryRegisterDesc_with_correct_format(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testPinMappingDesc.bsd";

  char *string[] ={
    "\"4        (BC_1,     PI6,      OUTPUT3,   X,	 5,	1,	Z),	\" & \n",
    "\"3        (BC_4,     PI6,      INPUT,	    X),				\" & ",
    "\"2        (BC_1,       *,      CONTROL,   1),       \" & \n",
    "\"1        (BC_1,     PI7,      OUTPUT3,   X,	 2,	1,	Z),	\" & \n",
    "\"0        (BC_4,     PI7,      INPUT,	    X) 				\" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,bsinfo->boundaryReg);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }


  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

// TEST for getSubSting function //
void test_getsubstring_get_world_from_hello_world(void){
  char *str1 = "hello world";
  char *str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  char *result;
  result = getSubString(str,6,10);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
  str1 = "goodbye tmr";
  str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
}

void test_getsubstring_get_world_from_hello_world_abc(void){
  char *str1 = "hello world abc";
  char *str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  char *result;
  result = getSubString(str,6,10);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
  str1 = "goodbye, see you tmr";
  str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
}

// Test for getPortId function //
void test_getPortId_expect_pass_with_normal_portId(void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"GND,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("GND",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_expect_pass_with_subscripted_portname(void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"Q(5),\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("Q(5)",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_expect_pass_with_asterisk (void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"*,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("*",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}


void test_getPortId_with_invalid_portname(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"CL__K,\"",
    NULL
  };

  setupFake();
  putStringArray(string);


  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_replace_integer_to_char(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO(Z),\"",
    NULL
  };

  setupFake();
  putStringArray(string);


  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_no_closing_bracket(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO(1,\"",
    NULL
  };

  setupFake();
  putStringArray(string);


  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_no_open_bracket(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO1),\"",
    NULL
  };

  setupFake();
  putStringArray(string);


  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_portname_by_replace_arterisk_with_other_symbol(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\",\"",
    NULL
  };

  setupFake();
  putStringArray(string);


  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}
