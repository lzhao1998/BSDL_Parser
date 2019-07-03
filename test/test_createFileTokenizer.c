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

//check when file exists
void test_createFileTokenizer_expect_not_null_by_giving_valid_filepath(void){
  //change pls
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_be_read\\STM32F469_F479_WLCSP168.bsd";
  FileTokenizer *fileTokenizer;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);
}

//check when file not exists
void test_createFileTokenizer_expect_file_not_exists_by_giving_invalid_filename(void){
  CEXCEPTION_T ex;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_be_read\\hey.txt";
  FileTokenizer *fileTokenizer;
  fileTokenizer = NULL;

  Try{
    fileTokenizer = createFileTokenizer(filename);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_FILE_NOT_EXISTS, ex->errorCode);
  }
  freeToken(ex->data);
  freeFileTokenizer(fileTokenizer);
}

//check when the file type is cannot be read. exaple : PNG, gif, mp4
void test_createFileTokenizer_expect_file_not_exists_by_giving_png_type_file(void){
  CEXCEPTION_T ex;
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\\file_to_be_read\\temp.PNG";
  FileTokenizer *fileTokenizer;
  fileTokenizer = NULL;

  Try{
    fileTokenizer = createFileTokenizer(filename);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_FILE_NOT_EXISTS, ex->errorCode);
  }
  freeToken(ex->data);
  freeFileTokenizer(fileTokenizer);
}
