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
#include "fakeFunc.h"
#include "mock_createAndGetTokenFromFile.h"
#include "handlePinMappingDesc.h"
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

// INPUT: entity STM32F469_F479_WLCSP168 is
void test_handleComponentNameDesc_expect_return_componentName_when_its_in_correct_order(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168 is\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",bsinfo->modelName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

// INPUT: entity STM32F469_F479_WLCSP168 is --hello
void test_handleComponentNameDesc_expect_return_componentName_when_its_in_correct_order_and_there_is_comment_line_at_behind(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168 is --hello\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",bsinfo->modelName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity STM32F469_F479_WLCSP168
void test_handleComponentNameDesc_by_without_putting_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity STM32F469_F479_WLCSP168_ is
void test_handleComponentNameDesc_by_giving_invalid_componentName_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168_ is\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity STM32F469_F479_WLCSP168 a
void test_handleComponentNameDesc_by_replace_a_with_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168 a\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity STM32F469_F479_WLCSP168 is not
void test_handleComponentNameDesc_by_adding_not_behind_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32F469_F479_WLCSP168 is not\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity STM32 is \n entity STM123 is
void test_handleComponentNameDesc_by_giving_componentName_twice_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity STM32 is \n",
    "entity STM123 is\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: entity
void test_handleComponentNameDesc_by_giving_entity_only_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testCompName.bsdl";

  char *string[] ={
    "entity\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
