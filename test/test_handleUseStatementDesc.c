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

// INPUT: use STD_1149_1_2001.all;
void test_expect_return_true_when_useStatement_is_in_correct_order(void){
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  fileTokenizer = createFileTokenizer(filename);
  TEST_ASSERT_EQUAL_STRING("  use STD_1149_1_2001.all;\n",fileTokenizer->tokenizer->str);
  char *useStatement = handleUseStatementDesc(fileTokenizer);

  TEST_ASSERT_NOT_NULL(useStatement);
  TEST_ASSERT_EQUAL_STRING("STD_1149_1_2001",useStatement);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use STD_1149_1_2001.all
void test_handleUseStatementDesc_by_removing_semicolon_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use STD_1149_1_2001.all\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use STD_1149_1_2001.;
void test_handleUseStatementDesc_by_without_putting_all_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use STD_1149_1_2001.;\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use STD_1149_1_2001all;
void test_handleUseStatementDesc_by_without_putting_dot_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use STD_1149_1_2001all;\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use .all;
void test_handleUseStatementDesc_by_without_putting_userPackageName_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use .all;\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: used STD_1149_1_2001.all;
void test_handleUseStatementDesc_by_replace_use_to_used_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  used STD_1149_1_2001.all;\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use 1235 .all;
void test_handleUseStatementDesc_by_replace_userPackageName_to_number_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use 1235 .all;\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: use STD_1149_1_2001
void test_handleUseStatementDesc_by_giving_use_and_usePackageNameOnly_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_useStatement.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_useStatement.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("  use STD_1149_1_2001\n",fileTokenizer->tokenizer->str);
    char *useStatement = handleUseStatementDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    freeException(ex);
  }
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
