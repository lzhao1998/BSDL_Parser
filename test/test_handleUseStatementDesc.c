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

// INPUT: use STD_1149_1_2001.all;
void test_expect_return_true_when_useStatement_is_in_correct_order(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use STD_1149_1_2001.all;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STD_1149_1_2001",bsinfo->useStatement);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001.all
void test_handleUseStatementDesc_by_removing_semicolon_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use STD_1149_1_2001.all\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001.;
void test_handleUseStatementDesc_by_without_putting_all_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use STD_1149_1_2001.;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001 all;
void test_handleUseStatementDesc_by_without_putting_dot_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use STD_1149_1_2001 all;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use .all;
void test_handleUseStatementDesc_by_without_putting_userPackageName_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use .all;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001
void test_handleUseStatementDesc_by_giving_use_and_usePackageNameOnly_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    "use STD_1149_1_2001\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2__001.all;
void test_handleUseStatementDesc_by_giving_invalid_packageName_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    " use STD_1149_1_2__001.all;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1123_1_2_001.all;
void test_handleUseStatementDesc_by_giving_invalid_package_name_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    " use STD_1123_1_2_001.all;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001.all;
//       use STD_1149_1_1994.all;
void test_handleUseStatementDesc_by_giving_multiple_useStatement_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testUseStatement.bsdl";

  char *string[] ={
    " use STD_1149_1_2001.all;\n",
    " use STD_1149_1_1994.all;\n",
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
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
