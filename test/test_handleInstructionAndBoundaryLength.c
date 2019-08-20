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

/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is 2;
**          attribute BOUNDARY_LENGTH of STM32F469 : entity is 1;
*/
void test_handleInstructionAndBoundaryLength_with_instructionLength_and_boundaryLength_expect_pass(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is 2;\n",
    "attribute BOUNDARY_LENGTH of STM32F469 : entity is 1;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL(2,bsinfo->instructionLength);
  TEST_ASSERT_EQUAL(1,bsinfo->boundaryLength);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is 5;
*/
void test_handleInstructionAndBoundaryLength_by_inserting_instruction_length_5_only_expect_pass(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is 5;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL(5,bsinfo->instructionLength);
  TEST_ASSERT_EQUAL(-1,bsinfo->boundaryLength);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity is 3;
*/
void test_handleInstructionAndBoundaryLength_by_inserting_boundary_length_3_only_expect_pass(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is 3;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL(-1,bsinfo->instructionLength);
  TEST_ASSERT_EQUAL(3,bsinfo->boundaryLength);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}


/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F: entity is 3;
*/
void test_handleInstructionAndBoundaryLength_with_different_modelName_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F: entity is 3;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469 entity is 3;
*/
void test_handleInstructionAndBoundaryLength_without_colon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469 entity is 3;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity is 3
*/
void test_handleInstructionAndBoundaryLength_without_semicolon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is 3\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity  3;
*/
void test_handleInstructionAndBoundaryLength_without_is_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity  3;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity is 1;
*/
void test_handleInstructionAndBoundaryLength_with_boundary_length_lower_than_1_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is 0;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is 1;
*/
void test_handleInstructionAndBoundaryLength_with_instruction_length_lower_than_2_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is 1;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is ;
*/
void test_handleInstructionAndBoundaryLength_with_instruction_length_without_value_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is ;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity is ;
*/
void test_handleInstructionAndBoundaryLength_with_boundary_length_without_value_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is ;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute BOUNDARY_LENGTH of STM32F469: entity is 2;
**          attribute BOUNDARY_LENGTH of STM32F469: entity is 3;
*/
void test_handleInstructionAndBoundaryLength_with_multiple_boundary_length_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is 2;\n",
    "attribute BOUNDARY_LENGTH of STM32F469: entity is 3;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
**  Input : entity STM32F469 is
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is 4;
**          attribute INSTRUCTION_LENGTH of STM32F469: entity is 5;
*/
void test_handleInstructionAndBoundaryLength_with_multiple_instruction_length_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testhandleInstructionAndBoundaryLength.bsdl";

  char *string[] ={
    "entity STM32F469 is\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is 4;\n",
    "attribute INSTRUCTION_LENGTH of STM32F469: entity is 5;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo =  initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION_LENGTH, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
