/*
 * Group 33
 * Aaditya Goel 2022B3A70417P
 * Divyansh Jha 2022B3A70438P
 * Prakhar Mittal 2022B3A70426P
 * Mukund Srivastava 2022B3A70562P
 * Aryan Gupta 2022B3A70495P
 * Garv Gulati 2022B4A70685P
 */

#ifndef STATEHANDLERS_H
#define STATEHANDLERS_H

#include "lexerDef.h"

// Shared accept-state map from lexer.
extern StateInfo acceptStateMap[MAX_STATES];

// Fill accept-state handlers and flags.
void initializeAcceptStateMap();

// Identifier handlers.
tokenInfo handle_TK_ID(char* lexeme);
tokenInfo handle_TK_FUNID(char* lexeme);

// Number handlers.
tokenInfo handle_TK_NUM(char* lexeme);
tokenInfo handle_TK_RNUM(char* lexeme);

// Operator handlers.
tokenInfo handle_TK_ASSIGNOP(char* lexeme);
tokenInfo handle_TK_PLUS(char* lexeme);
tokenInfo handle_TK_MINUS(char* lexeme);
tokenInfo handle_TK_MUL(char* lexeme);
tokenInfo handle_TK_DIV(char* lexeme);
tokenInfo handle_TK_AND(char* lexeme);
tokenInfo handle_TK_OR(char* lexeme);
tokenInfo handle_TK_LT(char* lexeme);
tokenInfo handle_TK_LE(char* lexeme);
tokenInfo handle_TK_EQ(char* lexeme);
tokenInfo handle_TK_GT(char* lexeme);
tokenInfo handle_TK_GE(char* lexeme);
tokenInfo handle_TK_NE(char* lexeme);
tokenInfo handle_NOT(char* lexeme);

// Delimiter handlers.
tokenInfo handle_TK_SQL(char* lexeme);
tokenInfo handle_TK_SQR(char* lexeme);
tokenInfo handle_TK_OP(char* lexeme);
tokenInfo handle_TK_CL(char* lexeme);
tokenInfo handle_TK_COMMA(char* lexeme);
tokenInfo handle_TK_SEM(char* lexeme);
tokenInfo handle_TK_COLON(char* lexeme);
tokenInfo handle_TK_DOT(char* lexeme);
tokenInfo handle_TK_RUID(char* lexeme);
tokenInfo handle_TK_FIELDID(char* lexeme);

// Generic lexical error token.
tokenInfo handle_TK_ERROR(char* lexeme);

#endif
