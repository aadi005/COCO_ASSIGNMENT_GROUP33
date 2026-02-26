#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/* dummy/no-op handler used to populate the state map for non-final
   states.  It simply returns an empty error token and is never invoked by
   the lexer when isFinal is false, but having a real function avoids
   null-pointer checks elsewhere. */
tokenInfo handle_noop(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ERROR;
    tk.lexeme[0] = '\0';
    tk.lineNo = 0;
    return tk;
}

/* Helper to handle the 'others' retraction logic */
void handleRetraction(twinBuffer *tb) {
    // Retract if the accept state was reached via an 'others' transition 
    // This applies to states like 10, 14, 48, 52, etc.
    retract(tb);
}

/* ==========================================================
   NUMERIC HANDLERS
   ========================================================== */

tokenInfo handle_TK_NUM(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_NUM;
    strcpy(tk.lexeme, lexeme);
    // Fill the named union 'val'
    tk.val.intValue = atoi(lexeme); 
    return tk;
}

tokenInfo handle_TK_RNUM(char* lexeme) {
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

tokenInfo handle_TK_ID(char* lexeme) {
    tokenInfo tk;
    // Check if the lexeme is a reserved keyword
    TokenName kbd = checkKeyword(lexeme); 
    tk.token = kbd;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_FUNID(char* lexeme) {
    tokenInfo tk;
    extern TokenName checkKeyword(char *lexeme);
    
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

tokenInfo handle_TK_ASSIGNOP(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ASSIGNOP;
    strcpy(tk.lexeme, lexeme); // lexeme will be "<---"
    return tk;
}

tokenInfo handle_TK_PLUS(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_PLUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_NOT(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_NOT;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

tokenInfo handle_TK_MINUS(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_MINUS;
    strcpy(tk.lexeme, lexeme);
    return tk;
}

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

/* ==========================================================
   SPECIAL HANDLERS
   ========================================================== */

tokenInfo handle_TK_EOF(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_EOF;
    strcpy(tk.lexeme, "EOF");
    return tk;
}

tokenInfo handle_TK_AND(char* lexeme) { 
    tokenInfo tk; tk.token = TK_AND; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_MUL(char* lexeme) { 
    tokenInfo tk; tk.token = TK_MUL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_DIV(char* lexeme) { 
    tokenInfo tk; tk.token = TK_DIV; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_SQL(char* lexeme) { 
    tokenInfo tk; tk.token = TK_SQL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_SQR(char* lexeme) { 
    tokenInfo tk; tk.token = TK_SQR; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_OP(char* lexeme) { 
    tokenInfo tk; tk.token = TK_OP; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_CL(char* lexeme) { 
    tokenInfo tk; tk.token = TK_CL; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_COMMA(char* lexeme) { 
    tokenInfo tk; tk.token = TK_COMMA; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_COLON(char* lexeme) { 
    tokenInfo tk; tk.token = TK_COLON; strcpy(tk.lexeme, lexeme); return tk; 
}

/* comparison / logical operator handlers */
tokenInfo handle_TK_EQ(char* lexeme) { tokenInfo tk; tk.token = TK_EQ; strcpy(tk.lexeme, lexeme); return tk; }
tokenInfo handle_TK_GE(char* lexeme) { tokenInfo tk; tk.token = TK_GE; strcpy(tk.lexeme, lexeme); return tk; }
tokenInfo handle_TK_GT(char* lexeme) { tokenInfo tk; tk.token = TK_GT; strcpy(tk.lexeme, lexeme); return tk; }
tokenInfo handle_TK_NE(char* lexeme) { tokenInfo tk; tk.token = TK_NE; strcpy(tk.lexeme, lexeme); return tk; }
tokenInfo handle_TK_OR(char* lexeme) { tokenInfo tk; tk.token = TK_OR; strcpy(tk.lexeme, lexeme); return tk; }

/* custom DFA state 42 handler -- suppress output and clear lexeme */
tokenInfo handle_state42(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ERROR;   /* placeholder token */
    tk.lexeme[0] = '\0';  /* ensure lexeme reset */
    tk.lineNo = 0;
    return tk;
}

tokenInfo handle_TK_LE(char* lexeme) { 
    tokenInfo tk; tk.token = TK_LE; strcpy(tk.lexeme, lexeme); return tk; 
}
tokenInfo handle_TK_LT(char* lexeme) { 
    tokenInfo tk; tk.token = TK_LT; strcpy(tk.lexeme, lexeme); return tk; 
}

tokenInfo handle_TOK(char* lexeme) { 
    tokenInfo tk; tk.token = TOK; strcpy(tk.lexeme, lexeme); return tk; 
}

tokenInfo handle_TK_RUID(char* lexeme) { 
    tokenInfo tk; tk.token = TK_RUID; strcpy(tk.lexeme, lexeme); return tk; 
}

tokenInfo handle_TK_ERROR(char* lexeme) {
    tokenInfo tk;
    tk.token = TK_ERROR;
    strcpy(tk.lexeme, lexeme);
    tk.lineNo = 0; // line number will be filled by caller
    return tk;
}