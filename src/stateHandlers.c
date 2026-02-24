#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/* Helper to handle the 'others' retraction logic */
void handleRetraction(twinBuffer *tb, int state) {
    // Retract if the accept state was reached via an 'others' transition 
    // This applies to states like 10, 14, 48, 52, etc.
    retract(tb);
}

/* ==========================================================
   NUMERIC HANDLERS
   ========================================================== */

tokenInfo handle_TK_NUM(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_NUM;
    strcpy(tk.lexeme, lexeme);
    // Fill the named union 'val'
    tk.val.intValue = atoi(lexeme); 
    return tk;
}

tokenInfo handle_TK_RNUM(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_RNUM;
    strcpy(tk.lexeme, lexeme);
    // Fill the named union 'val'
    tk.val.realValue = (float)atof(lexeme); 
    return tk;
}

/* ==========================================================
   IDENTIFIER & KEYWORD HANDLERS
   ========================================================== */

tokenInfo handle_TK_ID(char* lexeme, int state) {
    tokenInfo tk;
    // Check if the lexeme is a reserved keyword
    TokenName kbd = checkKeyword(lexeme); 
    tk.token = kbd;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_FUNID(char* lexeme, int state) {
    tokenInfo tk;
    /* if the lexeme happens to be a reserved word we should honour that
       rather than blindly classifying it as a function identifier.  the
       special case for _main remains. */
    if (strcmp(lexeme, "_main") == 0) {
        tk.token = TK_MAIN;
    } else {
        TokenName kbd = checkKeyword(lexeme);
        if (kbd != TK_ID) {
            tk.token = kbd;      // treat keyword appropriately
        } else {
            tk.token = TK_FUNID;
        }
    }
    strcpy(tk.lexeme, lexeme);
    /* line number will be filled by the caller (getNextToken) */
    tk.lineNo = 0;
    return tk;
}

/* ==========================================================
   OPERATOR & DELIMITER HANDLERS
   ========================================================== */

tokenInfo handle_TK_ASSIGNOP(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_ASSIGNOP;
    strcpy(tk.lexeme, lexeme); // lexeme will be "<---"
    return tk;
}

tokenInfo handle_TK_PLUS(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_PLUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_MINUS(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_MINUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_DOT(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_DOT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_SEM(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_SEM;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

/* ==========================================================
   SPECIAL HANDLERS
   ========================================================== */

tokenInfo handle_TK_EOF(char* lexeme, int state) {
    tokenInfo tk;
    tk.token = TK_EOF;
    strcpy(tk.lexeme, "EOF");
    return tk;
}

tokenInfo handle_TK_AND(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_AND; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_MUL(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_MUL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_DIV(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_DIV; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_SQL(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_SQL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_SQR(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_SQR; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_OP(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_OP; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_CL(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_CL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_COMMA(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_COMMA; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_COLON(char* lexeme, int state) { 
    tokenInfo tk; tk.token = TK_COLON; strcpy(tk.lexeme, lexeme); return tk; 
}