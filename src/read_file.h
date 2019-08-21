#ifndef _READ_FILE_H
#define _READ_FILE_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"

typedef struct {
  char *portId;
  char *haltState;
  char *clock;
  int type;
}tapScanObj;

typedef struct{
  char *cellNumber;
  char *cellName;
  char *portId;
  char *function;
  char *safeBit;
  char *ccell;
  char *disval;     // disable value
  char *disres;   // disable result
}boundaryRegister;

typedef struct{
  char *modelName;
  char *packageName;
  LinkedList *port;
  LinkedList *pinMapping;
  char *useStatement;
  char *componentConformance;
  int instructionLength;
  int boundaryLength;
  tapScanObj *tapScanClk;
  tapScanObj *tapScanIn;
  tapScanObj *tapScanMode;
  tapScanObj *tapScanOut;
  tapScanObj *tapScanReset;
  LinkedList *boundaryReg;
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

typedef enum {
  ENTITY,       //0
  GENERIC,      //1
  PORT,         //2
  USE,          //3
  ATTRIBUTE,    //4
  END,          //5
  ERROR_TYPE,   //6
} DescriptionName;

typedef enum {
  COMPONENT_CONFORMANCE,  //0
  PIN_MAP,                //1
  TAP_SCAN_CLOCK,         //2
  TAP_SCAN_IN,            //3
  TAP_SCAN_MODE,          //4
  TAP_SCAN_OUT,           //5
  TAP_SCAN_RESET,         //6
  COMPLIANCE_PATTERNS,    //7
  INSTRUCTION_LENGTH,     //8
  INSTRUCTION_OPCODE,     //9
  INSTRUCTION_CAPTURE,    //10
  IDCODE_REGISTER,        //11
  REGISTER_ACCESS,        //12
  BOUNDARY_LENGTH,        //13
  BOUNDARY_REGISTER,      //14
  DESIGN_WARNING,         //15
  ERROR_ATTRIBUTE,             //16
} AttributeName;


tapScanObj *tapScanObjInit();

void checkAndSkipCommentLine(FileTokenizer *fileTokenizer);
void handleDescSelector(char *str, FileTokenizer *fileTokenizer, BSinfo *bsinfo);
void handleAttributeSelector(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
int checkFileExists(char *file_name);

void handleComponentConformanceDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
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
void freeLinkedList(void *linkedlist);
void createCallBackToken(Tokenizer *tokenizer, Token *token);
#endif // _READ_FILE_H
