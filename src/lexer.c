#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexerDef.h"
#include "stateHandlers.h"

/* Definition of the global accept state map */
StateHandler acceptStateMap[MAX_STATES] = { NULL };

/* Helper to map characters to the transition matrix columns based on CSV header */
InputType mapCharToEnum(char ch) {
    if (ch == EOF) return EOF_TYPE;
    if (ch == ' ' || ch == '\t' || ch == '\r') return DELIM;
    if (ch == '\n') return NEWLINE;
    
    // Priority: digit1 (2-7) must be checked before general digit (0-9)
    if (ch >= '2' && ch <= '7') return DIGIT1;
    if (ch >= '0' && ch <= '9') return DIGIT;
    
    // Character classes for identifiers as defined in regex
    if (ch == 'a' || ch == 'b') return ALPHA;
    if (ch == 'c' || ch == 'd') return ALPHA1;
    if (ch >= 'e' && ch <= 'z') return ALPHA2;
    if ((ch >= 'A' && ch <= 'Z')) return ALPHABET;

    switch (ch) {
        case '<': return LESS_THAN;
        case '>': return GREATER_THAN;
        case '=': return EQUAL;
        case '-': return MINUS;
        case '.': return DOT;
        case ';': return SEMICOLON;
        case ',': return COMMA;
        case '@': return AT_SYMBOL;
        case '#': return HASH;
        case '%': return PERCENT;
        case '+': return PLUS;
        case '/': return FORWARD_SLASH;
        case '*': return ASTERISK;
        case '!': return EXCLAMATION;
        case ':': return COLON;
        case '(': return OPEN_PAREN;
        case ')': return CLOSE_PAREN;
        case '_': return UNDERSCORE;
        case '&': return AMPERSAND;
        default: return INPUT_COUNT; 
    }
}

twinBuffer* initializeLexer(FILE *fp) {
    twinBuffer *tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    tb->fp = fp;
    tb->forward = 0;
    tb->lexemeBegin = 0;
    tb->currentBuffer = 1;
    
    // Initial fill of buffer1
    memset(tb->buffer1, 0, BUFFER_SIZE);
    fread(tb->buffer1, 1, BUFFER_SIZE, tb->fp);
    return tb;
}

void refillBuffer(twinBuffer *tb) {
    if (tb->currentBuffer == 1) {
        memset(tb->buffer2, 0, BUFFER_SIZE);
        fread(tb->buffer2, 1, BUFFER_SIZE, tb->fp);
        tb->currentBuffer = 2;
    } else {
        memset(tb->buffer1, 0, BUFFER_SIZE);
        fread(tb->buffer1, 1, BUFFER_SIZE, tb->fp);
        tb->currentBuffer = 1;
    }
}

char getNextChar(twinBuffer *tb) {
    char ch;
    if (tb->currentBuffer == 1) {
        ch = tb->buffer1[tb->forward];
    } else {
        ch = tb->buffer2[tb->forward];
    }
    
    tb->forward++;
    if (tb->forward == BUFFER_SIZE) {
        refillBuffer(tb);
        tb->forward = 0;
    }
    return ch;
}

void retract(twinBuffer *tb) {
    tb->forward--;
    if (tb->forward < 0) {
        tb->forward = BUFFER_SIZE - 1;
        tb->currentBuffer = (tb->currentBuffer == 1) ? 2 : 1;
    }
}

tokenInfo getNextToken(twinBuffer *tb) {
    int currentState = 1;
    int nextState;
    char ch;
    tokenInfo tk;
    int lp = 0;
    static int lineNo = 1;

    memset(tk.lexeme, 0, MAX_LEXEME_LEN);

    while (1) {
        ch = getNextChar(tb);
        InputType input = mapCharToEnum(ch);
        
        // Handle EOF immediately
        if (ch == EOF) {
            tk.token = TK_EOF;
            strcpy(tk.lexeme, "EOF");
            tk.lineNo = lineNo;
            return tk;
        }

        nextState = transitionMatrix[currentState][input];

        /* --- ERRATA HANDLING --- */

        // 1. Lexical Errata: <- should be reported as error
        if (currentState == 2 && ch == '-') {
            tk.token = TK_ERROR;
            strcpy(tk.lexeme, "<-");
            tk.lineNo = lineNo;
            return tk;
        }

        // 2. Lexical Errata: 23.abc handling (Split TK_NUM, TK_DOT, TK_FIELDID)
        // Accessing via tk.val.intValue to resolve unnamed union field error
        if (currentState == 52 && input == DOT) {
            retract(tb);
            tk.token = TK_NUM;
            tk.lexeme[lp] = '\0';
            tk.lineNo = lineNo;
            tk.val.intValue = atoi(tk.lexeme); 
            return tk;
        }

        /* --- ACCEPT STATE LOGIC --- */
        // Check if the current transition leads to an accept state
        if (acceptStateMap[nextState] != NULL) {
            tk.lineNo = lineNo;
            // The handler will populate the lexeme and token type
            return acceptStateMap[nextState](tk.lexeme, nextState);
        }

        // Update line count
        if (ch == '\n') lineNo++;

        // Append to lexeme if not a delimiter at start state
        if (!(currentState == 1 && input == DELIM)) {
            if (lp < MAX_LEXEME_LEN - 1) {
                tk.lexeme[lp++] = ch;
                tk.lexeme[lp] = '\0';
            }
        } else {
            // Stay in state 1 and move lexemeBegin forward
            tb->lexemeBegin = tb->forward;
        }

        currentState = nextState;
    }
}

void initializeAcceptStateMap() {
    // Initialization of all entries to NULL is already handled by { NULL }
    
    // --- Operator Accept States ---
    acceptStateMap[5]  = handle_TK_ASSIGNOP;  // <---
    acceptStateMap[15] = handle_TK_PLUS;      // +
    acceptStateMap[16] = handle_TK_MINUS;     // -
    acceptStateMap[17] = handle_TK_MUL;       // *
    acceptStateMap[18] = handle_TK_DIV;       // /
    acceptStateMap[37] = handle_TK_AND;       // &&&

    // --- Identifier and Keyword Accept States ---
    acceptStateMap[10] = handle_TK_FUNID;     // Functions/Main
    acceptStateMap[14] = handle_TK_ID;        // ID using digit1 (2-7)
    acceptStateMap[48] = handle_TK_ID;        // Generic Identifier

    // --- Numeric Accept States ---
    acceptStateMap[61] = handle_TK_NUM;       // Integer
    acceptStateMap[62] = handle_TK_RNUM;      // Real Number

    // --- Delimiter Accept States ---
    acceptStateMap[22] = handle_TK_SEM;       // ;
    acceptStateMap[21] = handle_TK_COMMA;     // ,
    acceptStateMap[20] = handle_TK_SQL;       // [
    acceptStateMap[19] = handle_TK_SQR;       // ]
    acceptStateMap[26] = handle_TK_OP;        // (
    acceptStateMap[25] = handle_TK_CL;        // )
    acceptStateMap[23] = handle_TK_COLON;     // :
    acceptStateMap[24] = handle_TK_DOT;       // .

    // --- Special Accept States ---
    acceptStateMap[44] = handle_TK_EOF;       // End of file
    
    /* Note: State 64 handles comment removal. It typically does not 
       return a token but resets the DFA to State 1.
    */
}