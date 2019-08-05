#include "unity.h"
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

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}


void test_handlePortDescription_for_printing_out_for_checking(void)
{
  char *string[] ={
    "port(",
    "BOOT0 :in      bit;",
    "BOOT1 :in      bit;",
    "BOOT2,JTCK : out bit_vector(0 to 9)",
    ");",
    NULL
  };

  setupFake();
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\file_portTest.bsd";

  Try{
    putStringArray(string);
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    printPortDesc(bsinfo->port);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    //TEST_ASSERT_EQUAL(ERR_PORT_DESCRIPTION, ex->errorCode);
    printf("fail\n" );
    dumpException(ex);
    //dumpTokenErrorMessage(ex,1);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
  printf("=================\n" );
}


void test_handlePortDescription_by_inserting_correct_format_expect_pass(void){
  char *string[] = {
    "port(",
    " BOOT0 :in      bit;",
    " BOOT1,JTCK : out bit_vector(0 to 9)",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handlePortDesc\\portDesc_normal_format.txt";


  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_NOT_NULL(bsinfo);
  Item *item;
  portDesc *portD;

  item = bsinfo->port->head;
  portD = ((portDesc*)item->data);
  TEST_ASSERT_EQUAL_STRING("BOOT0",portD->portName);
  TEST_ASSERT_EQUAL(0,portD->pinType);
  TEST_ASSERT_EQUAL(0,portD->bitType);
  TEST_ASSERT_EQUAL(0,portD->integer1);
  TEST_ASSERT_EQUAL(2,portD->rangeType);
  TEST_ASSERT_EQUAL(0,portD->integer2);

  item = item->next;
  portD = ((portDesc*)item->data);
  TEST_ASSERT_EQUAL_STRING("BOOT1",portD->portName);
  TEST_ASSERT_EQUAL(1,portD->pinType);
  TEST_ASSERT_EQUAL(1,portD->bitType);
  TEST_ASSERT_EQUAL(0,portD->integer1);
  TEST_ASSERT_EQUAL(0,portD->rangeType);
  TEST_ASSERT_EQUAL(9,portD->integer2);

  item = item->next;
  portD = ((portDesc*)item->data);
  TEST_ASSERT_EQUAL_STRING("JTCK",portD->portName);
  TEST_ASSERT_EQUAL(1,portD->pinType);
  TEST_ASSERT_EQUAL(1,portD->bitType);
  TEST_ASSERT_EQUAL(0,portD->integer1);
  TEST_ASSERT_EQUAL(0,portD->rangeType);
  TEST_ASSERT_EQUAL(9,portD->integer2);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_inserting_semicolon_at_the_end_of_pinSpec_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 :in      bit;",
    " BOOT1,JTCK : out bit_vector(0 to 9);",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handlePortDesc\\semicolon_at_the_end_of_pinSpec.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_PORT_DESCRIPTION, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_removing_semicolon_at_the_end_of_1st_pinSpec_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 :in      bit",
    " BOOT1,JTCK : out bit_vector(0 to 9)",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handlePortDesc\\no_semicolon_1st_pinSpec.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_PORT_DESCRIPTION, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
