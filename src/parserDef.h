/*
 * Group 33
 * Aaditya Goel 2022B3A70417P
 * Divyansh Jha 2022B3A70438P
 * Prakhar Mittal 2022B3A70426P
 * Mukund Srivastava 2022B3A70562P
 * Aryan Gupta 2022B3A70495P
 * Garv Gulati 2022B4A70685P
 */



#ifndef PARSERDEF_H
#define PARSERDEF_H

#include "lexerDef.h"
#include <stdbool.h>
#include <stdio.h>

// Non-terminals of the grammar.
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

// Grammar symbol type.
typedef enum {
    SYM_TERMINAL,
    SYM_NON_TERMINAL,
    SYM_EPSILON
} SymbolType;

typedef struct {
    // Symbol category.
    SymbolType type;
    union {
        // Valid when type == SYM_TERMINAL.
        TokenName    terminal;
        // Valid when type == SYM_NON_TERMINAL.
        NonTerminal  nonTerminal;
    };
} GrammarSymbol;

// Max RHS length per production.
#define MAX_RHS          10
// Total number of grammar productions.
#define NUM_PRODUCTIONS  95

typedef struct {
    // Production LHS.
    NonTerminal   lhs;
    // Production RHS sequence.
    GrammarSymbol rhs[MAX_RHS];
    // Number of valid RHS symbols.
    int           rhsLen;
    // Stable rule index.
    int           ruleNum;
} Production;

// Number of terminal tokens used by parser.
#define NUM_TOKENS  59
// Index used for epsilon in FIRST rows.
#define EPS_IDX     59
// FIRST row width including epsilon slot.
#define FIRST_SIZE  60
// FOLLOW row width (terminals + EOF).
#define FOLLOW_SIZE 59

typedef struct {
    // FIRST sets by non-terminal.
    bool first [NT_COUNT][FIRST_SIZE];
    // FOLLOW sets by non-terminal.
    bool follow[NT_COUNT][FOLLOW_SIZE];
} FirstAndFollow;

// LL(1) parse table stores rule indices.
typedef int ParseTableType[NT_COUNT][NUM_TOKENS];

typedef struct ParseTreeNode {
    // Grammar symbol at this node.
    GrammarSymbol        symbol;
    // Token data for leaf terminals.
    tokenInfo            tkInfo;
    // Parent pointer.
    struct ParseTreeNode *parent;
    // First child in sibling chain.
    struct ParseTreeNode *firstChild;
    // Next sibling pointer.
    struct ParseTreeNode *nextSibling;
    // True for terminal/epsilon leaves.
    bool                  isLeaf;
} ParseTreeNode;

typedef struct StackElem {
    // Symbol stored on parse stack.
    GrammarSymbol   sym;
    // Parse-tree node mapped to symbol.
    ParseTreeNode  *treeNode;
    // Next stack element.
    struct StackElem *next;
} StackElem;

typedef struct {
    // Top stack pointer.
    StackElem *top;
    // Current stack size.
    int        size;
} ParseStack;

// Global grammar rules.
extern Production     grammar[NUM_PRODUCTIONS];
// Global FIRST/FOLLOW cache.
extern FirstAndFollow FF;
// Global LL(1) parse table.
extern ParseTableType parseTable;
// Non-terminal names for debug/print.
extern const char    *ntNames[NT_COUNT];
// Token names for debug/print.
extern const char    *tokenNameStr[NUM_TOKENS];

#endif
