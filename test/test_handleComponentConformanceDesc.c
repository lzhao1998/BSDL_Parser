#include <stdio.h>
#include "Token.h"
#include "Error.h"
#include "unity.h"
#include "Common.h"
#include "fakeFunc.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "handleScanPortIdentification.h"
#include "mock_createAndGetTokenFromFile.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

void test_handleComponentConformance_with_correct_format(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "--comment line\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469:entity is \"STD_1149_1_1994\";\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469",bsinfo->modelName);
  TEST_ASSERT_EQUAL_STRING("STD_1149_1_1994",bsinfo->componentConformance);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_diffrent_modelName_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F566:entity is \"STD_1149_1_1994\";\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_conformance_component_is_in_identifier_type_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469:entity is STD_1149_1_1994;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_remove_colon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 entity is \"STD_1149_1_1994\";\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_replace_semicolon_with_colon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 :entity is \"STD_1149_1_1994\"\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_add_comment_line_behind_expect_pass(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 :entity is \"STD_1149_1_1994\"; --comment\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469",bsinfo->modelName);
  TEST_ASSERT_EQUAL_STRING("STD_1149_1_1994",bsinfo->componentConformance);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_insert_invalid_component_conformance_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 :entity is \"ST1994\";\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleComponentConformance_declare_component_conformance_twice_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testComponentConformance.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 :entity is \"STD_1149_1_1994\";\n",
    "attribute COMPONENT_CONFORMANCE of STM32F469 :entity is \"STD_1149_1_2001\";\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but no.\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_COMPONENT_COMFORMANCE_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
