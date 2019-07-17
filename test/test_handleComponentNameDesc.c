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
#include "handlePortDescription.h"

void setUp(void){}
void tearDown(void){}

// INPUT: entity STM32F469_F479_WLCSP168 is
void test_expect_return_componentName_when_its_in_correct_order(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);

    TEST_ASSERT_EQUAL(3,fileTokenizer->readLineNo);
    TEST_ASSERT_EQUAL_STRING("STM32F469_F479_WLCSP168",bsinfo->modelName);


  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
//INPUT: entitle STM32F469_F479_WLCSP168 is
void test_handleComponentNameDesc_by_replace_entity_to_entitle_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entitle STM32F469_F479_WLCSP168 is\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168
void test_handleComponentNameDesc_by_without_putting_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168 a
void test_handleComponentNameDesc_by_replace_a_with_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168 a\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity STM32F469_F479_WLCSP168 is not
void test_handleComponentNameDesc_by_adding_not_behind_is_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity STM32F469_F479_WLCSP168 is not\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity 21 is
void test_handleComponentNameDesc_by_replace_the_genericParameter_with_number_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity 21 is\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: entity
void test_handleComponentNameDesc_by_giving_entity_only_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_compName.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_compName.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  entity\n",fileTokenizer->tokenizer->str);
    char *componentName = handleComponentNameDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_COMPONENT_NAME_FORMAT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}*/
