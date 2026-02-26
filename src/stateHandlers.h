#ifndef STATEHANDLERS_H
#define STATEHANDLERS_H

#include "lexerDef.h"


/**
 * Global map linking DFA states to their handler functions and
 * associated metadata (final/retract).  Declared here for access across
 * lexer.c and stateHandlers.c.
 */
extern StateInfo acceptStateMap[MAX_STATES];

/**
 * Initializes the acceptStateMap with function pointers.
 */
void initializeAcceptStateMap();

/* ==========================================================
   STATE HANDLER PROTOTYPES
   Each function returns a tokenInfo package for the Parser.
   ========================================================== */

// Identifier and Keyword Handlers
tokenInfo handle_TK_ID(char* lexeme);
tokenInfo handle_TK_FUNID(char* lexeme);

// Numeric Handlers
tokenInfo handle_TK_NUM(char* lexeme);
tokenInfo handle_TK_RNUM(char* lexeme);

// Operator Handlers

// comparison operators and logical OR
tokenInfo handle_TK_EQ(char* lexeme);
tokenInfo handle_TK_GE(char* lexeme);
tokenInfo handle_TK_GT(char* lexeme);
tokenInfo handle_TK_NE(char* lexeme);
tokenInfo handle_TK_OR(char* lexeme);

// custom ignore/reset handler for state 42
tokenInfo handle_state42(char* lexeme);
tokenInfo handle_TK_ASSIGNOP(char* lexeme);
tokenInfo handle_TK_PLUS(char* lexeme);
tokenInfo handle_TK_MINUS(char* lexeme);
tokenInfo handle_TK_MUL(char* lexeme);
tokenInfo handle_TK_DIV(char* lexeme);
tokenInfo handle_TK_AND(char* lexeme);
tokenInfo handle_TK_OR(char* lexeme);
tokenInfo handle_TK_NOT(char* lexeme);
tokenInfo handle_TK_LT(char* lexeme);
tokenInfo handle_TK_LE(char* lexeme);
tokenInfo handle_TK_EQ(char* lexeme);
tokenInfo handle_TK_GT(char* lexeme);
tokenInfo handle_TK_GE(char* lexeme);
tokenInfo handle_TK_NE(char* lexeme);

// Delimiter Handlers
tokenInfo handle_TK_SQL(char* lexeme);
tokenInfo handle_TK_SQR(char* lexeme);
tokenInfo handle_TK_OP(char* lexeme);
tokenInfo handle_TK_CL(char* lexeme);
tokenInfo handle_TK_COMMA(char* lexeme);
tokenInfo handle_TK_SEM(char* lexeme);
tokenInfo handle_TK_COLON(char* lexeme);
tokenInfo handle_TK_DOT(char* lexeme);
tokenInfo handle_NOT(char* lexeme);
tokenInfo handle_TK_LE(char* lexeme);
tokenInfo handle_TK_LT(char* lexeme);
tokenInfo handle_TK_RUID(char* lexeme);
tokenInfo handle_TK_FIELDID(char* lexeme);

// Special Handlers
tokenInfo handle_TK_ERROR(char* lexeme);

#endif