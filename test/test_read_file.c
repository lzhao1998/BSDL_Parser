#include "unity.h"
#include "read_file.h"
#include <stdio.h>

void setUp(void){}

void tearDown(void){}

void test_read_exist_file(void)
{
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\hey.txt";
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
  char *file_name = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\test\\hello.txt";
  read(read_file(file_name));
}*/
