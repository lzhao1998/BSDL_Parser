#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"

void setUp(void){}
void tearDown(void){}

void test_skipLine_expect_pass(void){
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\skipLine\\2_line.txt";
  fileTokenizer = createFileTokenizer(filename);

  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("i am fine",fileTokenizer->tokenizer->str);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

void test_skipLine_by_giving_only_1_line_file_expect_get_tokenizer_str_null(void){
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\skipLine\\goodbye_world.txt";
  fileTokenizer = createFileTokenizer(filename);

  skipLine(fileTokenizer);
  TEST_ASSERT_NULL(fileTokenizer->tokenizer->str);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

void test_skipLine_multiple_times_expect_pass(void){
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\skipLine\\multiple_line.txt";
  fileTokenizer = createFileTokenizer(filename);

  TEST_ASSERT_EQUAL_STRING("this is 1st line\n",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("this is 2nd line\n",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("3rd line here\n",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("4th line is the last line.",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_NULL(fileTokenizer->tokenizer->str);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
