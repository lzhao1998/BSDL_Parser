#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "unity.h"
#include "Token.h"
#include "Error.h"
#include "read_file.h"
#include "Tokenizer.h"
#include "Exception.h"
#include "getStrToken.h"
#include "handlePinMappingDesc.h"
#include "handlePortDescription.h"
#include "createAndGetTokenFromFile.h"
#include "handleScanPortIdentification.h"
#include "handleBoundaryRegisterDesc.h"

void handleBR(FileTokenizer *fileTokenizer, LinkedList *list){

}

typedef struct{
  int num;
  char *cell;
  char *port;
  char *function;
  char *safe;
  int ccell;
  int disval;
  char *ralt;
}boundaryregister;
