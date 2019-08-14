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

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

/*****************TEST FOR TAP_SCAN_CLOCK****************************/
void test_handleTapScanClockDesc_with_correct_format_and_value(void){
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

/**********TEST FOR TAP_SCAN_IN*********************/
void test_handleScanPortDesc_with_correct_format_and_value(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testTapScanIn.bsdl";

  char *string[] ={
    " attribute TAP_SCAN_IN of ABC:signal is true;\n",
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
