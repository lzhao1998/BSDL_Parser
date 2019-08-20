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
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"
#include "handleScanPortIdentification.h"
#include "handleBoundaryRegisterDesc.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

/*****************TEST FOR TAP_SCAN_CLOCK****************************/
void test_handleTapScanClockDesc_with_correct_format_and_integer_clock_value(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1000, LOW);\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("ABC",bsinfo->tapScanClk->portId);
  TEST_ASSERT_EQUAL_STRING("1000",bsinfo->tapScanClk->clock);
  TEST_ASSERT_EQUAL_STRING("LOW",bsinfo->tapScanClk->haltState);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_with_correct_format_and_float_clock_value(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1.0e4, BOTH);\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("ABC",bsinfo->tapScanClk->portId);
  TEST_ASSERT_EQUAL_STRING("1.0e4",bsinfo->tapScanClk->clock);
  TEST_ASSERT_EQUAL_STRING("BOTH",bsinfo->tapScanClk->haltState);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_insert_invalid_portId_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC_:signal is (1000, LOW);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_signal_is_true_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is true;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}


void test_handleTapScanClockDesc_replace_colon_with_semicolon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC;signal is (1000, LOW);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_invalid_clock_value_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (ge, LOW);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_invalid_halt_state_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1200, HIGH);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_declare_tapScanClock_twice_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1200, LOW);\n",
    " attribute TAP_SCAN_CLOCK of DEF:signal is (200, BOTH);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_removing_left_paren_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is 1200, LOW);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_removing_right_paren_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1200, LOW;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleTapScanClockDesc_removing_comma_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_CLOCK of ABC:signal is (1200 LOW);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_CLOCK_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/**********TEST FOR TAP_SCAN_IN*********************/
void test_handleScanPortDesc_TapScanIn_with_correct_format_and_value(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC:signal is true; --hello\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("ABC",bsinfo->tapScanIn);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_by_replacing_true_to_false_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC:signal is false;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_by_removing_semicolon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC:signal is true\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_by_replace_colon_with_semicolon_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC;signal is true;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_by_giving_invalid_portId_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of DE__F:signal is true;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_by_declare_TapScanIn_twice_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of DEF:signal is true;\n",
    " attribute TAP_SCAN_IN of asd:signal is true;\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleScanPortDesc_TapScanIn_with_TAP_SCAN_CLOCK_format_and_value(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC:signal is (2e8, BOTH);\n",
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
    TEST_ASSERT_EQUAL(ERR_INVALID_TAP_SCAN_IN_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/************TEST ALL SCAN PORT IDENTIFICATION**********/
void test_handleTapScanClockDesc_and_handleScanPortDesc(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanClock.bsdl";

  char *string[] ={
    "attribute TAP_SCAN_CLOCK of ABC:signal is (1000, LOW);\n",
    "attribute TAP_SCAN_IN of PD:signal is true;\n",
    "attribute TAP_SCAN_MODE of DE:signal is true; --comment line\n",
    "attribute TAP_SCAN_OUT of WR:signal is true;\n",
    "attribute TAP_SCAN_RESET of DR:signal is true;\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  bsinfo =  initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("ABC",bsinfo->tapScanClk->portId);
  TEST_ASSERT_EQUAL_STRING("1000",bsinfo->tapScanClk->clock);
  TEST_ASSERT_EQUAL_STRING("LOW",bsinfo->tapScanClk->haltState);
  TEST_ASSERT_EQUAL_STRING("PD",bsinfo->tapScanIn);
  TEST_ASSERT_EQUAL_STRING("DE",bsinfo->tapScanMode);
  TEST_ASSERT_EQUAL_STRING("WR",bsinfo->tapScanOut);
  TEST_ASSERT_EQUAL_STRING("DR",bsinfo->tapScanReset);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
