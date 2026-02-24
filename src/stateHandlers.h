#ifndef STATEHANDLERS_H
#define STATEHANDLERS_H

#include "lexerDef.h"

/**
 * Global map linking DFA states to their handler functions. 
 * Declared here for access across lexer.c and stateHandlers.c.
 */
extern StateHandler acceptStateMap[MAX_STATES];

/**
 * Initializes the acceptStateMap with function pointers.
 */
void initializeAcceptStateMap();

/* ==========================================================
   STATE HANDLER PROTOTYPES
   Each function returns a tokenInfo package for the Parser.
   ========================================================== */

// Identifier and Keyword Handlers
tokenInfo handle_TK_ID(char* lexeme, int state);
tokenInfo handle_TK_FUNID(char* lexeme, int state);

// Numeric Handlers
tokenInfo handle_TK_NUM(char* lexeme, int state);
tokenInfo handle_TK_RNUM(char* lexeme, int state);

// Operator Handlers
tokenInfo handle_TK_ASSIGNOP(char* lexeme, int state);
tokenInfo handle_TK_PLUS(char* lexeme, int state);
tokenInfo handle_TK_MINUS(char* lexeme, int state);
tokenInfo handle_TK_MUL(char* lexeme, int state);
tokenInfo handle_TK_DIV(char* lexeme, int state);
tokenInfo handle_TK_AND(char* lexeme, int state);
tokenInfo handle_TK_OR(char* lexeme, int state);
tokenInfo handle_TK_NOT(char* lexeme, int state);
tokenInfo handle_TK_LT(char* lexeme, int state);
tokenInfo handle_TK_LE(char* lexeme, int state);
tokenInfo handle_TK_EQ(char* lexeme, int state);
tokenInfo handle_TK_GT(char* lexeme, int state);
tokenInfo handle_TK_GE(char* lexeme, int state);
tokenInfo handle_TK_NE(char* lexeme, int state);

// Delimiter Handlers
tokenInfo handle_TK_SQL(char* lexeme, int state);
tokenInfo handle_TK_SQR(char* lexeme, int state);
tokenInfo handle_TK_OP(char* lexeme, int state);
tokenInfo handle_TK_CL(char* lexeme, int state);
tokenInfo handle_TK_COMMA(char* lexeme, int state);
tokenInfo handle_TK_SEM(char* lexeme, int state);
tokenInfo handle_TK_COLON(char* lexeme, int state);
tokenInfo handle_TK_DOT(char* lexeme, int state);

// Special Handlers
tokenInfo handle_TK_EOF(char* lexeme, int state);
tokenInfo handle_TK_ERROR(char* lexeme, int state);

#endif