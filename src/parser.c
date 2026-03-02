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
#include <stdbool.h>

#include "parser.h"
#include "lexer.h"
#include "parserDef.h"

// Global production list.
Production     grammar[NUM_PRODUCTIONS];
// Global FIRST/FOLLOW cache.
FirstAndFollow FF;
// Global LL(1) parse table.
ParseTableType parseTable;

// Readable names for non-terminals.
const char *ntNames[NT_COUNT] = {
    "program",              "mainFunction",         "otherFunctions",
    "function",             "input_par",            "output_par",
    "parameter_list",       "dataType",             "primitiveDatatype",
    "constructedDatatype",  "remaining_list",       "stmts",
    "typeDefinitions",      "actualOrRedefined",    "typeDefinition",
    "fieldDefinitions",     "fieldDefinition",      "fieldType",
    "moreFields",           "declarations",         "declaration",
    "global_or_not",        "otherStmts",           "stmt",
    "assignmentStmt",       "singleOrRecId",        "optionSingleConstr",
    "oneExpansion",         "moreExpansions",       "funCallStmt",
    "outputParameters",     "inputParameters",      "iterativeStmt",
    "conditionalStmt",      "elsePart",             "ioStmt",
    "arithmeticExpression", "expPrime",             "term",
    "termPrime",            "factor",               "highPrecedenceOp",
    "lowPrecedenceOp",      "booleanExpression",    "var",
    "logicalOp",            "relationalOp",         "returnStmt",
    "optionalReturn",       "idList",               "moreIds",
    "definetypeStmt",       "A"
};

// Readable names for terminal tokens.
const char *tokenNameStr[NUM_TOKENS] = {
    "TK_COMMENT",   "TK_WITH",       "TK_PARAMETERS", "TK_END",
    "TK_WHILE",     "TK_UNION",      "TK_ENDUNION",   "TK_DEFINETYPE",
    "TK_AS",        "TK_TYPE",       "TK_MAIN",       "TK_GLOBAL",
    "TK_PARAMETER", "TK_LIST",       "TK_INPUT",      "TK_OUTPUT",
    "TK_INT",       "TK_REAL",       "TK_ENDWHILE",   "TK_IF",
    "TK_THEN",      "TK_ENDIF",      "TK_READ",       "TK_WRITE",
    "TK_RETURN",    "TK_CALL",       "TK_RECORD",     "TK_ENDRECORD",
    "TK_ELSE",      "TK_ASSIGNOP",   "TK_PLUS",       "TK_MINUS",
    "TK_MUL",       "TK_DIV",        "TK_AND",        "TK_OR",
    "TK_NOT",       "TK_LT",         "TK_LE",         "TK_EQ",
    "TK_GT",        "TK_GE",         "TK_NE",         "TOK",
    "TK_SQL",       "TK_SQR",        "TK_OP",         "TK_CL",
    "TK_COMMA",     "TK_SEM",        "TK_COLON",      "TK_DOT",
    "TK_ID",        "TK_FUNID",      "TK_FIELDID",    "TK_RUID",
    "TK_NUM",       "TK_RNUM",       "TK_EOF"
};

// Shorthand to create terminal grammar symbol.
#define T(tk)   ((GrammarSymbol){SYM_TERMINAL,     {.terminal    = (tk)}})
// Shorthand to create non-terminal grammar symbol.
#define NT(nt)  ((GrammarSymbol){SYM_NON_TERMINAL, {.nonTerminal = (nt)}})

