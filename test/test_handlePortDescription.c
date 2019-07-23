#include "unity.h"
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

void test_handlePortDescription_NeedToImplement(void)
{
  CEXCEPTION_T ex;
  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_test\\file_portTest.bsd";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_test\\file_portTest.bsd";

  Try{
    initBSinfo(bsinfo);
    fileTokenizer = createFileTokenizer(filename);
    BSDL_Parser(bsinfo,fileTokenizer);
    printPortDesc(bsinfo->port);
  }Catch(ex){
    TEST_ASSERT_NOT_NULL(ex);
    //TEST_ASSERT_EQUAL(ERR_PORT_DESCRIPTION, ex->errorCode);
    printf("fail\n" );
    dumpException(ex);
    //dumpTokenErrorMessage(ex,1);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
  freeBsInfo(bsinfo);
}
