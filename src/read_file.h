#ifndef _READ_FILE_H
#define _READ_FILE_H

char* check_file_exists(char *file_name);
char* read_file(char *file_name);
char* getModelName(char *filename);
char *getPackageName(char *filename);
char* getUseStatement(char *filename);
int stringCompare(char **str1, char *str2);
#endif // _READ_FILE_H
