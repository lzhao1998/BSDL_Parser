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

//INPUT: generic(PHYSICAL_PIN_MAP: string);
void test_handleGenericParameterDesc_by_giving_generic_default_format_expect_return_empty_string(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    printf("package name is %s\n",bsinfo->packageName );
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    //TEST_ASSERT_EQUAL(ERR_PORT_DESCRIPTION, ex->errorCode);
    printf("fail obtain package name\n" );
    dumpException(ex);
    //dumpTokenErrorMessage(ex,1);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}

/*
//INPUT: generic(PHYSICAL_PIN_MAP: string:="hello");
void test_handleGenericParameterDesc_by_giving_generic_no_default_format_expect_return_string(void){
  FileTokenizer *fileTokenizer;
  Token *token;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  fileTokenizer = createFileTokenizer(filename);
  skipLine(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: string:=\"hello\");\n",fileTokenizer->tokenizer->str);

  char *genericParameter = handleGenericParameterDesc(fileTokenizer);
  TEST_ASSERT_EQUAL_STRING("hello",genericParameter);
  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generik(PHYSICAL_PIN_MAP: string:="hello");
void test_handleGenericParameterDesc_by_replace_generic_with_generik_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("generik(PHYSICAL_PIN_MAP: string:=\"hello\");\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(: string:="hello");
void test_handleGenericParameterDesc_by_removing_PHYSICAL_PIN_MAP_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    skipLine(fileTokenizer);
    TEST_ASSERT_EQUAL_STRING("generic(: string:=\"hello\");\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:=);
void test_handleGenericParameterDesc_by_removing_generic_parameter_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 4){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: string:=);\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: :="hello");
void test_handleGenericParameterDesc_by_removing_string_word_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 5){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: :=\"hello\");\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:=hello);
void test_handleGenericParameterDesc_by_changing_type_of_generic_parameter_to_identifier_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 6){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: string:=hello);\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:="hello"); end
void test_handleGenericParameterDesc_by_inserting_end_word_behind_semicolon_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 7){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: string:=\"hello\"); end\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: string:="hello);
void test_handleGenericParameterDesc_by_giving_invalid_string_type_for_generic_parameter_expect_throw_token_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 8){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: string:=\"hello);\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_INVALID_STRING, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}

//INPUT: generic(PHYSICAL_PIN_MAP: strong:="hello");
void test_handleGenericParameterDesc_by_changing_string_into_strong_expect_throw_error(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  Token *token;
  int i = 0;
  //char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";
  char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\test_handleGenericParameter.txt";

  Try{
    fileTokenizer = createFileTokenizer(filename);
    while(i < 9){
      skipLine(fileTokenizer);
      i++;
    }
    TEST_ASSERT_EQUAL_STRING("generic(PHYSICAL_PIN_MAP: strong:=\"hello\");\n",fileTokenizer->tokenizer->str);
    char *genericParameter = handleGenericParameterDesc(fileTokenizer);
    TEST_FAIL_MESSAGE("Expect to fail\n");
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    TEST_ASSERT_EQUAL(ERR_GENERIC_PARAMETER, ex->errorCode);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}*/
