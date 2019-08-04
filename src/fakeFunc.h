#ifndef _FAKEFUNC_H
#define _FAKEFUNC_H

# include <stdio.h>
# include "Tokenizer.h"
# include "read_file.h"

FileTokenizer *fake_createFileTokenizer(char *filename, int count);
Token *fake_getTokenFromFile(FileTokenizer *fileTokenizer, int count);
void fake_skipLine(FileTokenizer *fileTokenizer, int count);
void putStringArray(char *strArr[]);

#endif // _FAKEFUNC_H
