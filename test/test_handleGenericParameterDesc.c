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

// INPUT: entity STM32F469_F479_WLCSP168 is
void test_expect_return_genericParameter_when_its_in_correct_order(void){
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168 is\n",fileTokenizer->tokenizer->str);
  char *genericParam = handleGenericParameterDesc(fileTokenizer);

  TEST_ASSERT_NOT_NULL(genericParam);
  TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",genericParam);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entitle STM32F469_F479_WLCSP168 is
void test_handleGenericParameterDesc_by_replace_entity_to_entitle_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entitle STM32F469_F479_WLCSP168 is\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168
void test_handleGenericParameterDesc_by_without_putting_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168 a
void test_handleGenericParameterDesc_by_replace_a_with_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168 a\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168 is not
void test_handleGenericParameterDesc_by_adding_not_behind_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168 is not\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity 21 is
void test_handleGenericParameterDesc_by_replace_the_genericParameter_with_number_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity 21 is\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity
void test_handleGenericParameterDesc_by_giving_entity_only_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_genParam.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_genParam.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity\n",fileTokenizer->tokenizer->str);
    char *genericParam = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