// Fill all grammar rules in rule-number order.
void initGrammar(void) {
    int r = 0; // current rule index

    grammar[r] = (Production){NT_PROGRAM, {NT(NT_OTHERFUNCTIONS), NT(NT_MAINFUNCTION)}, 2, r}; r++; // start rule

    grammar[r] = (Production){NT_MAINFUNCTION, {T(TK_MAIN), NT(NT_STMTS), T(TK_END)}, 3, r}; r++;

    grammar[r] = (Production){NT_OTHERFUNCTIONS, {NT(NT_FUNCTION), NT(NT_OTHERFUNCTIONS)}, 2, r}; r++;

    grammar[r] = (Production){NT_OTHERFUNCTIONS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_FUNCTION,
        {T(TK_FUNID), NT(NT_INPUT_PAR), NT(NT_OUTPUT_PAR), T(TK_SEM), NT(NT_STMTS), T(TK_END)},
        6, r}; r++;

    grammar[r] = (Production){NT_INPUT_PAR,
        {T(TK_INPUT), T(TK_PARAMETER), T(TK_LIST), T(TK_SQL), NT(NT_PARAMETER_LIST), T(TK_SQR)},
        6, r}; r++;

    grammar[r] = (Production){NT_OUTPUT_PAR,
        {T(TK_OUTPUT), T(TK_PARAMETER), T(TK_LIST), T(TK_SQL), NT(NT_PARAMETER_LIST), T(TK_SQR)},
        6, r}; r++;

    grammar[r] = (Production){NT_OUTPUT_PAR, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_PARAMETER_LIST,
        {NT(NT_DATA_TYPE), T(TK_ID), NT(NT_REMAINING_LIST)}, 3, r}; r++;

    grammar[r] = (Production){NT_DATA_TYPE, {NT(NT_PRIMITIVE_DATATYPE)}, 1, r}; r++;

    grammar[r] = (Production){NT_DATA_TYPE, {NT(NT_CONSTRUCTED_DATATYPE)}, 1, r}; r++;

    grammar[r] = (Production){NT_PRIMITIVE_DATATYPE, {T(TK_INT)}, 1, r}; r++;

    grammar[r] = (Production){NT_PRIMITIVE_DATATYPE, {T(TK_REAL)}, 1, r}; r++;

    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_RECORD), T(TK_RUID)}, 2, r}; r++;

    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_UNION), T(TK_RUID)}, 2, r}; r++;

    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_RUID)}, 1, r}; r++;

    grammar[r] = (Production){NT_REMAINING_LIST, {T(TK_COMMA), NT(NT_PARAMETER_LIST)}, 2, r}; r++;

    grammar[r] = (Production){NT_REMAINING_LIST, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_STMTS,
        {NT(NT_TYPE_DEFINITIONS), NT(NT_DECLARATIONS), NT(NT_OTHER_STMTS), NT(NT_RETURN_STMT)},
        4, r}; r++;

    grammar[r] = (Production){NT_TYPE_DEFINITIONS,
        {NT(NT_ACTUAL_OR_REDEFINED), NT(NT_TYPE_DEFINITIONS)}, 2, r}; r++;

    grammar[r] = (Production){NT_TYPE_DEFINITIONS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_ACTUAL_OR_REDEFINED, {NT(NT_TYPE_DEFINITION)}, 1, r}; r++;

    grammar[r] = (Production){NT_ACTUAL_OR_REDEFINED, {NT(NT_DEFINETYPE_STMT)}, 1, r}; r++;

    grammar[r] = (Production){NT_TYPE_DEFINITION,
        {T(TK_RECORD), T(TK_RUID), NT(NT_FIELD_DEFINITIONS), T(TK_ENDRECORD)}, 4, r}; r++;

    grammar[r] = (Production){NT_TYPE_DEFINITION,
        {T(TK_UNION), T(TK_RUID), NT(NT_FIELD_DEFINITIONS), T(TK_ENDUNION)}, 4, r}; r++;

    grammar[r] = (Production){NT_FIELD_DEFINITIONS,
        {NT(NT_FIELD_DEFINITION), NT(NT_FIELD_DEFINITION), NT(NT_MORE_FIELDS)}, 3, r}; r++;

    grammar[r] = (Production){NT_FIELD_DEFINITION,
        {T(TK_TYPE), NT(NT_FIELD_TYPE), T(TK_COLON), T(TK_FIELDID), T(TK_SEM)}, 5, r}; r++;

    grammar[r] = (Production){NT_FIELD_TYPE, {NT(NT_PRIMITIVE_DATATYPE)}, 1, r}; r++;

    grammar[r] = (Production){NT_FIELD_TYPE, {NT(NT_CONSTRUCTED_DATATYPE)}, 1, r}; r++;

    grammar[r] = (Production){NT_MORE_FIELDS,
        {NT(NT_FIELD_DEFINITION), NT(NT_MORE_FIELDS)}, 2, r}; r++;

    grammar[r] = (Production){NT_MORE_FIELDS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_DECLARATIONS,
        {NT(NT_DECLARATION), NT(NT_DECLARATIONS)}, 2, r}; r++;

    grammar[r] = (Production){NT_DECLARATIONS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_DECLARATION,
        {T(TK_TYPE), NT(NT_DATA_TYPE), T(TK_COLON), T(TK_ID), NT(NT_GLOBAL_OR_NOT), T(TK_SEM)},
        6, r}; r++;

    grammar[r] = (Production){NT_GLOBAL_OR_NOT, {T(TK_COLON), T(TK_GLOBAL)}, 2, r}; r++;

    grammar[r] = (Production){NT_GLOBAL_OR_NOT, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_OTHER_STMTS, {NT(NT_STMT), NT(NT_OTHER_STMTS)}, 2, r}; r++;

    grammar[r] = (Production){NT_OTHER_STMTS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_STMT, {NT(NT_ASSIGNMENT_STMT)}, 1, r}; r++;
    grammar[r] = (Production){NT_STMT, {NT(NT_ITERATIVE_STMT)},  1, r}; r++;
    grammar[r] = (Production){NT_STMT, {NT(NT_CONDITIONAL_STMT)},1, r}; r++;
    grammar[r] = (Production){NT_STMT, {NT(NT_IO_STMT)},         1, r}; r++;
    grammar[r] = (Production){NT_STMT, {NT(NT_FUN_CALL_STMT)},   1, r}; r++;

    grammar[r] = (Production){NT_ASSIGNMENT_STMT,
        {NT(NT_SINGLE_OR_REC_ID), T(TK_ASSIGNOP), NT(NT_ARITH_EXPR), T(TK_SEM)}, 4, r}; r++;

    grammar[r] = (Production){NT_SINGLE_OR_REC_ID,
        {T(TK_ID), NT(NT_OPTION_SINGLE_CONSTR)}, 2, r}; r++;

    grammar[r] = (Production){NT_OPTION_SINGLE_CONSTR, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_OPTION_SINGLE_CONSTR,
        {NT(NT_ONE_EXPANSION), NT(NT_MORE_EXPANSIONS)}, 2, r}; r++;

    grammar[r] = (Production){NT_ONE_EXPANSION, {T(TK_DOT), T(TK_FIELDID)}, 2, r}; r++;

    grammar[r] = (Production){NT_MORE_EXPANSIONS,
        {NT(NT_ONE_EXPANSION), NT(NT_MORE_EXPANSIONS)}, 2, r}; r++;

    grammar[r] = (Production){NT_MORE_EXPANSIONS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_FUN_CALL_STMT,
        {NT(NT_OUTPUT_PARAMETERS), T(TK_CALL), T(TK_FUNID), T(TK_WITH),
         T(TK_PARAMETERS), NT(NT_INPUT_PARAMETERS), T(TK_SEM)}, 7, r}; r++;

    grammar[r] = (Production){NT_OUTPUT_PARAMETERS,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR), T(TK_ASSIGNOP)}, 4, r}; r++;

    grammar[r] = (Production){NT_OUTPUT_PARAMETERS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_INPUT_PARAMETERS,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR)}, 3, r}; r++;

    grammar[r] = (Production){NT_ITERATIVE_STMT,
        {T(TK_WHILE), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL),
         NT(NT_STMT), NT(NT_OTHER_STMTS), T(TK_ENDWHILE)}, 7, r}; r++;

    grammar[r] = (Production){NT_CONDITIONAL_STMT,
        {T(TK_IF), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL), T(TK_THEN),
         NT(NT_STMT), NT(NT_OTHER_STMTS), NT(NT_ELSE_PART)}, 8, r}; r++;

    grammar[r] = (Production){NT_ELSE_PART,
        {T(TK_ELSE), NT(NT_STMT), NT(NT_OTHER_STMTS), T(TK_ENDIF)}, 4, r}; r++;

    grammar[r] = (Production){NT_ELSE_PART, {T(TK_ENDIF)}, 1, r}; r++;

    grammar[r] = (Production){NT_IO_STMT,
        {T(TK_READ), T(TK_OP), NT(NT_VAR), T(TK_CL), T(TK_SEM)}, 5, r}; r++;

    grammar[r] = (Production){NT_IO_STMT,
        {T(TK_WRITE), T(TK_OP), NT(NT_VAR), T(TK_CL), T(TK_SEM)}, 5, r}; r++;

    grammar[r] = (Production){NT_ARITH_EXPR, {NT(NT_TERM), NT(NT_EXP_PRIME)}, 2, r}; r++;

    grammar[r] = (Production){NT_EXP_PRIME,
        {NT(NT_LOW_PREC_OP), NT(NT_TERM), NT(NT_EXP_PRIME)}, 3, r}; r++;

    grammar[r] = (Production){NT_EXP_PRIME, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_TERM, {NT(NT_FACTOR), NT(NT_TERM_PRIME)}, 2, r}; r++;

    grammar[r] = (Production){NT_TERM_PRIME,
        {NT(NT_HIGH_PREC_OP), NT(NT_FACTOR), NT(NT_TERM_PRIME)}, 3, r}; r++;

    grammar[r] = (Production){NT_TERM_PRIME, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_FACTOR,
        {T(TK_OP), NT(NT_ARITH_EXPR), T(TK_CL)}, 3, r}; r++;

    grammar[r] = (Production){NT_FACTOR, {NT(NT_VAR)}, 1, r}; r++;

    grammar[r] = (Production){NT_HIGH_PREC_OP, {T(TK_MUL)}, 1, r}; r++;
    grammar[r] = (Production){NT_HIGH_PREC_OP, {T(TK_DIV)}, 1, r}; r++;

    grammar[r] = (Production){NT_LOW_PREC_OP, {T(TK_PLUS)},  1, r}; r++;
    grammar[r] = (Production){NT_LOW_PREC_OP, {T(TK_MINUS)}, 1, r}; r++;

    grammar[r] = (Production){NT_BOOL_EXPR,
        {T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL), NT(NT_LOGICAL_OP),
         T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL)}, 7, r}; r++;

    grammar[r] = (Production){NT_BOOL_EXPR,
        {NT(NT_VAR), NT(NT_RELATIONAL_OP), NT(NT_VAR)}, 3, r}; r++;

    grammar[r] = (Production){NT_BOOL_EXPR,
        {T(TK_NOT), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL)}, 4, r}; r++;

    grammar[r] = (Production){NT_VAR, {NT(NT_SINGLE_OR_REC_ID)}, 1, r}; r++;

    grammar[r] = (Production){NT_VAR, {T(TK_NUM)}, 1, r}; r++;

    grammar[r] = (Production){NT_VAR, {T(TK_RNUM)}, 1, r}; r++;

    grammar[r] = (Production){NT_LOGICAL_OP, {T(TK_AND)}, 1, r}; r++;
    grammar[r] = (Production){NT_LOGICAL_OP, {T(TK_OR)},  1, r}; r++;

    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_LT)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_LE)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_EQ)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_GT)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_GE)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_NE)}, 1, r}; r++;

    grammar[r] = (Production){NT_RETURN_STMT,
        {T(TK_RETURN), NT(NT_OPTIONAL_RETURN), T(TK_SEM)}, 3, r}; r++;

    grammar[r] = (Production){NT_OPTIONAL_RETURN,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR)}, 3, r}; r++;

    grammar[r] = (Production){NT_OPTIONAL_RETURN, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_ID_LIST, {T(TK_ID), NT(NT_MORE_IDS)}, 2, r}; r++;

    grammar[r] = (Production){NT_MORE_IDS, {T(TK_COMMA), NT(NT_ID_LIST)}, 2, r}; r++;

    grammar[r] = (Production){NT_MORE_IDS, {{0}}, 0, r}; r++;

    grammar[r] = (Production){NT_DEFINETYPE_STMT,
        {T(TK_DEFINETYPE), NT(NT_A), T(TK_RUID), T(TK_AS), T(TK_RUID)}, 5, r}; r++;

    grammar[r] = (Production){NT_A, {T(TK_RECORD)}, 1, r}; r++;
    grammar[r] = (Production){NT_A, {T(TK_UNION)},  1, r}; r++;
}

