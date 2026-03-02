/*
 * Group 33
 * Aaditya Goel 2022B3A70417P
 * Divyansh Jha 2022B3A70438P
 * Prakhar Mittal 2022B3A70426P
 * Mukund Srivastava 2022B3A70562P
 * Aryan Gupta 2022B3A70495P
 * Garv Gulati 2022B4A70685P
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

// Utility helpers.
void handleRetraction(twinBuffer *tb) {
    retract(tb);
}

// Numeric tokens.
tokenInfo handle_TK_NUM(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_NUM;
    strcpy(tk.lexeme, lexeme);
    tk.val.intValue = atoi(lexeme);
    return tk;
}

tokenInfo handle_TK_RNUM(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_RNUM;
    strcpy(tk.lexeme, lexeme);
    tk.val.realValue = (float)atof(lexeme);
    return tk;
}

// Identifier and keyword-like tokens.
tokenInfo handle_TK_ID(char* lexeme) {
    tokenInfo tk;
    TokenName kbd = checkKeyword(lexeme);
    tk.token = kbd;

    if (kbd == TK_ID && strlen(lexeme) > 20) {
        tk.token = TK_LENGTH_ERROR;
    }

    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_FUNID(char* lexeme) {
    tokenInfo tk;

    if (strcmp(lexeme, "_main") == 0) {
        tk.token = TK_MAIN;
    } else {
        TokenName kbd = checkKeyword(lexeme);
        if (kbd != TK_ID) {
            tk.token = kbd;
        } else {
            tk.token = TK_FUNID;
        }
    }

    strcpy(tk.lexeme, lexeme);
    tk.lineNo = 0;
    return tk;
}

// Arithmetic and assignment operators.
tokenInfo handle_TK_ASSIGNOP(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ASSIGNOP;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Build plus token.
tokenInfo handle_TK_PLUS(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_PLUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Build not token.
tokenInfo handle_NOT(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_NOT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Build minus token.
tokenInfo handle_TK_MINUS(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_MINUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_MUL(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_MUL;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_DIV(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_DIV;
    strcpy(tk.lexeme, lexeme);
    return tk;
}
tokenInfo handle_TK_AND(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_AND;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Relational and logical operators.
tokenInfo handle_TK_EQ(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_EQ;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_GE(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_GE;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_GT(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_GT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_NE(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_NE;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_OR(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_OR;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_LE(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_LE;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_LT(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_LT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Delimiters and punctuation.
tokenInfo handle_TK_DOT(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_DOT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_SEM(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_SEM;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_SQL(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_SQL;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_SQR(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_SQR;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_OP(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_OP;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_CL(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_CL;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_COMMA(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_COMMA;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_COLON(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_COLON;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

// Misc token constructors.
tokenInfo handle_TK_RUID(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_RUID;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_ERROR(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ERROR;
    strcpy(tk.lexeme, lexeme);
    tk.lineNo = 0;
    return tk;
}

tokenInfo handle_TK_FIELDID(char* lexeme) {
    tokenInfo tk;
    TokenName kbd = checkKeyword(lexeme);

    if (kbd != TK_ID) {
        tk.token = kbd;
    } else {
        tk.token = TK_FIELDID;
        if (strlen(lexeme) > 20) {
            tk.token = TK_LENGTH_ERROR;
        }
    }

    strcpy(tk.lexeme, lexeme);
    tk.lineNo = 0;
    return tk;
}
