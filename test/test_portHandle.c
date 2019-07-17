#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "Common.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "read_file.h"
#include "handlePortDescription.h"

void setUp(void){}
void tearDown(void){}

void test_port_access(void){
  CEXCEPTION_T ex;
  FileTokenizer *fileTokenizer;
  char *filename = "C:\\Users\\lzhao\\Documents\\haohao\\BSDL_Parser\\file_to_be_read\\testForPort.bsd";
  //char *filename = "C:\\ZheHao\\Project\\C\\BSDL_Parser\\file_to_be_read\\testForPort.bsd";
  fileTokenizer = createFileTokenizer(filename);

  BSinfo *bsinfo;
  bsinfo = (BSinfo*)malloc(sizeof(BSinfo));

  Try{
    BSDL_Parser(bsinfo,fileTokenizer);
  }Catch(ex){
    //dumpTokenErrorMessage(ex, 1);
    dumpException(ex);
    freeException(ex);
  }

  fclose(fileTokenizer->fileHandler);
  freeFileTokenizer(fileTokenizer);
}