// Insert token into FIRST set, returns true if changed.
static bool addToFirst(bool first[][FIRST_SIZE], NonTerminal nt, int t) {
    if (!first[nt][t]) { first[nt][t] = true; return true; }
    return false;
}
// Insert token into FOLLOW set, returns true if changed.
static bool addToFollow(bool follow[][FOLLOW_SIZE], NonTerminal nt, int t) {
    if (!follow[nt][t]) { follow[nt][t] = true; return true; }
    return false;
}

// Compute FIRST of rhs[start..len-1].
static void firstOfSequence(GrammarSymbol *rhs, int start, int len,
                             bool result[], bool g_first[][FIRST_SIZE]) {
    memset(result, 0, FIRST_SIZE * sizeof(bool));

    if (start >= len) {
        result[EPS_IDX] = true;
        return;
    }

    bool allNullable = true; // stays true only if all symbols can go eps
    for (int i = start; i < len && allNullable; i++) {
        allNullable = false;
        GrammarSymbol sym = rhs[i];

        if (sym.type == SYM_TERMINAL) {
            result[sym.terminal] = true;

        } else {

            for (int t = 0; t < NUM_TOKENS; t++)
                if (g_first[sym.nonTerminal][t])
                    result[t] = true;

            if (g_first[sym.nonTerminal][EPS_IDX])
                allNullable = true;
        }
    }

    if (allNullable)
        result[EPS_IDX] = true;
}

