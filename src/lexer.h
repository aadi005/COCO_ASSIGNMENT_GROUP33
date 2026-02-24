#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"
#include "stateHandlers.h"

/* Initializes the twin buffer and prepares input stream */
twinBuffer* initializeLexer(FILE *fp);

/* Refills the appropriate buffer block */
void refillBuffer(twinBuffer *tb);

/* Returns next character from twin buffer */
char getNextChar(twinBuffer *tb);

/* Retracts forward pointer by one position */
void retract(twinBuffer *tb);

/* Returns next token from input stream */
tokenInfo getNextToken(twinBuffer *tb);

/* Removes comments from source file and prints cleaned code */
void removeComments(char *inputFile, char *outputFile);

/* Checks if lexeme is a keyword and updates token type */
TokenName checkKeyword(char *lexeme);

/* Resets lexeme buffer */
void resetLexeme(char *lexeme);

/* Adds character to lexeme buffer */
void addCharToLexeme(char *lexeme, char c);

/* Prints token (for Option 2 in driver) */
void printToken(tokenInfo tk);

void initializeAcceptStateMap() ;


#endif