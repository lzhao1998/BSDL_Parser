#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Token.h"
#include "Error.h"
#include "unity.h"
#include "Common.h"
#include "fakeFunc.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string);
void test_handleGenericParameterDesc_by_giving_generic_default_format_expect_return_empty_string(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic(PHYSICAL_PIN_MAP: string);\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("",bsinfo->packageName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:="DW");
void test_handleGenericParameterDesc_by_giving_generic_no_default_format_expect_return_string(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic(PHYSICAL_PIN_MAP: string:=\"DW\");\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("DW",bsinfo->packageName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:="DW"); --hello world
void test_handleGenericParameterDesc_by_inserting_comment_line_behind_expect_throw_error(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic(PHYSICAL_PIN_MAP: string:=\"DW\");  --hello world\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("DW",bsinfo->packageName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic(: string:="IEE__STD");
void test_handleGenericParameterDesc_by_inserting_invalid_pin_map_name_expect_throw_error(void){
  BSinfo *bsinfo;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic(PHYSICAL_PIN_MAP: string:=\"IEE__STD\"); \n",
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
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT:  generic (PHYSICAL_PIN_MAP string := "DW");
void test_handleGenericParameterDesc_by_removing_colon_expect_throw_error(void){
  BSinfo *bsinfo;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic (PHYSICAL_PIN_MAP string := \"DW\"); \n",
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
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT:   generic PHYSICAL_PIN_MAP: string := "DW");
void test_handleGenericParameterDesc_by_removing_left_paren_expect_throw_error(void){
  BSinfo *bsinfo;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic PHYSICAL_PIN_MAP: string := \"DW\"); \n",
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
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic (PHYSICAL_PIN_MAP: string := "DW")
void test_handleGenericParameterDesc_by_removing_semicolon_expect_throw_error(void){
  BSinfo *bsinfo;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic (PHYSICAL_PIN_MAP: string := \"DW\") \n",
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
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}



//INPUT:  generic (PHYSICAL_PIN_MAP: string); this is not null
void test_handleGenericParameterDesc_by_insert_some_word_after_semiclon_expect_throw_error(void){
  BSinfo *bsinfo;
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    "  generic (PHYSICAL_PIN_MAP: string); hello \n",
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
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