// Fixed-point computation of FIRST and FOLLOW.
FirstAndFollow computeFirstAndFollowSets(void) {

    static bool g_first [NT_COUNT][FIRST_SIZE];
    static bool g_follow[NT_COUNT][FOLLOW_SIZE];

    memset(g_first,  0, sizeof(g_first));
    memset(g_follow, 0, sizeof(g_follow));

    bool changed = true;
    while (changed) {
        changed = false;
        for (int p = 0; p < NUM_PRODUCTIONS; p++) {
            Production *prod = &grammar[p];
            NonTerminal A    = prod->lhs;

            if (prod->rhsLen == 0) {

                if (addToFirst(g_first, A, EPS_IDX)) changed = true;
                continue;
            }

            bool seq[FIRST_SIZE];
            firstOfSequence(prod->rhs, 0, prod->rhsLen, seq, g_first);

            for (int t = 0; t < FIRST_SIZE; t++) {
                if (seq[t]) {
                    if (addToFirst(g_first, A, t)) changed = true;
                }
            }
        }
    }

    g_follow[NT_PROGRAM][TK_EOF] = true; // $ in FOLLOW(start)

    changed = true;
    while (changed) {
        changed = false;
        for (int p = 0; p < NUM_PRODUCTIONS; p++) {
            Production *prod = &grammar[p];
            NonTerminal A    = prod->lhs;

            for (int i = 0; i < prod->rhsLen; i++) {
                GrammarSymbol sym = prod->rhs[i];
                if (sym.type != SYM_NON_TERMINAL) continue;

                NonTerminal B = sym.nonTerminal;

                bool beta[FIRST_SIZE];
                firstOfSequence(prod->rhs, i + 1, prod->rhsLen, beta, g_first);

                for (int t = 0; t < NUM_TOKENS; t++) {
                    if (beta[t])
                        if (addToFollow(g_follow, B, t)) changed = true;
                }

                if (beta[EPS_IDX]) {
                    for (int t = 0; t < FOLLOW_SIZE; t++) {
                        if (g_follow[A][t])
                            if (addToFollow(g_follow, B, t)) changed = true;
                    }
                }
            }
        }
    }

    memcpy(FF.first,  g_first,  sizeof(g_first));
    memcpy(FF.follow, g_follow, sizeof(g_follow));
    return FF;
}

