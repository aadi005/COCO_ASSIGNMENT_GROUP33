#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdbool.h>

// Max token text length.
#define MAX_LEXEME_LEN 30
// Total DFA states.
#define MAX_STATES 67
// Twin-buffer chunk size.
#define BUFFER_SIZE 50

typedef enum {
    // Comment marker token (% line comment).
    TK_COMMENT,

    // Keywords.
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_ENDWHILE,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,

    // Operators.
    TK_ASSIGNOP,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,
    TOK,

    // Delimiters.
    TK_SQL,
    TK_SQR,
    TK_OP,
    TK_CL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,

    // Identifier kinds.
    TK_ID,
    TK_FUNID,
    TK_FIELDID,
    TK_RUID,

    // Numeric literals.
    TK_NUM,
    TK_RNUM,

    // Special/internal tokens.
    TK_EOF,
    TK_ERROR,
    TK_LENGTH_ERROR

} TokenName;

typedef enum {
    // Input classes used by DFA columns.
    DIGIT,
    DIGIT1,
    ALPHA,
    ALPHA1,
    ALPHA2,
    ALPHABET,
    DELIM,
    NEWLINE,
    EOF_TYPE,
    LESS_THAN,
    GREATER_THAN,
    EQUAL,
    MINUS,
    DOT,
    SEMICOLON,
    COMMA,
    AT_SYMBOL,
    HASH,
    PERCENT,
    PLUS,
    FORWARD_SLASH,
    ASTERISK,
    EXCLAMATION,
    AMPERSAND,
    E,
    CLOSE_PAREN,
    COLON,
    OPEN_PAREN,
    UNDERSCORE,
    L_SQR,
    R_SQR,
    NOT,
    INPUT_COUNT
} InputType;

typedef struct {
    // Token enum.
    TokenName token;
    // Raw token text.
    char lexeme[MAX_LEXEME_LEN];
    // 1-based source line.
    int lineNo;

    // Parsed literal value.
    union {
        int intValue;
        float realValue;
    } val;

} tokenInfo;

typedef struct {
    // First half of twin buffer.
    char buffer1[BUFFER_SIZE];
    // Second half of twin buffer.
    char buffer2[BUFFER_SIZE];
    // Current cursor index.
    int forward;
    // Start index of current lexeme.
    int lexemeBegin;
    // Active buffer id (1 or 2).
    int currentBuffer;
    // Current source line.
    int lineNo;
    // Source file handle.
    FILE *fp;
} twinBuffer;

// Callback for final DFA states.
typedef tokenInfo (*StateHandler)(char* lexeme);

typedef struct {
    // Token constructor for this state.
    StateHandler handler;
    // True if this is an accepting state.
    bool isFinal;
    // True if lexer should retract one char.
    bool retract;
} StateInfo;

// Global DFA transition matrix.
extern int transitionMatrix[MAX_STATES][INPUT_COUNT];
// Global accept-state metadata table.
extern StateInfo acceptStateMap[MAX_STATES];

#endif
