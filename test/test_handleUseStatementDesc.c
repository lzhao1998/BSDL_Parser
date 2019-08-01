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
#include "linkedList.h"
#include "handlePortDescription.h"

void setUp(void){}
void tearDown(void){}

// INPUT: use STD_1149_1_2001.all;
void test_expect_return_true_when_useStatement_is_in_correct_order(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\normal_useStatement_format.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\normal_useStatement_format.txt";

  initBSinfo(bsinfo);
  fileTokenizer = createFileTokenizer(filename);
  BSDL_Parser(bsinfo,fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("STD_1149_1_2001",bsinfo->useStatement);

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001.all
void test_handleUseStatementDesc_by_removing_semicolon_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\no_semicolon.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\no_semicolon.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001.;
void test_handleUseStatementDesc_by_without_putting_all_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\no_all.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\no_all.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001all;
void test_handleUseStatementDesc_by_without_putting_dot_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\no_period.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\no_period.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use .all;
void test_handleUseStatementDesc_by_without_putting_userPackageName_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\no_useStatement.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\no_useStatement.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2001
void test_handleUseStatementDesc_by_giving_use_and_usePackageNameOnly_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\only_useStatement.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\only_useStatement.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1149_1_2__001.all;
void test_handleUseStatementDesc_by_giving_invalid_packageName_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\invalid_useStatement.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\invalid_useStatement.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

//INPUT: use STD_1123_1_2_001.all;
void test_handleUseStatementDesc_by_giving_invalid_package_name_expect_throw_error(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\getUseStatement\\invalid_package_name.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\getUseStatement\\invalid_package_name.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