// Print FIRST/FOLLOW sets for debugging.
void printFirstAndFollow(FirstAndFollow *ff) {
    printf("\n========== FIRST SETS ==========\n");
    for (int nt = 0; nt < NT_COUNT; nt++) {
        printf("FIRST(%-25s) = { ", ntNames[nt]);
        for (int t = 0; t < NUM_TOKENS; t++)
            if (ff->first[nt][t]) printf("%s ", tokenNameStr[t]);
        if (ff->first[nt][EPS_IDX]) printf("eps ");
        printf("}\n");
    }

    printf("\n========== FOLLOW SETS ==========\n");
    for (int nt = 0; nt < NT_COUNT; nt++) {
        printf("FOLLOW(%-25s) = { ", ntNames[nt]);
        for (int t = 0; t < FOLLOW_SIZE; t++)
            if (ff->follow[nt][t]) printf("%s ", tokenNameStr[t]);
        printf("}\n");
    }
}

// Build LL(1) parse table from FIRST/FOLLOW.
void createParseTable(FirstAndFollow *ff, ParseTableType table) {

    for (int i = 0; i < NT_COUNT; i++)
        for (int j = 0; j < NUM_TOKENS; j++)
            table[i][j] = -1; // -1 means empty cell

    static bool g_first[NT_COUNT][FIRST_SIZE];
    memcpy(g_first, ff->first, sizeof(g_first));

    for (int p = 0; p < NUM_PRODUCTIONS; p++) {
        Production *prod = &grammar[p];
        NonTerminal A    = prod->lhs;

        bool alpha[FIRST_SIZE];
        firstOfSequence(prod->rhs, 0, prod->rhsLen, alpha, g_first);

        for (int t = 0; t < NUM_TOKENS; t++) {
            if (!alpha[t]) continue;
            if (table[A][t] != -1) {
                // log conflicts but keep latest rule

                fprintf(stderr,
                    "Parse table conflict: NT=%s, token=%s "
                    "(rules %d and %d)\n",
                    ntNames[A], tokenNameStr[t], table[A][t], p);
            }
            table[A][t] = p;
        }

        if (alpha[EPS_IDX]) {
            for (int t = 0; t < FOLLOW_SIZE; t++) {
                if (!ff->follow[A][t]) continue;
                if (table[A][t] != -1) {
                    fprintf(stderr,
                        "Parse table conflict (via eps): NT=%s, token=%s "
                        "(rules %d and %d)\n",
                        ntNames[A], tokenNameStr[t], table[A][t], p);
                }
                table[A][t] = p;
            }
        }
    }
}

