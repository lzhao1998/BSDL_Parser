#ifndef _CREATEANDGETTOKENFROMFILE_H
#define _CREATEANDGETTOKENFROMFILE_H

# include <stdio.h>
# include "Tokenizer.h"
# include "linkedList.h"
# include "read_file.h"

FileTokenizer *createFileTokenizer(char *filename);
Token *getTokenFromFile(FileTokenizer *fileTokenizer);
void skipLine(FileTokenizer *fileTokenizer);

#endif // _CREATEANDGETTOKENFROMFILE_H
