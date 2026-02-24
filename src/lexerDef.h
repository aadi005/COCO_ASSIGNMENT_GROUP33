#ifndef LEXERDEF_H
#define LEXERDEF_H

#define MAX_LEXEME_LEN 30
#define MAX_STATES 64
#define BUFFER_SIZE 50

typedef enum {
    // Keywords
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

    // Operators
    TK_ASSIGNOP,     // <---
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_AND,          // &&&
    TK_OR,           // @@@
    TK_NOT,          // ~
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE,

    // Delimiters
    TK_SQL,          // [
    TK_SQR,          // ]
    TK_OP,           // (
    TK_CL,           // )
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,

    // Identifiers
    TK_ID,
    TK_FUNID,
    TK_FIELDID,
    TK_RUID,

    // Numbers
    TK_NUM,
    TK_RNUM,

    // Special
    TK_EOF,
    TK_ERROR

} TokenName;

typedef enum {
    DIGIT,          // 0-9
    DIGIT1,         // 2-7 
    ALPHA,          // a-b 
    ALPHA1,         // b-d
    wALPHA2,         // e-z 
    ALPHABET,       // a-z, A-Z 
    DELIM,          // Space/Blank 
    NEWLINE,        // \n 
    EOF_TYPE,       // End of file 
    LESS_THAN,      // <
    GREATER_THAN,   // > 
    ZERO,           // 0
    MINUS,          // -
    DOT,            // . 
    SEMICOLON,      // ; 
    COMMA,          // , 
    AT_SYMBOL,      // @ 
    HASH,           // # 
    PERCENT,        // % 
    EQUAL,          // =
    FORWARD_SLASH,  // / 
    ASTERISK,       // * 
    EXCLAMATION,    // ! 
    AMPERSAND,      // &
    EPSILON,        // epsilon 
    CLOSE_PAREN,    // )//
    COLON,          // : 
    OPEN_PAREN,     // (
    UNDERSCORE,     // _ 
    INPUT_COUNT     // Helper to track size
} InputType;

typedef struct {
    TokenName token;
    char lexeme[MAX_LEXEME_LEN];
    int lineNo;
    union {
        int intValue;
        float realValue;
    } value;

} tokenInfo;

typedef struct {
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int forward;
    int lexemeBegin;
    int currentBuffer;   // 1 or 2
    FILE *fp;
} twinBuffer;

extern int transitionMatrix[MAX_STATES][INPUT_COUNT];

#endif LEXERDEF_H