// Print non-empty parse table cells.
void printParseTable(ParseTableType table) {
    printf("\n========== PARSE TABLE (non-empty entries) ==========\n");
    printf("%-30s %-20s %s\n", "Non-Terminal", "Token", "Rule#");
    printf("--------------------------------------------------------------\n");
    for (int nt = 0; nt < NT_COUNT; nt++)
        for (int t = 0; t < NUM_TOKENS; t++)
            if (table[nt][t] != -1)
                printf("%-30s %-20s %d\n",
                       ntNames[nt], tokenNameStr[t], table[nt][t]);
}

// Allocate one parse-tree node.
ParseTreeNode *createParseTreeNode(GrammarSymbol sym, ParseTreeNode *parent) {
    ParseTreeNode *node = (ParseTreeNode *)malloc(sizeof(ParseTreeNode));
    if (!node) { fprintf(stderr, "Fatal: parse tree malloc failed\n"); exit(1); }
    node->symbol      = sym;
    node->parent      = parent;
    node->firstChild  = NULL;
    node->nextSibling = NULL;
    node->isLeaf      = false;
    memset(&node->tkInfo, 0, sizeof(tokenInfo));
    return node;
}

// Append child at end of parent's child list.
void addChild(ParseTreeNode *parent, ParseTreeNode *child) {
    if (!parent->firstChild) {
        parent->firstChild = child;
        return;
    }
    ParseTreeNode *cur = parent->firstChild;
    while (cur->nextSibling) cur = cur->nextSibling;
    cur->nextSibling = child;
}

// Flat token stream used by parser for easy lookahead.
typedef struct {
    tokenInfo *data;
    size_t     count;
    size_t     cap;
    size_t     idx;
} TokenStream;

// Initialize empty token stream.
static void tokenStreamInit(TokenStream *ts) {
    ts->data  = NULL;
    ts->count = 0;
    ts->cap   = 0;
    ts->idx   = 0;
}

// Free token stream memory.
static void tokenStreamFree(TokenStream *ts) {
    free(ts->data);
    ts->data  = NULL;
    ts->count = 0;
    ts->cap   = 0;
    ts->idx   = 0;
}

// Push token into dynamic stream buffer.
static bool tokenStreamPush(TokenStream *ts, tokenInfo tk) {
    if (ts->count == ts->cap) {
        size_t newCap = (ts->cap == 0) ? 256 : (ts->cap * 2);
        tokenInfo *newData = (tokenInfo *)realloc(ts->data, newCap * sizeof(tokenInfo));
        if (!newData) return false;
        ts->data = newData;
        ts->cap  = newCap;
    }
    ts->data[ts->count++] = tk;
    return true;
}

// Return next token from stream, EOF if exhausted.
static tokenInfo tokenStreamNext(TokenStream *ts) {
    if (ts->idx < ts->count) return ts->data[ts->idx++];
    tokenInfo eofTk;
    memset(&eofTk, 0, sizeof(tokenInfo));
    eofTk.token = TK_EOF;
    strcpy(eofTk.lexeme, "EOF");
    return eofTk;
}

// Skip comments and bad tokens before parsing.
static tokenInfo nextMeaningfulFromStream(TokenStream *ts) {
    tokenInfo tk;
    while (1) {
        tk = tokenStreamNext(ts);

        if (tk.token == TK_COMMENT) continue; // parser ignores comments

        if (tk.token == TK_ERROR) {
            if (strlen(tk.lexeme) == 1)
                printf("Line %d Error: Unknown Symbol <%s>\n", tk.lineNo, tk.lexeme);
            else
                printf("Line %d Error: Unknown pattern <%s>\n", tk.lineNo, tk.lexeme);
            continue; // report and skip bad lexeme
        }

        if (tk.token == TK_LENGTH_ERROR) {
            printf("Line %d Error: Variable Identifier is longer than the prescribed length of 20 characters.\n",
                   tk.lineNo);
            continue;
        }

        return tk;
    }
}

// Lex full source once and cache tokens.
static bool buildTokenStream(char *testcaseFile, TokenStream *ts) {
    FILE *fp = fopen(testcaseFile, "r");
    if (!fp) {
        printf("Error: Cannot open source file '%s'\n", testcaseFile);
        return false;
    }

    twinBuffer *tb = initializeLexer(fp);
    if (!tb) {
        fclose(fp);
        fprintf(stderr, "Fatal: lexer initialization failed.\n");
        return false;
    }

    tokenStreamInit(ts);

    while (1) {
        tokenInfo tk = getNextToken(tb);
        if (!tokenStreamPush(ts, tk)) {
            fprintf(stderr, "Fatal: token stream allocation failed.\n");
            free(tb);
            fclose(fp);
            tokenStreamFree(ts);
            return false;
        }

        if (tk.token == TK_EOF) break; // stop once EOF is captured
    }

    free(tb);
    fclose(fp);
    return true;
}

