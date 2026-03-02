/*
 * Group 33
 * Aaditya Goel 2022B3A70417P
 * Divyansh Jha 2022B3A70438P
 * Prakhar Mittal 2022B3A70426P
 * Mukund Srivastava 2022B3A70562P
 * Aryan Gupta 2022B3A70495P
 * Garv Gulati 2022B4A70685P
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "lexerDef.h"

// Allocate and initialize twin-buffer lexer state.
twinBuffer* initializeLexer(FILE *fp);

// Refill inactive half of twin buffer.
void refillBuffer(twinBuffer *tb);

// Read next character from twin buffer stream.
int getNextChar(twinBuffer *tb);

// Move input cursor one step back.
void retract(twinBuffer *tb);

// Return next token from input.
tokenInfo getNextToken(twinBuffer *tb);

// Copy file to output while removing '%' comments.
void removeComments(char *inputFile, char *outputFile);

// Check if lexeme is a reserved keyword.
TokenName checkKeyword(char *lexeme);

// Clear lexeme buffer to empty string.
void resetLexeme(char *lexeme);

// Append one character to lexeme buffer.
void addCharToLexeme(char *lexeme, char c);

// Print token in project output format.
void printToken(tokenInfo tk);

// Populate accept-state handlers for DFA.
void initializeAcceptStateMap() ;

#endif
