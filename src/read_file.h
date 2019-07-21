#ifndef _READ_FILE_H
#define _READ_FILE_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"


typedef struct{
  char *modelName;
  char *packageName;
  LinkedList *port;
  char *useStatement;
  //attribute *attr;
  //pinMapString *map[];
  //tapPort *taptap[];
  //instructionOpcode *insOp[];
  //registerAccess *register[];
  //boundaryRegister *bscell[];
}BSinfo;

typedef struct {
  char *portName;
  int pinType;
  int bitType;
  int integer1;
  int integer2;
  int rangeType;
}portDesc;

typedef struct{
  FILE *fileHandler;
  char *filename;
  int readLineNo;
  Tokenizer *tokenizer;
} FileTokenizer;


char *symbolChar[] = {
  "(",  //0   LEFTPAREN
  ")",  //1   RIGHTPAREN
  ";",  //2   SEMICOLON
  ":",  //3   COLON
  ",",  //4   COMMA
  "-",  //5   DASH
};

void checkAndSkipCommentLine(FileTokenizer *fileTokenizer);
int compareDescriptionName(char *str);
void BSDL_Parser(BSinfo *bsinfo, FileTokenizer *fileTokenizer);
int checkFileExists(char *file_name);
FileTokenizer *createFileTokenizer(char *filename);
Token *getTokenFromFile(FileTokenizer *fileTokenizer);

char *handleComponentNameDesc(FileTokenizer *fileTokenizer);
char *handleUseStatementDesc(FileTokenizer *fileTokenizer);
char *handleGenericParameterDesc(FileTokenizer *fileTokenizer);
void skipLine(FileTokenizer *fileTokenizer);
int checkVHDLidentifier(char *str);
void freeFileTokenizer(FileTokenizer *tokenizer);
void initBSinfo(BSinfo *bsinfo);
void freeBsInfo(BSinfo *bsinfo);


#endif // _READ_FILE_H
