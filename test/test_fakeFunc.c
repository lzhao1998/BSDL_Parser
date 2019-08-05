#include "unity.h"
#include "fakeFunc.h"
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

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

void test_fakeFunc_NeedToImplment(void){
  setupFake();
  char *string[] = {"entity STM32F469_F479_WLCSP168 is",NULL};
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "normal_name.txt";

  putStringArray(string);
  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",bsinfo->modelName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_fakeFunc_NeedToImplment2(void){
  setupFake();
  char *string[] = {"entity STM32F469 is",NULL};
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "normal_name.txt";

  putStringArray(string);
  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STM32F469",bsinfo->modelName);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);

}
