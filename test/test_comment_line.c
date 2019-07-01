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

void test_normal_comment_line_expect_return_1(void){
  char *str = "-- This section is comment line";
  int result = isCommentLine(str);
  TEST_ASSERT_EQUAL(result,1);
}

void test_triple_dash_comment_line_expect_return_1(void){
  char *str = "--- This section is comment line";
  int result = isCommentLine(str);
  TEST_ASSERT_EQUAL(result,1);
}

void test_one_dash_only_comment_line_expect_return_0(void){
  char *str = "- This section is comment line";
  int result = isCommentLine(str);
  TEST_ASSERT_EQUAL(result,0);
}

void test_no_dash_infront_comment_line_expect_return_0(void){
  char *str = " This section is comment line";
  int result = isCommentLine(str);
  TEST_ASSERT_EQUAL(result,0);
}

void test_replace_colon_with_dash_comment_line_expect_return_0(void){
  char *str = ":- This section is comment line";
  int result = isCommentLine(str);
  TEST_ASSERT_EQUAL(result,0);
}
