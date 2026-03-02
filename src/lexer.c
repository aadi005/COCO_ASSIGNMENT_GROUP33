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
#include "lexerDef.h"
#include "stateHandlers.h"
#include <ctype.h>
#include <stdbool.h>

// Accept-state metadata: handler + final/retract flags.
StateInfo acceptStateMap[MAX_STATES] = { {NULL, false, false} };
// DFA transition table generated from state design
int transitionMatrix[MAX_STATES][INPUT_COUNT] = {
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {52, 65, 65, 11, 9, 65, 41, 43, 44, 2, 32, 28, 16, 24, 22, 21, 38, 49, 63, 15, 18, 17, 30, 35, 65, 25, 23, 26, 45, 20, 19, 27},
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    {66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 4, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 5, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {10, 10, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {10, 12, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    {14, 13, 14, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14},
    {14, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 29, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 31, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 36, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 37, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 39, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 40, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {42, 42, 42, 42, 42, 42, 41, 41, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 46, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {47, 48, 48, 48, 48, 46, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
    {47, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 50, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {51, 51, 50, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {52, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 53, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
    {54, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {55, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 56, 59, 59, 59, 59, 59, 59, 59},
    {60, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 57, 65, 65, 65, 65, 65, 65, 57, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {60, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {62, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {63, 63, 63, 63, 63, 63, 63, 64, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
};

// Map raw char to DFA input column.
InputType mapCharToEnum(int ch) {
    // treat '\0' as sentinel/EOF in twin buffers
    if (ch == EOF || ch == '\0') return EOF_TYPE;
    if (ch == ' ' || ch == '\t' || ch == '\r') return DELIM;
    if (ch == '\n') return NEWLINE;

    if (ch >= '2' && ch <= '7') return DIGIT1;
    if (ch >= '0' && ch <= '9') return DIGIT;

    if (ch=='b' || ch == 'c' || ch == 'd') return ALPHA1;
    if (ch >= 'a' && ch <= 'z' && (ch!='b' && ch != 'c' && ch != 'd')) return ALPHA2;
    if (ch >= 'a' && ch <= 'z') return ALPHA;

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
        case '[': return L_SQR;
        case ']': return R_SQR;
        case '_': return UNDERSCORE;
        case '&': return AMPERSAND;
        case '~': return NOT;
        default: return INPUT_COUNT;
    }
}

// Create lexer state and preload first buffer.
twinBuffer* initializeLexer(FILE *fp) {
    twinBuffer *tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    if (!tb) return NULL;
    tb->fp = fp;
    tb->forward = 0;
    tb->lexemeBegin = 0;
    tb->currentBuffer = 1;
    tb->lineNo = 1;

    memset(tb->buffer1, 0, BUFFER_SIZE);
    memset(tb->buffer2, 0, BUFFER_SIZE);

    size_t n = fread(tb->buffer1, 1, BUFFER_SIZE - 1, tb->fp);
    tb->buffer1[n] = '\0';
    return tb;
}

// Refill inactive twin buffer and switch.
void refillBuffer(twinBuffer *tb) {
    if (tb->currentBuffer == 1) {
        memset(tb->buffer2, 0, BUFFER_SIZE);
        size_t n = fread(tb->buffer2, 1, BUFFER_SIZE - 1, tb->fp);
        tb->buffer2[n] = '\0';
        tb->currentBuffer = 2;
    } else {
        memset(tb->buffer1, 0, BUFFER_SIZE);
        size_t n = fread(tb->buffer1, 1, BUFFER_SIZE - 1, tb->fp);
        tb->buffer1[n] = '\0';
        tb->currentBuffer = 1;
    }
}

// Fetch next char from active buffer.
int getNextChar(twinBuffer *tb) {
    char *buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
    int ch = (unsigned char)buf[tb->forward];

    if (ch == '\0') {
        if (feof(tb->fp)) return EOF;

        refillBuffer(tb);
        tb->forward = 0;
        buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
        ch = (unsigned char)buf[tb->forward];
        if (ch == '\0') return EOF;
    }

    tb->forward++;
    return ch;
}

// Move one char back in twin-buffer stream.
void retract(twinBuffer *tb) {
    if (tb->forward == 0) {
        tb->currentBuffer = (tb->currentBuffer == 1) ? 2 : 1;
        tb->forward = BUFFER_SIZE - 1;

        char *buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
        while (tb->forward > 0 && buf[tb->forward] == '\0')
            tb->forward--;
    } else {
        tb->forward--;
    }
}

// DFA driver: returns one token each call.
tokenInfo getNextToken(twinBuffer *tb) {
    int currentState = 1; // DFA current state
    int nextState;        // DFA next state
    int ch;               // current input character
    tokenInfo tk;         // token being built
    int lp = 0;           // lexeme length

    memset(tk.lexeme, 0, MAX_LEXEME_LEN);
    tk.lineNo = tb->lineNo;

    while (1) {
        ch = getNextChar(tb);

        if (ch == EOF) {
            if (lp > 0) {
                break;
            }
            tk.token = TK_EOF;
            strcpy(tk.lexeme, "EOF");
            tk.lineNo = tb->lineNo;
            return tk;
        }

        if (ch == '%') {
            // keep comment token but skip full comment text
            tk.token = TK_COMMENT;
            strcpy(tk.lexeme, "%");
            tk.lineNo = tb->lineNo;

            while ((ch = getNextChar(tb)) != EOF && ch != '\n');
            if (ch == '\n') tb->lineNo++;
            return tk;
        }

        InputType input = mapCharToEnum(ch);

        if ((currentState!=1 && currentState!=12) && (input==ALPHA1 || input==ALPHA2) ){
            input = ALPHA;
        }

        if (!(currentState==11 || currentState==12 || currentState==13) && (input==DIGIT1) ){
            input = DIGIT;
        }

        if ((currentState==45 || currentState==46) && (input==ALPHA)){
            input = ALPHABET;
        }

        if (currentState==55 && ch=='E'){
            input = E;
        }

        if (input == INPUT_COUNT) {
            // unknown char: either finish current token or emit error

            if (lp > 0) {
                retract(tb);
                StateInfo info = acceptStateMap[currentState];
                if (info.isFinal) {
                    tokenInfo result = info.handler(tk.lexeme);
                    result.lineNo = tb->lineNo;
                    return result;
                }
                tk.token = TK_ERROR;
                tk.lexeme[lp] = '\0';
                tk.lineNo = tb->lineNo;
                return tk;
            }

            tk.token = TK_ERROR;
            tk.lexeme[0] = ch;
            tk.lexeme[1] = '\0';
            tk.lineNo = tb->lineNo;
            return tk;
        }

        nextState = transitionMatrix[currentState][input];

        if (currentState == 53 && (nextState == 65 || nextState == 66)) {
            // split 23.abc as NUM + DOT + FIELDID
            retract(tb);
            retract(tb);
            if (lp > 0) { lp--; tk.lexeme[lp] = '\0'; }
            tokenInfo result = handle_TK_NUM(tk.lexeme);
            result.lineNo = tb->lineNo;
            return result;
        }

        StateInfo nextInfo = acceptStateMap[nextState];

        if (nextInfo.isFinal) {
            // final state reached, return token now
            if (nextInfo.retract) {
                retract(tb);
            } else {
                if (lp < MAX_LEXEME_LEN - 1) {
                    tk.lexeme[lp++] = ch;
                    tk.lexeme[lp] = '\0';
                }
            }

            tk.lineNo = tb->lineNo;
            tokenInfo result = nextInfo.handler(tk.lexeme);
            result.lineNo = tb->lineNo;
            return result;
        }
        if (ch == '\n') tb->lineNo++;

        if (currentState == 1 && (input == DELIM || input == NEWLINE)) {
            // ignore leading spaces/newlines

            tb->lexemeBegin = tb->forward;
            continue;
        }

        if (nextInfo.retract) {
            retract(tb);

        } else {
            if (lp < MAX_LEXEME_LEN - 1) {
                tk.lexeme[lp++] = ch;
                tk.lexeme[lp] = '\0';
            }
        }

        currentState = nextState;
    }

    if (lp > 0) {
        StateInfo info = acceptStateMap[currentState];
        if (info.isFinal) {
            tokenInfo result = info.handler(tk.lexeme);
            result.lineNo = tb->lineNo;
            return result;
        } else {
            tk.token = TK_ERROR;
            tk.lexeme[lp] = '\0';
            tk.lineNo = tb->lineNo;
            return tk;
        }
    }

    tk.token = TK_EOF;
    strcpy(tk.lexeme, "EOF");
    tk.lineNo = tb->lineNo;
    return tk;
}
// Register all final DFA states with handlers.
void initializeAcceptStateMap() {
    // default: non-final states
    for (int i = 0; i < MAX_STATES; ++i) {
        acceptStateMap[i].handler = NULL;
        acceptStateMap[i].isFinal = false;
        acceptStateMap[i].retract = false;
    }

    acceptStateMap[5]  = (StateInfo){ .handler = handle_TK_ASSIGNOP, .isFinal = true };
    acceptStateMap[7]  = (StateInfo){ .handler = handle_TK_LE, .isFinal = true };
    acceptStateMap[8]  = (StateInfo){ .handler = handle_TK_LT, .isFinal = true , .retract = true };
    acceptStateMap[15] = (StateInfo){ .handler = handle_TK_PLUS,     .isFinal = true };
    acceptStateMap[16] = (StateInfo){ .handler = handle_TK_MINUS,    .isFinal = true };
    acceptStateMap[17] = (StateInfo){ .handler = handle_TK_MUL,      .isFinal = true };
    acceptStateMap[18] = (StateInfo){ .handler = handle_TK_DIV,      .isFinal = true };
    acceptStateMap[37] = (StateInfo){ .handler = handle_TK_AND,      .isFinal = true };

    acceptStateMap[10] = (StateInfo){ .handler = handle_TK_FIELDID,   .isFinal = true, .retract = true };
    acceptStateMap[14] = (StateInfo){ .handler = handle_TK_ID,      .isFinal = true, .retract = true };
    acceptStateMap[61] = (StateInfo){ .handler = handle_TK_NUM,     .isFinal = true , .retract = true };
    acceptStateMap[62] = (StateInfo){ .handler = handle_TK_RNUM,    .isFinal = true };

    acceptStateMap[22] = (StateInfo){ .handler = handle_TK_SEM,     .isFinal = true };
    acceptStateMap[21] = (StateInfo){ .handler = handle_TK_COMMA,   .isFinal = true };
    acceptStateMap[20] = (StateInfo){ .handler = handle_TK_SQL,     .isFinal = true };
    acceptStateMap[19] = (StateInfo){ .handler = handle_TK_SQR,     .isFinal = true };
    acceptStateMap[26] = (StateInfo){ .handler = handle_TK_OP,      .isFinal = true };
    acceptStateMap[25] = (StateInfo){ .handler = handle_TK_CL,      .isFinal = true };
    acceptStateMap[23] = (StateInfo){ .handler = handle_TK_COLON,   .isFinal = true };
    acceptStateMap[24] = (StateInfo){ .handler = handle_TK_DOT,     .isFinal = true };
    acceptStateMap[27] = (StateInfo){ .handler = handle_NOT,     .isFinal = true };
    acceptStateMap[29] = (StateInfo){ .handler = handle_TK_EQ,     .isFinal = true };
    acceptStateMap[31] = (StateInfo){ .handler = handle_TK_NE,     .isFinal = true };
    acceptStateMap[33] = (StateInfo){ .handler = handle_TK_GE,     .isFinal = true };
    acceptStateMap[34] = (StateInfo){ .handler = handle_TK_GT,     .isFinal = true };
    acceptStateMap[40] = (StateInfo){ .handler = handle_TK_OR,     .isFinal = true };
    acceptStateMap[51] = (StateInfo){ .handler = handle_TK_RUID,     .isFinal = true, .retract = true };
    acceptStateMap[59] = (StateInfo){ .handler = handle_TK_RNUM,     .isFinal = true, .retract = true };
    acceptStateMap[65] = (StateInfo){ .handler = handle_TK_ERROR,     .isFinal = true, .retract = true };

    acceptStateMap[66] = (StateInfo){ .handler = handle_TK_ERROR, .isFinal = true, .retract = true }; // "<-" case

    acceptStateMap[48] = (StateInfo){ .handler = handle_TK_FUNID,     .isFinal = true, .retract = true };

}

// Copy file while skipping '%' comments.
void removeComments(char *inputFile, char *outputFile) {
    // simple pre-pass: drop '%' to end-of-line
    FILE *src = fopen(inputFile, "r");
    FILE *dest = fopen(outputFile, "w");
    if (!src || !dest) {
        if (src) fclose(src);
        if (dest) fclose(dest);
        return;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        if (ch == '%') {
            while ((ch = fgetc(src)) != EOF && ch != '\n');
            if (ch == '\n') fputc('\n', dest);
        } else {
            fputc(ch, dest);
        }
    }
    fclose(src);
    fclose(dest);
}

// Print token / lexical error in required format.
void printToken(tokenInfo tk)
{
    static const char *tokenNames[] = {
        "TK_COMMENT",
        "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION",
        "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN",
        "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_INPUT", "TK_OUTPUT",
        "TK_INT", "TK_REAL", "TK_ENDWHILE", "TK_IF", "TK_THEN",
        "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_CALL",
        "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_ASSIGNOP", "TK_PLUS",
        "TK_MINUS", "TK_MUL", "TK_DIV", "TK_AND", "TK_OR", "TK_NOT",
        "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE", "TOK",
        "TK_SQL", "TK_SQR", "TK_OP", "TK_CL", "TK_COMMA", "TK_SEM",
        "TK_COLON", "TK_DOT", "TK_ID", "TK_FUNID", "TK_FIELDID",
        "TK_RUID", "TK_NUM", "TK_RNUM", "TK_EOF", "TK_ERROR"
    };

    const char *name = "UNKNOWN";

    if (tk.token >= 0 && tk.token < (int)(sizeof(tokenNames)/sizeof(tokenNames[0])))
        name = tokenNames[tk.token];

    if (tk.token == TK_LENGTH_ERROR)
    {
        printf("Line %d Error: Variable Identifier is longer than the prescribed length of 20 characters.\n",
               tk.lineNo);
        return;
    }

    if (tk.token == TK_ERROR)
    {
        if (strlen(tk.lexeme)==1){
            printf("Line %d Error: Unknown Symbol <%s>\n",
                        tk.lineNo, tk.lexeme);
        }else{
            printf("Line %d Error: Unknown pattern <%s>\n",
               tk.lineNo, tk.lexeme);
        }

        return;
    }

    printf("Line no. %d\t Lexeme %s\t Token %s\n",
           tk.lineNo, tk.lexeme, name);
}
// Return keyword token if reserved, else TK_ID.
TokenName checkKeyword(char *lexeme) {
    if (strcmp(lexeme, "with") == 0) return TK_WITH;
    if (strcmp(lexeme, "parameters") == 0) return TK_PARAMETERS;
    if (strcmp(lexeme, "end") == 0) return TK_END;
    if (strcmp(lexeme, "while") == 0) return TK_WHILE;
    if (strcmp(lexeme, "union") == 0) return TK_UNION;
    if (strcmp(lexeme, "endunion") == 0) return TK_ENDUNION;
    if (strcmp(lexeme, "definetype") == 0) return TK_DEFINETYPE;
    if (strcmp(lexeme, "as") == 0) return TK_AS;
    if (strcmp(lexeme, "type") == 0) return TK_TYPE;
    if (strcmp(lexeme, "global") == 0) return TK_GLOBAL;
    if (strcmp(lexeme, "parameter") == 0) return TK_PARAMETER;
    if (strcmp(lexeme, "list") == 0) return TK_LIST;
    if (strcmp(lexeme, "input") == 0) return TK_INPUT;
    if (strcmp(lexeme, "output") == 0) return TK_OUTPUT;
    if (strcmp(lexeme, "int") == 0) return TK_INT;
    if (strcmp(lexeme, "real") == 0) return TK_REAL;
    if (strcmp(lexeme, "endwhile") == 0) return TK_ENDWHILE;
    if (strcmp(lexeme, "if") == 0) return TK_IF;
    if (strcmp(lexeme, "then") == 0) return TK_THEN;
    if (strcmp(lexeme, "endif") == 0) return TK_ENDIF;
    if (strcmp(lexeme, "read") == 0) return TK_READ;
    if (strcmp(lexeme, "write") == 0) return TK_WRITE;
    if (strcmp(lexeme, "return") == 0) return TK_RETURN;
    if (strcmp(lexeme, "call") == 0) return TK_CALL;
    if (strcmp(lexeme, "record") == 0) return TK_RECORD;
    if (strcmp(lexeme, "endrecord") == 0) return TK_ENDRECORD;
    if (strcmp(lexeme, "else") == 0) return TK_ELSE;
    return TK_ID;
}
