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
void setUp(void){}

void tearDown(void){}

void test_read_exist_file(void)
{
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\test_read_file.c";
  TEST_ASSERT_NOT_NULL(check_file_exists(file_name));

}

void test_read_non_exist_file(void)
{
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\hello.txt";
  TEST_ASSERT_NULL(check_file_exists(file_name));
}
/*
void test_read(void)
{
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\normal_name.txt";
  char* temp = read_file(file_name);
  //printf("%s",temp);
}*/


// GET MODEL NAME
void test_getModelName_expect_STM32F469_F479_WLCSP168_byGiving_STM32F469_F479_WLCSP168_file(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\STM32F469_F479_WLCSP168.bsd";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NOT_NULL(modelName);
  TEST_ASSERT_EQUAL_STRING(modelName, "STM32F469_F479_WLCSP168");
}

void test_getModelName_expect_NULL_by_giving_entity_only(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\entity_only.txt";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NULL(modelName);
}

void test_getModelName_expect_STM32_by_giving_normal_model_name_line(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\normal_name.txt";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NOT_NULL(modelName);
  TEST_ASSERT_EQUAL_STRING(modelName, "STM32_123");
}

void test_getModelName_expect_NULL_by_giving_entity_after_is_not_null(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\after_is_not_null.txt";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NULL(modelName);
}

void test_getModelName_expect_NULL_by_changing_is_to_null(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getModelName\\replace_is_to_not.txt";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NULL(modelName);
}

void test_getModelName_expect_fail_byGiving_file_not_exist(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\damn.txt";
  char* modelName = getModelName(filename);
  TEST_ASSERT_NULL(modelName);
}



/*
void test_getPackageName_expect_getThePackageName_byGiving_STM32F469_F479_WLCSP168_file(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\STM32F469_F479_WLCSP168.bsd";
  char* packageName = getPackageName(filename);
  TEST_ASSERT_NOT_NULL(packageName);
  TEST_ASSERT_EQUAL_STRING(packageName, "WLCSP168_PACKAGE");
}

void test_getPackageName_expect_fail_byGiving_file_which_is_not_exist(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\a8.bsd";
  char* packageName = getPackageName(filename);
  TEST_ASSERT_NULL(packageName);
}

void test_getPackageName_expect_fail_byGiving_file_not_contain_packageName(void)
{
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\temp.txt";
  char* packageName = getPackageName(filename);
  TEST_ASSERT_NULL(packageName);
}
*/
