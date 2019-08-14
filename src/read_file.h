#ifndef _READ_FILE_H
#define _READ_FILE_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"

typedef struct {
  char *portId;
  char *haltState;
  char *clock;
}tapScanClock;

typedef struct{
  char *modelName;
  char *packageName;
  LinkedList *port;
  LinkedList *pinMapping;
  char *useStatement;
  char *componentConformace;
  int instructionLength;
  int boundaryLength;
  tapScanClock *tapScanClk;
  char *tapScanIn;
  char *tapScanMode;
  char *tapScanOut;
  char *tapScanReset;
  //instructionOpcode *insOp[];
  //registerAccess *register[];
  //boundaryRegister *bscell[];
}BSinfo;

typedef struct {
  char *portName;
  int pinType;
  int bitType;
  int integer1;
  int rangeType;
  int integer2;
}portDesc;

typedef struct{
  FILE *fileHandler;
  char *filename;
  int readLineNo;
  Tokenizer *tokenizer;
} FileTokenizer;



tapScanClock *tapScanClockInit();

void checkAndSkipCommentLine(FileTokenizer *fileTokenizer);
void handleDescSelector(char *str, FileTokenizer *fileTokenizer, BSinfo *bsinfo);
void handleAttributeSelector(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
int checkFileExists(char *file_name);


void handleComponentNameDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void handleUseStatementDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void handleGenericParameterDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);

int checkVHDLidentifier(char *str);
void freeFileTokenizer(void *tokenizer);
BSinfo *initBSinfo();
void freeBsInfo(void *bsinfo);
char *getString(FileTokenizer *fileTokenizer, char *strArr[], int *tokenType, int errorCode, int length, int type);
int handleInstructionAndBoundaryLength(FileTokenizer *fileTokenizer,int errorCode, char *compName, int type);
int checkStandardPackageName(char *str);

void createCallBackToken(Tokenizer *tokenizer, Token *token);
#endif // _READ_FILE_H
