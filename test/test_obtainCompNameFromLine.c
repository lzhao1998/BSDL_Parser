#include "unity.h"
#include "read_file.h"
#include <stdlib.h>
#include <errno.h>
#include "Common.h"
#include "Token.h"
#include "Error.h"
#include "Tokenizer.h"
#include "Exception.h"
#include <stdio.h>
#include <string.h>

void setUp(void){}
void tearDown(void){}

void test_normal_compName_line_expect_compName(void){
  char *str = "entity xyz is ";
  char *result = obtainComponentNameFromLine(str);
  TEST_ASSERT_NOT_NULL(result);
  TEST_ASSERT_EQUAL_STRING(result,"xyz");
}

void test_obtainCompName_by_giving_xyz_is_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = " xyz is ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}

void test_obtainCompName_by_giving_entity_xyz_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = " entity  xyz ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}

void test_obtainCompName_by_giving_entity_is_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = " entity  is ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}

void test_obtainCompName_by_giving_port_xyz_is_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = "port xyz  is ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}

void test_obtainCompName_by_giving_entity_xyz_a_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = "entity xyz  a ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}

void test_obtainCompName_by_giving_entity_xyz_is_not_expect_fail(void){
  CEXCEPTION_T ex;
  char *str = "entity xyz  is not ";
  Try{
    char *result = obtainComponentNameFromLine(str);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    dumpTokenErrorMessage(ex, 1);
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME, ex->errorCode);
  }
}
