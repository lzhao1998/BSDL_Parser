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


void test_handleInstructionAndBoundaryLength(void){
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\testing.txt";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\testing.txt";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    printf("instruction length is %d\n", bsinfo->instructionLength);
    printf("boundary length is %d\n", bsinfo->boundaryLength);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    //TEST_ASSERT_EQUAL(ERR_USE_STATEMENT, ex->errorCode);
    printf("readline no is %d\n",fileTokenizer->readLineNo );
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