// Main predictive parser routine.
ParseTreeNode *parseInputSourceCode(char *testcaseFile, ParseTableType table) {
    TokenStream stream; // cached lexer output
    if (!buildTokenStream(testcaseFile, &stream))
        return NULL;

    GrammarSymbol startSym = NT(NT_PROGRAM); // parser start symbol
    ParseTreeNode *root    = createParseTreeNode(startSym, NULL);
    root->parent           = NULL;

    ParseStack stack; // LL(1) parsing stack
    stack.top  = NULL;
    stack.size = 0;

    GrammarSymbol dollarSym = {SYM_TERMINAL, {.terminal = TK_EOF}};
    stackPush(&stack, dollarSym, NULL); // push $
    stackPush(&stack, startSym, root);

    tokenInfo lookahead = nextMeaningfulFromStream(&stream); // current input token

    bool syntaxOK = true; // flips false on any syntax error

    while (!stackIsEmpty(&stack)) { // standard LL(1) loop
        StackElem *topElem = stackPeek(&stack);
        GrammarSymbol top  = topElem->sym;

        if (top.type == SYM_TERMINAL) {
            if (top.terminal == lookahead.token) {

                ParseTreeNode *leaf = topElem->treeNode;
                if (leaf) {
                    leaf->isLeaf  = true;
                    leaf->tkInfo  = lookahead;
                    leaf->symbol  = top;
                }
                StackElem *popped = stackPop(&stack);
                free(popped);

                if (lookahead.token != TK_EOF)
                    lookahead = nextMeaningfulFromStream(&stream);
            } else {

                printf("Line %d Error: The token %s for lexeme %s does not match with the expected token %s\n",
                       lookahead.lineNo,
                       (lookahead.token < NUM_TOKENS) ? tokenNameStr[lookahead.token] : "UNKNOWN",
                       lookahead.lexeme,
                       tokenNameStr[top.terminal]);
                syntaxOK = false;
                StackElem *popped = stackPop(&stack);
                free(popped);

            }
            continue;
        }

        NonTerminal A = top.nonTerminal;
        int token     = (int)lookahead.token;

        if (token < 0 || token >= NUM_TOKENS) {
            printf("Line %d Error: Invalid token UNKNOWN encountered with value %s stack top %s\n",
                   lookahead.lineNo, lookahead.lexeme, ntNames[A]);
            syntaxOK = false;
            StackElem *popped = stackPop(&stack);
            free(popped);
            if (lookahead.token != TK_EOF)
                lookahead = nextMeaningfulFromStream(&stream);
            continue;
        }

        int ruleIdx = table[A][token];

        if (ruleIdx == -1) {

            if (lookahead.token == TK_EOF) {
                StackElem *popped = stackPop(&stack);
                free(popped);
                continue;
            }

            if ((A == NT_OPTION_SINGLE_CONSTR || A == NT_TERM_PRIME || A == NT_EXP_PRIME) &&
                lookahead.token == TK_ELSE) {
                StackElem *popped = stackPop(&stack);
                free(popped);
                continue;
            }

            printf("Line %d Error: Invalid token %s encountered with value %s stack top %s\n",
                   lookahead.lineNo,
                   (lookahead.token < NUM_TOKENS) ? tokenNameStr[lookahead.token] : "UNKNOWN",
                   lookahead.lexeme,
                   ntNames[A]);
            syntaxOK = false;

            while (lookahead.token != TK_EOF &&
                   !(lookahead.token >= 0 &&
                     lookahead.token < FOLLOW_SIZE &&
                     FF.follow[A][lookahead.token])) {
                lookahead = nextMeaningfulFromStream(&stream);
            }
            StackElem *popped = stackPop(&stack);
            free(popped);
            continue;
        }

        Production *prod      = &grammar[ruleIdx];
        ParseTreeNode *ntNode = topElem->treeNode;
        StackElem *popped     = stackPop(&stack);
        free(popped);

        if (prod->rhsLen == 0) {

            GrammarSymbol epsSym;
            epsSym.type = SYM_EPSILON;
            epsSym.terminal = 0;
            ParseTreeNode *epsNode = createParseTreeNode(epsSym, ntNode);
            epsNode->isLeaf = true;
            if (ntNode) addChild(ntNode, epsNode);
        } else {

            ParseTreeNode *children[MAX_RHS];
            for (int i = 0; i < prod->rhsLen; i++) {
                children[i] = createParseTreeNode(prod->rhs[i], ntNode);
                if (ntNode) addChild(ntNode, children[i]);
            }
            for (int i = prod->rhsLen - 1; i >= 0; i--)
                stackPush(&stack, prod->rhs[i], children[i]);
        }
    }

    tokenStreamFree(&stream);

    if (syntaxOK)
        printf("\nInput source code is syntactically correct...........\n");
    else
        printf("\nInput source code has syntax errors (see above).\n");

    stackFree(&stack);
    return root;
}

