#ifndef LEXERDEF_H
#define LEXERDEF_H

#include <stdio.h>
#include <stdbool.h>

#define MAX_LEXEME_LEN 30
#define MAX_STATES 67
#define BUFFER_SIZE 50

typedef enum {

    TK_COMMENT,


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


    TK_SQL,
    TK_SQR,
    TK_OP,
    TK_CL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,


    TK_ID,
    TK_FUNID,
    TK_FIELDID,
    TK_RUID,


    TK_NUM,
    TK_RNUM,


    TK_EOF,
    TK_ERROR,
    TK_LENGTH_ERROR

} TokenName;

typedef enum {
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
    TokenName token;
    char lexeme[MAX_LEXEME_LEN];
    int lineNo;

    union {
        int intValue;
        float realValue;
    } val;

} tokenInfo;

typedef struct {
    char buffer1[BUFFER_SIZE];
    char buffer2[BUFFER_SIZE];
    int forward;
    int lexemeBegin;
    int currentBuffer;
    int lineNo;
    FILE *fp;
} twinBuffer;


typedef tokenInfo (*StateHandler)(char* lexeme);



typedef struct {
    StateHandler handler;
    bool isFinal;
    bool retract;
} StateInfo;


extern int transitionMatrix[MAX_STATES][INPUT_COUNT];
extern StateInfo acceptStateMap[MAX_STATES];

#endif
