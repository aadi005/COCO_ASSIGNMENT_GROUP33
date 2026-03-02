

#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>


typedef enum {
    NT_PROGRAM = 0,
    NT_MAINFUNCTION,
    NT_OTHERFUNCTIONS,
    NT_FUNCTION,
    NT_INPUT_PAR,
    NT_OUTPUT_PAR,
    NT_PARAMETER_LIST,
    NT_DATA_TYPE,
    NT_PRIMITIVE_DATATYPE,
    NT_CONSTRUCTED_DATATYPE,
    NT_REMAINING_LIST,
    NT_STMTS,
    NT_TYPE_DEFINITIONS,
    NT_ACTUAL_OR_REDEFINED,
    NT_TYPE_DEFINITION,
    NT_FIELD_DEFINITIONS,
    NT_FIELD_DEFINITION,
    NT_FIELD_TYPE,
    NT_MORE_FIELDS,
    NT_DECLARATIONS,
    NT_DECLARATION,
    NT_GLOBAL_OR_NOT,
    NT_OTHER_STMTS,
    NT_STMT,
    NT_ASSIGNMENT_STMT,
    NT_SINGLE_OR_REC_ID,
    NT_OPTION_SINGLE_CONSTR,
    NT_ONE_EXPANSION,
    NT_MORE_EXPANSIONS,
    NT_FUN_CALL_STMT,
    NT_OUTPUT_PARAMETERS,
    NT_INPUT_PARAMETERS,
    NT_ITERATIVE_STMT,
    NT_CONDITIONAL_STMT,
    NT_ELSE_PART,
    NT_IO_STMT,
    NT_ARITH_EXPR,
    NT_EXP_PRIME,
    NT_TERM,
    NT_TERM_PRIME,
    NT_FACTOR,
    NT_HIGH_PREC_OP,
    NT_LOW_PREC_OP,
    NT_BOOL_EXPR,
    NT_VAR,
    NT_LOGICAL_OP,
    NT_RELATIONAL_OP,
    NT_RETURN_STMT,
    NT_OPTIONAL_RETURN,
    NT_ID_LIST,
    NT_MORE_IDS,
    NT_DEFINETYPE_STMT,
    NT_A,
    NT_COUNT
} NonTerminal;


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


#define MAX_RHS          10
#define NUM_PRODUCTIONS  95

typedef struct {
    NonTerminal   lhs;
    GrammarSymbol rhs[MAX_RHS];
    int           rhsLen;
    int           ruleNum;
} Production;


#define NUM_TOKENS  59
#define EPS_IDX     59
#define FIRST_SIZE  60
#define FOLLOW_SIZE 59

typedef struct {
    bool first [NT_COUNT][FIRST_SIZE];
    bool follow[NT_COUNT][FOLLOW_SIZE];
} FirstAndFollow;


typedef int ParseTableType[NT_COUNT][NUM_TOKENS];


typedef struct ParseTreeNode {
    GrammarSymbol        symbol;
    tokenInfo            tkInfo;
    struct ParseTreeNode *parent;
    struct ParseTreeNode *firstChild;
    struct ParseTreeNode *nextSibling;
    bool                  isLeaf;
} ParseTreeNode;


typedef struct StackElem {
    GrammarSymbol   sym;
    ParseTreeNode  *treeNode;
    struct StackElem *next;
} StackElem;

typedef struct {
    StackElem *top;
    int        size;
} ParseStack;


extern Production     grammar[NUM_PRODUCTIONS];
extern FirstAndFollow FF;
extern ParseTableType parseTable;
extern const char    *ntNames[NT_COUNT];
extern const char    *tokenNameStr[NUM_TOKENS];

#endif
