#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Token.h"
#include "Error.h"
#include "unity.h"
#include "Common.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"

void setUp(void){}
void tearDown(void){}


void test_expect_return_genericParameter_when_its_in_correct_order(void){
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  fileTokenizer = createFileTokenizer(filename);
  char *genericParam = handleGenericParameterDesc(fileTokenizer);

  TEST_ASSERT_NOT_NULL(genericParam);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",genericParam);
  freeFileTokenizer(fileTokenizer);
}
