/*
 * parserDef.h
 * CS F363 - Compiler Construction, BITS Pilani, Jan 2026
 * Contains all data structure definitions for the Parser.
 */

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>

/* ==========================================================
   NON-TERMINAL ENUMERATION
   53 non-terminals as per the modified LL(1) grammar.
   ========================================================== */
typedef enum {
    NT_PROGRAM = 0,
    NT_MAINFUNCTION,          /*  1 */
    NT_OTHERFUNCTIONS,        /*  2 */
    NT_FUNCTION,              /*  3 */
    NT_INPUT_PAR,             /*  4 */
    NT_OUTPUT_PAR,            /*  5 */
    NT_PARAMETER_LIST,        /*  6 */
    NT_DATA_TYPE,             /*  7 */
    NT_PRIMITIVE_DATATYPE,    /*  8 */
    NT_CONSTRUCTED_DATATYPE,  /*  9 */
    NT_REMAINING_LIST,        /* 10 */
    NT_STMTS,                 /* 11 */
    NT_TYPE_DEFINITIONS,      /* 12 */
    NT_ACTUAL_OR_REDEFINED,   /* 13 */
    NT_TYPE_DEFINITION,       /* 14 */
    NT_FIELD_DEFINITIONS,     /* 15 */
    NT_FIELD_DEFINITION,      /* 16 */
    NT_FIELD_TYPE,            /* 17 */
    NT_MORE_FIELDS,           /* 18 */
    NT_DECLARATIONS,          /* 19 */
    NT_DECLARATION,           /* 20 */
    NT_GLOBAL_OR_NOT,         /* 21 */
    NT_OTHER_STMTS,           /* 22 */
    NT_STMT,                  /* 23 */
    NT_ASSIGNMENT_STMT,       /* 24 */
    NT_SINGLE_OR_REC_ID,      /* 25 */
    NT_OPTION_SINGLE_CONSTR,  /* 26 */
    NT_ONE_EXPANSION,         /* 27 */
    NT_MORE_EXPANSIONS,       /* 28 */
    NT_FUN_CALL_STMT,         /* 29 */
    NT_OUTPUT_PARAMETERS,     /* 30 */
    NT_INPUT_PARAMETERS,      /* 31 */
    NT_ITERATIVE_STMT,        /* 32 */
    NT_CONDITIONAL_STMT,      /* 33 */
    NT_ELSE_PART,             /* 34 */
    NT_IO_STMT,               /* 35 */
    NT_ARITH_EXPR,            /* 36 */
    NT_EXP_PRIME,             /* 37 */
    NT_TERM,                  /* 38 */
    NT_TERM_PRIME,            /* 39 */
    NT_FACTOR,                /* 40 */
    NT_HIGH_PREC_OP,          /* 41 */
    NT_LOW_PREC_OP,           /* 42 */
    NT_BOOL_EXPR,             /* 43 */
    NT_VAR,                   /* 44 */
    NT_LOGICAL_OP,            /* 45 */
    NT_RELATIONAL_OP,         /* 46 */
    NT_RETURN_STMT,           /* 47 */
    NT_OPTIONAL_RETURN,       /* 48 */
    NT_ID_LIST,               /* 49 */
    NT_MORE_IDS,              /* 50 */
    NT_DEFINETYPE_STMT,       /* 51 */
    NT_A,                     /* 52 */
    NT_COUNT                  /* sentinel - total count */
} NonTerminal;

/* ==========================================================
   GRAMMAR SYMBOL
   A grammar symbol is either a terminal (TokenName),
   a non-terminal (NonTerminal enum), or epsilon.
   ========================================================== */
typedef enum {
    SYM_TERMINAL,
    SYM_NON_TERMINAL,
    SYM_EPSILON
} SymbolType;

typedef struct {
    SymbolType type;
    union {
        TokenName    terminal;
        NonTerminal  nonTerminal;
    };
} GrammarSymbol;

/* ==========================================================
   PRODUCTION RULE
   Each production stores its LHS non-terminal, the RHS
   symbols, and the rule number for parse-table use.
   ========================================================== */
#define MAX_RHS          10
#define NUM_PRODUCTIONS  95

typedef struct {
    NonTerminal   lhs;
    GrammarSymbol rhs[MAX_RHS];
    int           rhsLen;   /* 0 = epsilon production */
    int           ruleNum;
} Production;

/* ==========================================================
   FIRST / FOLLOW SET SIZES
   TK_EOF = 58 (last valid parse token), so:
     NUM_TOKENS = 59   (indices 0..58)
     EPS_IDX    = 59   (epsilon flag slot in FIRST array)
     FIRST_SIZE = 60   (0..58 tokens + 59 for eps)
     FOLLOW_SIZE= 59   (0..58, TK_EOF = $ end marker)
   ========================================================== */
#define NUM_TOKENS  59
#define EPS_IDX     59
#define FIRST_SIZE  60
#define FOLLOW_SIZE 59

typedef struct {
    bool first [NT_COUNT][FIRST_SIZE];
    bool follow[NT_COUNT][FOLLOW_SIZE];
} FirstAndFollow;

/* ==========================================================
   PREDICTIVE PARSE TABLE
   parseTable[NT][token] = production rule index, or -1.
   ========================================================== */
typedef int ParseTableType[NT_COUNT][NUM_TOKENS];

/* ==========================================================
   PARSE TREE NODE
   n-ary tree stored as first-child / next-sibling.
   ========================================================== */
typedef struct ParseTreeNode {
    GrammarSymbol        symbol;
    tokenInfo            tkInfo;         /* valid for leaf nodes only */
    struct ParseTreeNode *parent;
    struct ParseTreeNode *firstChild;
    struct ParseTreeNode *nextSibling;
    bool                  isLeaf;
} ParseTreeNode;

/* ==========================================================
   PARSE STACK
   Each element carries the grammar symbol being processed
   and its corresponding parse-tree node.
   ========================================================== */
typedef struct StackElem {
    GrammarSymbol   sym;
    ParseTreeNode  *treeNode;
    struct StackElem *next;
} StackElem;
//SUGGESTION 2 : there is no need for size of stack, also where is the stack array thing?
typedef struct {
    StackElem *top;
    int        size;
} ParseStack;

/* ==========================================================
   GLOBAL GRAMMAR & TABLE (defined in parser.c)
   ========================================================== */
extern Production     grammar[NUM_PRODUCTIONS];
extern FirstAndFollow FF;
extern ParseTableType parseTable;
extern const char    *ntNames[NT_COUNT];
extern const char    *tokenNameStr[NUM_TOKENS];

#endif /* PARSERDEF_H */
