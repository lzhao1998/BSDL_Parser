#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Token.h"
#include "Error.h"
#include "unity.h"
#include "Common.h"
#include "fakeFunc.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "linkedList.h"
#include "handlePortDescription.h"
#include "mock_createAndGetTokenFromFile.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"

void setUp(void){}
void tearDown(void){}

void setupFake(){
  createFileTokenizer_StubWithCallback(fake_createFileTokenizer);
  getTokenFromFile_StubWithCallback(fake_getTokenFromFile);
  skipLine_StubWithCallback(fake_skipLine);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string);
void test_handleGenericParameterDesc_by_giving_generic_default_format_expect_return_empty_string(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  FileTokenizer *fileTokenizer;
  char *filename = "testgenParameter.bsdl";

  char *string[] ={
    " generic(PHYSICAL_PIN_MAP: string:=\"DW\");\n",
    NULL
  };

  setupFake();
  putStringArray(string);

  Try{
  bsinfo =  (BSinfo*)malloc(sizeof(BSinfo));
  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  //TEST_ASSERT_EQUAL_STRING("",bsinfo->packageName);
  }
  Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    dumpException(ex);
    freeException(ex);
  }

  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
//INPUT: generic(PHYSICAL_PIN_MAP: string);
void test_handleGenericParameterDesc_by_giving_generic_default_format_expect_return_empty_string(void){
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\generic_default.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\generic_default.txt";

  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("",bsinfo->packageName);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}*/
/*
//INPUT: generic(PHYSICAL_PIN_MAP: string:="DW");
void test_handleGenericParameterDesc_by_giving_generic_no_default_format_expect_return_string(void){
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\generic_no_default.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\generic_no_default.txt";

  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("DW",bsinfo->packageName);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:="DW"); --hello world
void test_handleGenericParameterDesc_by_inserting_comment_line_behind_expect_throw_error(void){
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\comment_line_behind.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\comment_line_behind.txt";

  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("DW",bsinfo->packageName);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic(: string:="IEE__STD");
void test_handleGenericParameterDesc_by_inserting_invalid_pin_map_name_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\invalid_pin_mapping_name.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\invalid_pin_mapping_name.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT:  generic (PHYSICAL_PIN_MAP string := "DW");
void test_handleGenericParameterDesc_by_removing_colon_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_colon.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_colon.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT:   generic PHYSICAL_PIN_MAP: string := "DW");
void test_handleGenericParameterDesc_by_removing_left_paren_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_left_paren.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_left_paren.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: generic (PHYSICAL_PIN_MAP: string := "DW")
void test_handleGenericParameterDesc_by_removing_semicolon_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_semicolon.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\no_semicolon.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT:  generic (PHYSICAL_PIN_MAP: string); this is not null
void test_handleGenericParameterDesc_by_insert_some_word_after_semiclon_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\not_null_after_semicolon.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\handleGenericParameterDesc\\not_null_after_semicolon.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}*/
