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
    // Check specifically for TK_MAIN
    if (strcmp(lexeme, "_main") == 0) {
        tk.token = TK_MAIN;
    } else {
        tk.token = TK_FUNID;
    }
    strcpy(tk.lexeme, lexeme);
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