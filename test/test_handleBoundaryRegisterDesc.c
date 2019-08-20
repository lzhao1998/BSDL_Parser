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
#include "handleScanPortIdentification.h"
#include "handleBoundaryRegisterDesc.h"


void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

// test_handleBoundaryRegisterDesc//
void test_handleBoundaryRegisterDesc_with_correct_format_expect_pass(void){
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "entity STM32 is\n,",
    "\n",
    "attribute BOUNDARY_REGISTER of STM32 : entity is \n",
    "-- just a comment line\n",
    "\"5  (BC_1,  PI6,  OUTPUT3,  X,  5,  1, Z),	\" & \n",
    "\"4  (BC_4,  PI6,  INPUT,    X)\" ; ",
    NULL
  };

  setupFake();
  putStringArray(string);
  bsinfo = initBSinfo();
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  printBoundaryRegister(bsinfo->boundaryReg);

  Item *current;
  boundaryRegister *bs;

  current = bsinfo->boundaryReg->head;
  bs = ((boundaryRegister*)current->data);
  TEST_ASSERT_EQUAL_STRING("5",bs->cellNumber);
  TEST_ASSERT_EQUAL_STRING("BC_1",bs->cellName);
  TEST_ASSERT_EQUAL_STRING("PI6",bs->portId);
  TEST_ASSERT_EQUAL_STRING("OUTPUT3",bs->function);
  TEST_ASSERT_EQUAL_STRING("X",bs->safeBit);
  TEST_ASSERT_EQUAL_STRING("5",bs->ccell);
  TEST_ASSERT_EQUAL_STRING("1",bs->disval);
  TEST_ASSERT_EQUAL_STRING("Z",bs->disres);
  TEST_ASSERT_NOT_NULL(current->next);

  current = current->next;
  bs = ((boundaryRegister*)current->data);
  TEST_ASSERT_EQUAL_STRING("4",bs->cellNumber);
  TEST_ASSERT_EQUAL_STRING("BC_4",bs->cellName);
  TEST_ASSERT_EQUAL_STRING("PI6",bs->portId);
  TEST_ASSERT_EQUAL_STRING("INPUT",bs->function);
  TEST_ASSERT_EQUAL_STRING("X",bs->safeBit);
  TEST_ASSERT_EQUAL_STRING("",bs->ccell);
  TEST_ASSERT_EQUAL_STRING("",bs->disval);
  TEST_ASSERT_EQUAL_STRING("",bs->disres);
  TEST_ASSERT_NULL(current->next);

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

void test_handleBoundaryRegisterDesc_with_incorrect_component_name_expect_fail(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "entity STM32 is\n,",
    "\n",
    "attribute BOUNDARY_REGISTER of STM23 : entity is \n",
    "-- just a comment line\n",
    "\"5  (BC_1,  PI6,  OUTPUT3,  X,  5,  1, Z),	\" & \n",
    "\"4  (BC_4,  PI6,  INPUT,    X)\" ; ",
    NULL
  };

  Try{
    setupFake();
    putStringArray(string);
    bsinfo = initBSinfo();
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

// Test for boundaryregister//
//this use printing out to view the result
void test_handleBoundaryRegister_with_correct_format_expect_pass(void){
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"8  (SD1,   D(4),  OUTPUT2,  0,  KEEPER), \" &\n",
    "\"7  (AR_2,  Q(1),  INPUT,   1,  2,  1,  WEAK0), \"& \n",
    "\"6  (BC_1,  *,    CONTROL,  1), \" &\n",
    "\"5  (BC_1,  PI6,  OUTPUT3,  X,  5,  1, Z),	\" & \n",
    "\"4  (BC_4,  PI6,  INPUT,    X)\" ; ",
    NULL
  };

  setupFake();
  putStringArray(string);
  fileTokenizer = createFileTokenizer(filename);
  handleBoundaryRegister(fileTokenizer,list);
  printBoundaryRegister(list);
  printf("\n\n" );

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_no_semicolon_at_the_end_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"4  (BC_1,  PI6,  OUTPUT3,  X,  5,  1,  Z),	\" & \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING_TYPE,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_cellnum_is_not_integer_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"hello  (BC_1,  PI6,  OUTPUT3,  X,  5,  1,  Z),	\" & \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_no_open_bracket(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"0  BC_4, PI7,  INPUT,  X)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_end_after_insert_portId_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"0  (BC_4,  PI7)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_invalid_portId_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"0  (BC_4,  CLK_, OUTPUT, X)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_invalid_fucntion_name_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"0  (BC_4,  PI7,  INOUTPUT, X)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_invalid_safe_bit_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={

    "\"2  (BC_1,  *,  CONTROL,  Z)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_ccell_is_not_integer_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"4  (BC_1,  PI6,  OUTPUT3,  X,  P12,  1,  Z)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_invalid_disval_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"3  (BC_1,  PI6,  OUTPUT3,  X,  5,  2,  Z)  \" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_invalid_disableResult_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"4  (BC_1,  PI6,  OUTPUT3,  X,  5,  1,  OUT1)	\" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_inputSpec_but_disable_format_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"3  (BC_1,  AI7,  OUTPUT3,  X,  EXTERN0,  1,  Z);	\" \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

void test_handleBoundaryRegister_with_disableSpec_but_inputSpec_format_expect_fail(void){
  CEXCEPTION_T ex;
  LinkedList *list;
  list = listInit();
  FileTokenizer *fileTokenizer;
  char *filename = "testBR.bsd";

  char *string[] ={
    "\"4  (BC_1,  PI6,  OUTPUT3,  X,  5)	\" ; \n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    handleBoundaryRegister(fileTokenizer,list);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeLinkedList(list);
}

// TEST for getSubSting function //
void test_getsubstring_get_world_from_hello_world(void){
  char *str1 = "hello world";
  char *str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  char *result;
  result = getSubString(str,6,10);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
  str1 = "goodbye tmr";
  str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
}

void test_getsubstring_get_world_from_hello_world_abc(void){
  char *str1 = "hello world abc";
  char *str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  char *result;
  result = getSubString(str,6,10);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
  str1 = "goodbye, see you tmr";
  str = malloc(sizeof(char) * strlen(str1));
  strcpy(str,str1);
  TEST_ASSERT_EQUAL_STRING("world",result);
  free(str);
}

// Test for getPortId function //
void test_getPortId_expect_pass_with_normal_portId(void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"GND,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("GND",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_expect_pass_with_subscripted_portname(void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"Q(5),\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("Q(5)",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_expect_pass_with_asterisk (void){
  Token *token;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"*,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  fileTokenizer = createFileTokenizer(filename);
  result = getPortId(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("*",result);

  token = getStringToken(fileTokenizer);
  TEST_ASSERT_EQUAL(TOKEN_OPERATOR_TYPE,token->type);
  TEST_ASSERT_EQUAL_STRING(",",token->str);
  freeToken(token);
  freeFileTokenizer(fileTokenizer);
}


void test_getPortId_with_invalid_portname(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"CL__K,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_replace_integer_to_char(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO(Z),\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_no_closing_bracket(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO(1,\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_invalid_subscripted_portname_by_no_open_bracket(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\"DO1),\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}

void test_getPortId_with_portname_by_replace_arterisk_with_other_symbol(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *result;
  char *filename = "testgetPortId.bsd";

  char *string[] ={
    "\",\"",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
    fileTokenizer = createFileTokenizer(filename);
    result = getPortId(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect fail but it is not\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_BOUNDARY_REGISTER,ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
}
