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

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

/***********TEST FOR HANDLEPINDESCORLIST***********/

//Input : "TIE0"
void test_handlePinDescOrList_with_pinDesc_which_is_identifier_expect_pass(void){
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"TIE0\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePinDescOrList(fileTokenizer);
  Item *current;
  pinDescription *pinD;
  current = result->head;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("TIE0",pinD->pinDesc);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
}

//Input: "132"
void test_handlePinDescOrList_with_pinDesc_which_is_integer_expect_pass(void){
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"132\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePinDescOrList(fileTokenizer);
  Item *current;
  pinDescription *pinD;
  current = result->head;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("132",pinD->pinDesc);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
}

//Input: "1.23"
void test_handlePinDescOrList_with_pinDesc_which_is_float_expect_throwError(void){
  CEXCEPTION_T ex;
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"1.23\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = handlePinDescOrList(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PINDESC_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

//Input : "(OPEN,TIE0)"
void test_handlePinDescOrList_with_pinList_which_is_identifier_type_expect_pass(void){
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"(OPEN,TIE0)\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePinDescOrList(fileTokenizer);
  Item *current;
  pinDescription *pinD;
  current = result->head;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("OPEN",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(current->next);

  current = current->next;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("TIE0",pinD->pinDesc);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
}

//Input : "(Pad08,12)"
void test_handlePinDescOrList_with_pinList_which_is_identifier_and_integer_type_expect_pass(void){
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"(Pad08,12)\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePinDescOrList(fileTokenizer);
  Item *current;
  pinDescription *pinD;
  current = result->head;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("Pad08",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(current->next);

  current = current->next;
  pinD = (pinDescription*)current->data;
  TEST_ASSERT_EQUAL_STRING("12",pinD->pinDesc);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
}

//Input : "(Pad08 12)"
void test_handlePinDescOrList_with_pinList_without_comma_expect_throwError(void){
  CEXCEPTION_T ex;
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"(Pad08 12)\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = handlePinDescOrList(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PINDESC_FORMAT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

//Input : "(1.11, abc)"
void test_handlePinDescOrList_with_pinList_with_float_and_identifier_type_expect_throwError(void){
  CEXCEPTION_T ex;
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"(1.11 abc)\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = handlePinDescOrList(fileTokenizer);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PINDESC, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

//Input : "()"
void test_handlePinDescOrList_with_pinList_inside_empty_expect_throwError(void){
  CEXCEPTION_T ex;
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"()\"",
    NULL
  };

  setupFake();
  putStringArray(string);
  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = handlePinDescOrList(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PINDESC, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

/********TEST FOR HANDLE PORT MAP*********/
void test_handlePortMap_with_1_portname_and_1_pinDesc(void){
  LinkedList *result;
  Item *current;
  portMap *portM;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"D : 23\";",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePortMap(fileTokenizer);

  current = result->head;
  portM = (portMap*)current->data;
  TEST_ASSERT_EQUAL_STRING("D",portM->portName);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
}

void test_handlePortMap_with_1_portname_and_multiple_pinDesc(void){
  LinkedList *result, *result2;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"CLK : (23,12,32)\"; \n",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePortMap(fileTokenizer);

  Item *current;
  portMap *portM;
  current = result->head;
  portM = (portMap*)current->data;
  TEST_ASSERT_EQUAL_STRING("CLK",portM->portName);
  TEST_ASSERT_NULL(current->next);

  Item *currentChild;
  pinDescription *pinD;
  result2 = (LinkedList*)(portM->pindesc);
  currentChild = result2->head;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("23",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(currentChild->next);

  currentChild = currentChild->next;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("12",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(currentChild->next);

  currentChild = currentChild->next;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("32",pinD->pinDesc);
  TEST_ASSERT_NULL(currentChild->next);

  freeFileTokenizer(fileTokenizer);
}

void test_handlePortMap_with_multiple_portname_and_multiple_pinDesc(void){
  LinkedList *result, *result2;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"CLK : (23,12,32),\" & \n",
    "\"TDO : Pad08\"; \n",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  result = handlePortMap(fileTokenizer);

  Item *current;
  portMap *portM;
  current = result->head;
  portM = (portMap*)current->data;
  TEST_ASSERT_EQUAL_STRING("CLK",portM->portName);
  TEST_ASSERT_NOT_NULL(current->next);

  Item *currentChild;
  pinDescription *pinD;
  result2 = (LinkedList*)(portM->pindesc);
  currentChild = result2->head;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("23",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(currentChild->next);

  currentChild = currentChild->next;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("12",pinD->pinDesc);
  TEST_ASSERT_NOT_NULL(currentChild->next);

  currentChild = currentChild->next;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("32",pinD->pinDesc);
  TEST_ASSERT_NULL(currentChild->next);

  current = current->next;
  portM = (portMap*)current->data;
  TEST_ASSERT_EQUAL_STRING("TDO",portM->portName);
  TEST_ASSERT_NULL(current->next);

  result2 = (LinkedList*)(portM->pindesc);
  currentChild = result2->head;
  pinD = (pinDescription*)currentChild->data;
  TEST_ASSERT_EQUAL_STRING("Pad08",pinD->pinDesc);
  TEST_ASSERT_NULL(currentChild->next);

  freeFileTokenizer(fileTokenizer);
}

void test_handlePortMap_with_invalid_portname_expect_throw_error(void){
  CEXCEPTION_T ex;
  LinkedList *result;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "\"D12__ : 23\";",
    NULL
  };

  setupFake();
  putStringArray(string);
  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = handlePortMap(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it's not!!");

  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_PORTNAME, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_handlePinMapping(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "test1pinDesc.bsd";

  char *string[] ={
    "constant DW : PIN_MAP_STRING :=\n",
    "\"CLK : (23,12,32),\" & \n",
    "\"TDO : Pad08\"; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    handlePinMapping(fileTokenizer,bsinfo->pinMapping);
    printPinMapping(bsinfo->pinMapping);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    printf("fail\n" );
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
