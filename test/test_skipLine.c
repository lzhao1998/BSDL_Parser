#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handleScanPortIdentification.h"

void setUp(void){}
void tearDown(void){}

/* Input:
**  hi, how are you?
**  i am fine
*/
void test_skipLine_expect_pass(void){
  FileTokenizer *fileTokenizer;
  char *filename = "test\\file_to_test\\skipLine\\2_line.txt";
  fileTokenizer = createFileTokenizer(filename);

  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("i am fine",fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(2,fileTokenizer->readLineNo);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

/*Input: goodbye world*/
void test_skipLine_by_giving_only_1_line_file_expect_get_tokenizer_str_null(void){
  FileTokenizer *fileTokenizer;
  char *filename = "test\\file_to_test\\skipLine\\goodbye_world.txt";
  fileTokenizer = createFileTokenizer(filename);

  skipLine(fileTokenizer);
  TEST_ASSERT_NULL(fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(2,fileTokenizer->readLineNo);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

/*Input:
**  this is 1st line
**  this is 2nd line
**  3rd line here
**  4th line is the last line.
*/
void test_skipLine_multiple_times_expect_pass(void){
  FileTokenizer *fileTokenizer;
  char *filename = "test\\file_to_test\\skipLine\\multiple_line.txt";
  fileTokenizer = createFileTokenizer(filename);

  TEST_ASSERT_EQUAL_STRING("this is 1st line\n",fileTokenizer->tokenizer->str);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("this is 2nd line\n",fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(2,fileTokenizer->readLineNo);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("3rd line here\n",fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(3,fileTokenizer->readLineNo);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("4th line is the last line.",fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(4,fileTokenizer->readLineNo);
  skipLine(fileTokenizer);
  TEST_ASSERT_NULL(fileTokenizer->tokenizer->str);
  TEST_ASSERT_EQUAL(5,fileTokenizer->readLineNo);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