// Human-readable symbol name helper.
static const char *symName(GrammarSymbol sym) {
    if (sym.type == SYM_NON_TERMINAL)
        return ntNames[sym.nonTerminal];
    if (sym.type == SYM_TERMINAL && sym.terminal < NUM_TOKENS)
        return tokenNameStr[sym.terminal];
    return "eps";
}

// Parent symbol name helper for tree print.
static const char *parentSymName(ParseTreeNode *node) {
    if (!node->parent) return "ROOT";
    return symName(node->parent->symbol);
}

// Recursively free subtree.
static void freeParseTreeNodes(ParseTreeNode *node) {
    if (!node) return;

    ParseTreeNode *child = node->firstChild;
    while (child) {
        ParseTreeNode *nextSib = child->nextSibling;
        freeParseTreeNodes(child);
        child = nextSib;
    }

    free(node);
}

// Public parse-tree free wrapper.
void freeParseTree(ParseTreeNode *root) {
    freeParseTreeNodes(root);
}

// Print one parse-tree node as a table row.
static void printNode(ParseTreeNode *node, FILE *fp) {
    if (!node) return;

    char lexemeField[MAX_LEXEME_LEN + 4];
    if (node->isLeaf && node->symbol.type == SYM_TERMINAL)
        snprintf(lexemeField, sizeof(lexemeField), "%s", node->tkInfo.lexeme);
    else if (node->symbol.type == SYM_EPSILON)
        snprintf(lexemeField, sizeof(lexemeField), "eps");
    else
        snprintf(lexemeField, sizeof(lexemeField), "----");

    int lineNo = node->isLeaf ? node->tkInfo.lineNo : 0;

    const char *tokName = symName(node->symbol);

    char valueField[32] = "---";
    if (node->isLeaf && node->symbol.type == SYM_TERMINAL) {
        if (node->tkInfo.token == TK_NUM)
            snprintf(valueField, sizeof(valueField), "%d", node->tkInfo.val.intValue);
        else if (node->tkInfo.token == TK_RNUM)
            snprintf(valueField, sizeof(valueField), "%f", node->tkInfo.val.realValue);
    }

    const char *parentSym = parentSymName(node);

    const char *isLeaf = node->isLeaf ? "yes" : "no";

    char nodeSymField[64];
    if (!node->isLeaf && node->symbol.type == SYM_NON_TERMINAL)
        snprintf(nodeSymField, sizeof(nodeSymField), "%s", ntNames[node->symbol.nonTerminal]);
    else
        snprintf(nodeSymField, sizeof(nodeSymField), "---");

    fprintf(fp, "%-20s %-6d %-20s %-12s %-25s %-6s %s\n",
            lexemeField, lineNo, tokName, valueField,
            parentSym, isLeaf, nodeSymField);
}

// Inorder-style traversal used for output file.
static void inorderPrint(ParseTreeNode *node, FILE *fp) {
    if (!node) return;

    if (!node->firstChild) {

        printNode(node, fp);
        return;
    }

    inorderPrint(node->firstChild, fp);

    printNode(node, fp);

    ParseTreeNode *sib = node->firstChild->nextSibling;
    while (sib) {
        inorderPrint(sib, fp);
        sib = sib->nextSibling;
    }
}

// Write full parse tree to output file.
void printParseTree(ParseTreeNode *root, char *outfile) {
    if (!root) {
        printf("No parse tree to print (parsing failed).\n");
        return;
    }
    FILE *fp = fopen(outfile, "w");
    if (!fp) {
        printf("Error: Cannot open output file '%s'\n", outfile);
        return;
    }

    fprintf(fp, "%-20s %-6s %-20s %-12s %-25s %-6s %s\n",
            "Lexeme", "Line#", "TokenName", "Value", "ParentNodeSym",
            "isLeaf", "NodeSym");
    fprintf(fp, "%-20s %-6s %-20s %-12s %-25s %-6s %s\n",
            "------", "-----", "---------", "-----", "-------------",
            "------", "-------");
    inorderPrint(root, fp);
    fclose(fp);
    printf("Parse tree written to '%s'\n", outfile);
}
