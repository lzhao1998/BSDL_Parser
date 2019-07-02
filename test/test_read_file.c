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

void test_ing(void){
  TEST_ASSERT_NULL(NULL);
}

void test_getModelName_expect_STM32F469_F479_WLCSP168_byGiving_STM32F469_F479_WLCSP168_file(void){
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\STM32F469_F479_WLCSP168.bsd";
  BSinfo *bsInfo;
  bsInfo = getBSinfo(filename);
  TEST_ASSERT_EQUAL_STRING(bsInfo->modelName, "STM32F469_F479_WLCSP168");
}
/*
void test_read_exist_file(void){
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\test_read_file.c";
  TEST_ASSERT_NOT_NULL(check_file_exists(file_name));
}

void test_read_non_exist_file(void){
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\hello.txt";
  TEST_ASSERT_NULL(check_file_exists(file_name));
}

void test_read(void){
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\normal_name.txt";
  char* temp = read_file(file_name);
  //printf("%s",temp);
}*/
