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
#include "handleScanPortIdentification.h"

void setUp(void){}
void tearDown(void){}

//check when file exists
void test_createFileTokenizer_expect_not_null_by_giving_valid_filepath(void){
  char *filename = "test\\file_to_be_read\\STM32F469_F479_WLCSP168.bsd";
  FileTokenizer *fileTokenizer;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//check when file not exists
void test_createFileTokenizer_expect_file_not_exists_by_giving_invalid_filename(void){
  CEXCEPTION_T ex;
  char *filename = "test\\file_to_be_read\\tempz.txt";
  FileTokenizer *fileTokenizer;

  Try{
    fileTokenizer = createFileTokenizer(filename);
    TEST_FAIL_MESSAGE("Expect Error but none thrown");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_FILE_NOT_EXIST, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }
}

//check it can read or not when the file type is PNG
void test_createFileTokenizer_expect_file_not_exists_by_giving_png_type_file(void){
  char *filename = "test\\file_to_be_read\\temp.PNG";
  FileTokenizer *fileTokenizer;
  fileTokenizer = NULL;

  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer);
  TEST_ASSERT_NOT_NULL(fileTokenizer->fileHandler);
  TEST_ASSERT_EQUAL_STRING(fileTokenizer->filename,filename);
  TEST_ASSERT_NOT_NULL(fileTokenizer->tokenizer);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
