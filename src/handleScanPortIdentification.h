#ifndef _HANDLESCANPORTIDENTIFICATION_H
#define _HANDLESCANPORTIDENTIFICATION_H

# include <stdio.h>
# include "Tokenizer.h"
# include "read_file.h"


void handleScanPortDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer, int type);
void handleTapScanClockDesc(BSinfo *bsinfo, FileTokenizer *fileTokenizer);

#endif // _HANDLESCANPORTIDENTIFICATION_H
