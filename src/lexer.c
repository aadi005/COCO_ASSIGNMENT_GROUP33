#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexerDef.h"
#include "stateHandlers.h"
#include <ctype.h>   
#include <stdbool.h>  

/* Definition of the global accept state map.  Every entry is initialised
   to a dummy handler with isFinal=false and retract=false so that the
   lexer can index it without having to check for NULL pointers. */


StateInfo acceptStateMap[MAX_STATES] = { {NULL, false, false} };
int transitionMatrix[MAX_STATES][INPUT_COUNT] = {
    // State 0
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 1
    {52, 65, 65, 11, 9, 65, 41, 43, 44, 2, 32, 28, 16, 24, 22, 21, 38, 49, 63, 15, 18, 17, 30, 35, 65, 25, 23, 26, 45, 20, 19, 27},
    // State 2
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    // State 3
    {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
    // State 4
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 5, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 5
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 6
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 7
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 8
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 9
    {10, 10, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    // State 10
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 11
    {10, 12, 9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10},
    // State 12
    {14, 13, 14, 12, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14},
    // State 13
    {14, 13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14},
    // State 14
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 15
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 16
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 17
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 18
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 19
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 20
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 21
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 22
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 23
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 24
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 25
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 26
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 27
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 28
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 29, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 29
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 30
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 31, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 31
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 32
    {34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34},
    // State 33
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 34
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 35
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 36, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 36
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 37, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 37
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 38
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 39, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 39
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 40, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 40
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 41
    {42, 42, 42, 42, 42, 42, 41, 41, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42},
    // State 42
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 43
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 44
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 45
    {65, 65, 65, 65, 65, 46, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 46
    {47, 48, 48, 48, 48, 46, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
    // State 47
    {47, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48},
    // State 48
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 49
    {65, 65, 50, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 50
    {51, 51, 50, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51, 51},
    // State 51
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 52
    {52, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 53, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
    // State 53
    {54, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 54
    {55, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 55
    {59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 59, 56, 59, 59, 59, 59, 59, 59, 59},
    // State 56
    {60, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 57, 65, 65, 65, 65, 65, 65, 57, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 57
    {60, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 58 (Skipped/Empty)
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 59
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 60
    {62, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 61
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 62
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 63
    {63, 63, 63, 63, 63, 63, 63, 64, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
    // State 64
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
    // State 65
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    // States 66
    {65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
};

/* Helper to map characters to the transition matrix columns based on CSV header */
InputType mapCharToEnum(int ch) {
    // Treat both the EOF constant and any embedded nulls as end-of-file input
    if (ch == EOF || ch == '\0') return EOF_TYPE;
    if (ch == ' ' || ch == '\t' || ch == '\r') return DELIM;
    if (ch == '\n') return NEWLINE;
    
    // Priority: digit1 (2-7) must be checked before general digit (0-9)
    if (ch >= '2' && ch <= '7') return DIGIT1;
    if (ch >= '0' && ch <= '9') return DIGIT;
    
    // Character classes for identifiers as defined in regex
    
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

twinBuffer* initializeLexer(FILE *fp) {
    twinBuffer *tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    if (!tb) return NULL;
    tb->fp = fp;
    tb->forward = 0;
    tb->lexemeBegin = 0;
    tb->currentBuffer = 1;

    memset(tb->buffer1, 0, BUFFER_SIZE);
    memset(tb->buffer2, 0, BUFFER_SIZE);

    size_t n = fread(tb->buffer1, 1, BUFFER_SIZE - 1, tb->fp);
    tb->buffer1[n] = '\0'; // sentinel
    return tb;
}

void refillBuffer(twinBuffer *tb) {
    if (tb->currentBuffer == 1) {
        // switching TO buffer2, so fill buffer2
        memset(tb->buffer2, 0, BUFFER_SIZE);
        size_t n = fread(tb->buffer2, 1, BUFFER_SIZE - 1, tb->fp);
        tb->buffer2[n] = '\0'; // sentinel
        tb->currentBuffer = 2;
    } else {
        memset(tb->buffer1, 0, BUFFER_SIZE);
        size_t n = fread(tb->buffer1, 1, BUFFER_SIZE - 1, tb->fp);
        tb->buffer1[n] = '\0'; // sentinel
        tb->currentBuffer = 1;
    }
}

int getNextChar(twinBuffer *tb) {
    char *buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
    int ch = (unsigned char)buf[tb->forward];

    if (ch == '\0') {
        // hit sentinel — are we truly at EOF?
        if (feof(tb->fp)) return EOF;
        // otherwise refill and read first char of new buffer
        refillBuffer(tb);
        tb->forward = 0;
        buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
        ch = (unsigned char)buf[tb->forward];
        if (ch == '\0') return EOF; // new buffer also empty
    }

    tb->forward++;
    return ch;
}

void retract(twinBuffer *tb) {
    if (tb->forward == 0) {
        // switch back to the other buffer, last valid char
        tb->currentBuffer = (tb->currentBuffer == 1) ? 2 : 1;
        tb->forward = BUFFER_SIZE - 1;
        // walk back past the sentinel to the last real character
        char *buf = (tb->currentBuffer == 1) ? tb->buffer1 : tb->buffer2;
        while (tb->forward > 0 && buf[tb->forward] == '\0')
            tb->forward--;
    } else {
        tb->forward--;
    }
}


tokenInfo getNextToken(twinBuffer *tb) {
    int currentState = 1;
    int nextState;
    int ch;                    // now holds EOF sentinel as an int
    tokenInfo tk;
    int lp = 0;
    static int lineNo = 1;

    // Reset lexeme and token structure
    memset(tk.lexeme, 0, MAX_LEXEME_LEN);
    tk.lineNo = lineNo;

    while (1) {
        ch = getNextChar(tb);
        /* --- EOF HANDLING ---
           Instead of returning immediately when EOF is encountered we defer the
           decision.  This allows a token that ended exactly at end-of-file to be
           flushed out before the EOF token itself is generated.  The variable
           "lp" tracks how many characters have been accumulated in the current
           lexeme.  If we are mid‑token we break the loop and handle the lexeme
           after the loop; otherwise we can return the EOF token right away. */
        if (ch == EOF) {
            if (lp > 0) {
                /* there is a partial lexeme waiting – break so it can be
                   finalised below */
                break;
            }
            tk.token = TK_EOF;
            strcpy(tk.lexeme, "EOF");
            tk.lineNo = lineNo;
            return tk;
        }

        
        

        /* --- UNDERSCORE / FUNID HANDLING ---
           Function identifiers and other names start with an underscore.  Instead
           of letting the DFA drop them into a sink state, handle them explicitly.
           Collect characters until a non-alphanumeric/underscore is seen. */

        

        /* --- COMMENT HANDLING ---
           A percent sign begins a comment; the entire rest of the line is ignored
           but a TK_COMMENT token with lexeme "%" is returned so that the parser
           (or test harness) can log its presence.  Subsequent characters up to and
           including the newline are consumed here. */
        if (ch == '%') {
            tk.token = TK_COMMENT;
            strcpy(tk.lexeme, "%");
            tk.lineNo = lineNo;
            /* eat until end of line or file */
            while ((ch = getNextChar(tb)) != EOF && ch != '\n');
            if (ch == '\n') lineNo++;
            return tk;
        }

        InputType input = mapCharToEnum(ch);
        /* guard against unmapped characters returning INPUT_COUNT which would
           index past the end of the transition table.  Unrecognised symbols
           are considered lexical errors and produce a one‑character error
           token. */

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
            tk.token = TK_ERROR;
            tk.lexeme[0] = ch;
            tk.lexeme[1] = '\0';
            tk.lineNo = lineNo;
            return tk;
        }

        nextState = transitionMatrix[currentState][input];

        /* --- SINK STATE / ERROR HANDLING --- */
        // If the DFA hits state 65, it's a lexical error. 
        // We stop here to prevent an infinite loop
        /* --- ERRATA HANDLING --- */
        /* --- ACCEPT / RETRACTION LOGIC --- */
        /* We always look up the state info structure, even for non-final
           states.  The "retract" flag is used by both the accept check and
           the intermediate‑state requirement described later in the code. */
        StateInfo nextInfo = acceptStateMap[nextState];

        /* Update line count on newlines before we consider appending the
           character; this mirrors the behaviour of the previous implementation. */

        if (nextInfo.isFinal) {
            if (nextInfo.retract) {
                /* consume nothing when the accept state is reached via an
                   "other" transition. */
                
                retract(tb);
            } else {
                if (lp < MAX_LEXEME_LEN - 1) {
                    tk.lexeme[lp++] = ch;
                    tk.lexeme[lp] = '\0';
                }
            }

            /* invoke handler and return token */
            tk.lineNo = lineNo;
            tokenInfo result = nextInfo.handler(tk.lexeme);
            result.lineNo = lineNo;
            return result;
        }
        if (ch == '\n') lineNo++;
        /* --- DELIMITER / NEWLINE HANDLING ---
           When in state 1, skip all leading whitespace (delimiters & newlines).
           When NOT in state 1 and a delimiter appears, treat it as end-of-token
           marker *only if* we're in an accept state or the DFA would reject it
           anyway. */
        if (currentState == 1 && (input == DELIM || input == NEWLINE)) {
            // Skip leading whitespace in state 1
            tb->lexemeBegin = tb->forward;
            continue;
        }

        /* --- STATE TRANSITION LOGIC --- */

        /* handle retraction for intermediate states; the same flag that tells
           us to retract on accept is also used here so that we don't have a
           separate hard‑coded list. */
        if (nextInfo.retract) {
            retract(tb);
            /* lexeme already contains the previous characters only, so no
               additional adjustment is necessary */
        } else {
            if (lp < MAX_LEXEME_LEN - 1) {
                tk.lexeme[lp++] = ch;
                tk.lexeme[lp] = '\0';
            }
        }

        currentState = nextState;
    }

    /* --- EOF CLEANUP ---
       we exited the loop because ch was EOF while lp>0.  The lexeme buffer
       contains the characters that have been read so far; finalise a token
       based on the current DFA state.  If the state is accepting we simply
       invoke its handler, otherwise report a lexical error. */
    if (lp > 0) {
        StateInfo info = acceptStateMap[currentState];
        if (info.isFinal) {
            tokenInfo result = info.handler(tk.lexeme);
            result.lineNo = lineNo;
            return result;
        } else {
            tk.token = TK_ERROR;
            tk.lexeme[lp] = '\0';
            tk.lineNo = lineNo;
            return tk;
        }
    }

    /* no lexeme pending – fall through and return EOF token (should not
       ordinarily happen because it would have been returned earlier) */
    tk.token = TK_EOF;
    strcpy(tk.lexeme, "EOF");
    tk.lineNo = lineNo;
    return tk;
}
void initializeAcceptStateMap() {
    /* set every slot to a harmless default; the noop handler is defined in
       stateHandlers.c and simply returns an empty token.  All boolean flags
       default to false so that a plain lookup can be done without checking
       for NULL. */
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
    acceptStateMap[48] = (StateInfo){ .handler = handle_TK_ID,      .isFinal = true, .retract = true };

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
    acceptStateMap[61] = (StateInfo){ .handler = handle_TK_NUM,     .isFinal = true, .retract = true };
    acceptStateMap[65] = (StateInfo){ .handler = handle_TK_ERROR,     .isFinal = true, .retract = true };
    
    
    
    
    


    /* --- Special Accept States --- */
    //acceptStateMap[42] = (StateInfo){ .handler = handle_TK_CUSTOM,     .isFinal = true , .retract = true };
    //acceptStateMap[43] = (StateInfo){ .handler = handle_TK_NEW,     .isFinal = true };

    acceptStateMap[48] = (StateInfo){ .handler = handle_TK_FUNID,     .isFinal = true, .retract = true };

    
}

void removeComments(char *inputFile, char *outputFile) {
    // Basic implementation: read from inputFile, skip text between % and \n, write to outputFile
    FILE *src = fopen(inputFile, "r");
    FILE *dest = fopen(outputFile, "w");
    if (!src || !dest) return;

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

    /* ========================= */
    /* Handle identifier length  */
    /* ========================= */
    if (tk.token == TK_LENGTH_ERROR)
    {
        printf("Line no. %d: Error: Variable Identifier is longer than the prescribed length of 20 characters.\n",
               tk.lineNo);
        return;
    }

    /* ========================= */
    /* Handle lexical errors     */
    /* ========================= */
    if (tk.token == TK_ERROR)
    {
        if (strlen(tk.lexeme)==1){
            printf("Line no %d: Error: Unknown Symbol <%s>\n",
                        tk.lineNo, tk.lexeme);
        }else{
            printf("Line no %d: Error: Unknown Pattern <%s>\n",
               tk.lineNo, tk.lexeme);
        }
        
        return;
    }

    /* ========================= */
    /* Normal token printing     */
    /* ========================= */
    printf("Line no. %d\t Lexeme %s\t Token %s\n",
           tk.lineNo, tk.lexeme, name);
}
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
    return TK_ID; // Default if not a keyword
}
