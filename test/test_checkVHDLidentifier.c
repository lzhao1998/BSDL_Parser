#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"

void setUp(void){}
void tearDown(void){}

void test_1(void){
  char *str = "IEEE_STD_123";
  int result = checkVHDLidentifier(str);
  TEST_ASSERT_EQUAL(1, result);
}

void test_2(void){
  char *str = "IEE_STD_123_";
  int result = checkVHDLidentifier(str);
  TEST_ASSERT_EQUAL(0, result);
}

void test_3(void){
  char *str = "IEE_STD__123";
  int result = checkVHDLidentifier(str);
  TEST_ASSERT_EQUAL(0, result);
}

void test_4(void){
  char *str = "IEE._STD_123";
  int result = checkVHDLidentifier(str);
  TEST_ASSERT_EQUAL(0, result);
}

void test_5(void){
  char *str = "#IEE_STD_123";
  int result = checkVHDLidentifier(str);
  TEST_ASSERT_EQUAL(0, result);
}
