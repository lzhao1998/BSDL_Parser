#ifndef _READ_FILE_H
#define _READ_FILE_H

# include <stdio.h>
# include "Tokenizer.h"


typedef struct{
  char *modelName;
  char *packageName;
  //portFlowDirection *port[];
  char *useStatement;
  //attribute *attr;
  //pinMapString *map[];
  //tapPort *taptap[];
  //instructionOpcode *insOp[];
  //registerAccess *register[];
  //boundaryRegister *bscell[];
}BSinfo;

typedef struct{
  FILE *fileHandler;
  char *filename;
  int readLineNo;
  Tokenizer *tokenizer;
} FileTokenizer;

int checkFileExists(char *file_name);
FileTokenizer *createFileTokenizer(char *filename);
Token *getTokenFromFile(FileTokenizer *fileTokenizer);
char *handleComponentNameDesc(FileTokenizer *fileTokenizer);
char *handleUseStatementDesc(FileTokenizer *fileTokenizer);
char *handleGenericParameterDesc(FileTokenizer *fileTokenizer);
void skipLine(FileTokenizer *fileTokenizer);
void freeFileTokenizer(FileTokenizer *tokenizer) ;

char* read_file(char *file_name);
int isCommentLine(char *str);
int stringCompare(char **str1, char *str2);

#endif // _READ_FILE_H
