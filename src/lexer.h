#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"


twinBuffer* initializeLexer(FILE *fp);


void refillBuffer(twinBuffer *tb);


int getNextChar(twinBuffer *tb);


void retract(twinBuffer *tb);


tokenInfo getNextToken(twinBuffer *tb);


void removeComments(char *inputFile, char *outputFile);


TokenName checkKeyword(char *lexeme);


void resetLexeme(char *lexeme);


void addCharToLexeme(char *lexeme, char c);


void printToken(tokenInfo tk);

void initializeAcceptStateMap() ;



#endif