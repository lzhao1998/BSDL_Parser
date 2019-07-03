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
  Tokenizer *tokenizer;
} FileTokenizer;

int checkFileExists(char *file_name);
void freeFileTokenizer(void *tokenizer) ;
Token *getTokenFromFile(FileTokenizer *fileTokenizer);

char* read_file(char *file_name);
BSinfo *getBSinfo(char *filename);
char *obtainComponentNameFromLine(char *str);
int isCommentLine(char *str);
int stringCompare(char **str1, char *str2);

#endif // _READ_FILE_H
