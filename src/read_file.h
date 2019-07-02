#ifndef _READ_FILE_H
#define _READ_FILE_H

char* check_file_exists(char *file_name);
char* read_file(char *file_name);
char *getPackageName(char *filename);
char* getUseStatement(char *filename);

char *obtainComponentNameFromLine(char *str);
int isCommentLine(char *str);
int stringCompare(char **str1, char *str2);

#endif // _READ_FILE_H
