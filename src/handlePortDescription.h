#ifndef _HANDLEPORTDESCRIPTION_H
#define _HANDLEPORTDESCRIPTION_H

# include "Tokenizer.h"
# include "read_file.h"



void handlePortDesc(FileTokenizer *fileTokenizer,BSinfo *bsinfo);
void handlePinSpec(FileTokenizer *fileTokenizer, BSinfo *bsinfo);
void initPortDesc(portDesc *portdesc);
// int getPinType(Token *token);
// int getBitType(Token *token);
// int getRangeType(Token *token);

#endif // _HANDLEPORTDESCRIPTION_H
