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

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}


void test_handlePortDescription_for_printing_out_for_checking(void){
  char *string[] ={
    "port( --this is comment line\n",
    "BOOT0 :in      bit; --hello world\n",
    "BOOT1 :in      bit;\n",
    "BOOT2,JTCK : out bit_vector(0 to 9);\n",
    "BOOT3 : out bit_vector(2 downto 0)\n",
    "); --end",
    "port(\n",
    "BEAS : out bit\n ",
    ");\n",
    NULL
  };

  setupFake();
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "file_portTest.bsd";

  Try{
    putStringArray(string);
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    printPortDesc(bsinfo->port);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    printf("fail\n" );
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
  printf("=================\n" );
}


void test_handlePortDescription_normal_format_expect_pass(void){
  char *string[] = {
    "port(\n",
    " BOOT0 :in      bit;\n",
    " BOOT1,JTCK : out bit_vector(0 to 9)\n",
    ");\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "portDesc_normal_format.txt";


  bsinfo = initBSinfo();
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

void test_handlePortDescription_put_all_in_straight_line_expect_pass(void){
  char *string[] = {
    "port(BOOT0 :in bit;BOOT1,JTCK : out bit_vector(0 to 9));\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "portDesc_normal_format.txt";


  bsinfo = initBSinfo();
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

void test_handlePortDescription_normal_format_and_some_comment_line_expect_pass(void){
  char *string[] = {
    "port( --hello\n",
    " BOOT0 :in      bit; --hi\n",
    " BOOT1,JTCK : out bit_vector(0 to 9) --bye\n",
    ");\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "portDesc_normal_format.txt";


  bsinfo = initBSinfo();
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
    "port(\n",
    " BOOT0 :in      bit;\n",
    " BOOT1,JTCK : out bit_vector(0 to 9);\n",
    ");\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "semicolon_at_the_end_of_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
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
    "port(\n",
    " BOOT0 :in      bit\n",
    " BOOT1,JTCK : out bit_vector(0 to 9)\n",
    ");\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
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

void test_handlePortDescription_by_insert_int1_to_int2_where_int1_is_bigger_than_int2_expect_fail(void){
  char *string[] = {
    "port(\n",
    " BOOT0 :in      bit;\n",
    " BOOT1,JTCK : out bit_vector(5 to 1)\n",
    ");\n",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_VALUE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_insert_int1_downto_int2_where_int2_is_bigger_than_int1_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 :in      bit;",
    " BOOT1,JTCK : out bit_vector(1 downto 5)",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_VALUE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_insert_invalid_rangeType_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT1 : out bit_vector(1 downtoz 5)",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "dfs.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_RANGETYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_replace_bit_to_bot_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 :in      bot",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PORTDIMENSION, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_insert_invalid_pinType_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 : inoutz      bit",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PINTYPE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_declare_same_pin_twice_expect_fail(void){
  char *string[] = {
    "port(",
    " BOOT0 : inout      bit;",
    " BOOT0 : inout      bit",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_MULTIPLE_DECLARE, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_inserting_invalid_portName_expect_fail(void){
  char *string[] = {
    "port(",
    " BOO__1 : inout      bit",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PORTNAME, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handlePortDescription_by_remove_left_paren_after_port_expect_fail(void){
  char *string[] = {
    "port",
    " BOO1 : inout      bit",
    ");",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
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

void test_handlePortDescription_by_remove_right_paren_expect_fail(void){
  char *string[] = {
    "port(",
    " BOO1 : inout      bit",
    ";",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
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

void test_handlePortDescription_by_remove_semicolon_at_the_end_of_portDesc_expect_fail(void){
  char *string[] = {
    "port(",
    " BOO1 : inout      bit",
    ")",
    NULL
  };

  setupFake();
  putStringArray(string);
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "no_semicolon_1st_pinSpec.txt";

  Try{
    bsinfo = initBSinfo();
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
