#ifndef LEXER_H
#define LEXER_H

#include "lexerDef.h"

void removeComments(char *testcaseFile, char *cleanFile);
FILE *getStream(twinBuffer* buffer, FILE *fp, int bufno);
tokenInfo getNextToken(twinBuffer* B, FILE *fp, lookuptbl* table);

#endif
