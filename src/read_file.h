#ifndef _READ_FILE_H
#define _READ_FILE_H

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

int check_file_exists(char *file_name);
char* read_file(char *file_name);

BSinfo *getBSinfo(char *filename);
char *obtainComponentNameFromLine(char *str);
int isCommentLine(char *str);
int stringCompare(char **str1, char *str2);


#endif // _READ_FILE_H
