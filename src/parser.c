/*
 * parser.c
 * CS F363 - Compiler Construction, BITS Pilani, Jan 2026
 *
 * Implements:
 *   - Grammar initialisation (95 production rules, rules 0-94)
 *   - Automated iterative FIRST / FOLLOW set computation
 *   - Predictive parse table construction
 *   - Stack-based LL(1) parsing with parse-tree construction
 *   - N-ary inorder parse-tree printing to file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parser.h"
#include "lexer.h"
#include "parserDef.h"

/* ==========================================================
   GLOBAL DEFINITIONS
   ========================================================== */
Production     grammar[NUM_PRODUCTIONS];
FirstAndFollow FF;
ParseTableType parseTable;

/* Human-readable non-terminal names (must match NonTerminal enum order) */
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

/* Token name strings indexed by TokenName enum value */
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

/* Shorthand macros for building grammar RHS */
#define T(tk)   ((GrammarSymbol){SYM_TERMINAL,     {.terminal    = (tk)}})
#define NT(nt)  ((GrammarSymbol){SYM_NON_TERMINAL, {.nonTerminal = (nt)}})

/* ==========================================================
   GRAMMAR INITIALISATION  (95 rules, indices 0-94)
   ========================================================== */
void initGrammar(void) {
    int r = 0;

    /* 0: program → otherFunctions mainFunction */
    grammar[r] = (Production){NT_PROGRAM, {NT(NT_OTHERFUNCTIONS), NT(NT_MAINFUNCTION)}, 2, r}; r++;

    /* 1: mainFunction → TK_MAIN stmts TK_END */
    grammar[r] = (Production){NT_MAINFUNCTION, {T(TK_MAIN), NT(NT_STMTS), T(TK_END)}, 3, r}; r++;

    /* 2: otherFunctions → function otherFunctions */
    grammar[r] = (Production){NT_OTHERFUNCTIONS, {NT(NT_FUNCTION), NT(NT_OTHERFUNCTIONS)}, 2, r}; r++;

    /* 3: otherFunctions → eps */
    grammar[r] = (Production){NT_OTHERFUNCTIONS, {}, 0, r}; r++;

    /* 4: function → TK_FUNID input_par output_par TK_SEM stmts TK_END */
    grammar[r] = (Production){NT_FUNCTION,
        {T(TK_FUNID), NT(NT_INPUT_PAR), NT(NT_OUTPUT_PAR), T(TK_SEM), NT(NT_STMTS), T(TK_END)},
        6, r}; r++;

    /* 5: input_par → TK_INPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR */
    grammar[r] = (Production){NT_INPUT_PAR,
        {T(TK_INPUT), T(TK_PARAMETER), T(TK_LIST), T(TK_SQL), NT(NT_PARAMETER_LIST), T(TK_SQR)},
        6, r}; r++;

    /* 6: output_par → TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL parameter_list TK_SQR */
    grammar[r] = (Production){NT_OUTPUT_PAR,
        {T(TK_OUTPUT), T(TK_PARAMETER), T(TK_LIST), T(TK_SQL), NT(NT_PARAMETER_LIST), T(TK_SQR)},
        6, r}; r++;

    /* 7: output_par → eps */
    grammar[r] = (Production){NT_OUTPUT_PAR, {}, 0, r}; r++;

    /* 8: parameter_list → dataType TK_ID remaining_list */
    grammar[r] = (Production){NT_PARAMETER_LIST,
        {NT(NT_DATA_TYPE), T(TK_ID), NT(NT_REMAINING_LIST)}, 3, r}; r++;

    /* 9: dataType → primitiveDatatype */
    grammar[r] = (Production){NT_DATA_TYPE, {NT(NT_PRIMITIVE_DATATYPE)}, 1, r}; r++;

    /* 10: dataType → constructedDatatype */
    grammar[r] = (Production){NT_DATA_TYPE, {NT(NT_CONSTRUCTED_DATATYPE)}, 1, r}; r++;

    /* 11: primitiveDatatype → TK_INT */
    grammar[r] = (Production){NT_PRIMITIVE_DATATYPE, {T(TK_INT)}, 1, r}; r++;

    /* 12: primitiveDatatype → TK_REAL */
    grammar[r] = (Production){NT_PRIMITIVE_DATATYPE, {T(TK_REAL)}, 1, r}; r++;

    /* 13: constructedDatatype → TK_RECORD TK_RUID */
    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_RECORD), T(TK_RUID)}, 2, r}; r++;

    /* 14: constructedDatatype → TK_UNION TK_RUID */
    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_UNION), T(TK_RUID)}, 2, r}; r++;

    /* 15: constructedDatatype → TK_RUID  [alias/typedef usage] */
    grammar[r] = (Production){NT_CONSTRUCTED_DATATYPE, {T(TK_RUID)}, 1, r}; r++;

    /* 16: remaining_list → TK_COMMA parameter_list */
    grammar[r] = (Production){NT_REMAINING_LIST, {T(TK_COMMA), NT(NT_PARAMETER_LIST)}, 2, r}; r++;

    /* 17: remaining_list → eps */
    grammar[r] = (Production){NT_REMAINING_LIST, {}, 0, r}; r++;

    /* 18: stmts → typeDefinitions declarations otherStmts returnStmt */
    grammar[r] = (Production){NT_STMTS,
        {NT(NT_TYPE_DEFINITIONS), NT(NT_DECLARATIONS), NT(NT_OTHER_STMTS), NT(NT_RETURN_STMT)},
        4, r}; r++;

    /* 19: typeDefinitions → actualOrRedefined typeDefinitions */
    grammar[r] = (Production){NT_TYPE_DEFINITIONS,
        {NT(NT_ACTUAL_OR_REDEFINED), NT(NT_TYPE_DEFINITIONS)}, 2, r}; r++;

    /* 20: typeDefinitions → eps */
    grammar[r] = (Production){NT_TYPE_DEFINITIONS, {}, 0, r}; r++;

    /* 21: actualOrRedefined → typeDefinition */
    grammar[r] = (Production){NT_ACTUAL_OR_REDEFINED, {NT(NT_TYPE_DEFINITION)}, 1, r}; r++;

    /* 22: actualOrRedefined → definetypeStmt */
    grammar[r] = (Production){NT_ACTUAL_OR_REDEFINED, {NT(NT_DEFINETYPE_STMT)}, 1, r}; r++;

    /* 23: typeDefinition → TK_RECORD TK_RUID fieldDefinitions TK_ENDRECORD */
    grammar[r] = (Production){NT_TYPE_DEFINITION,
        {T(TK_RECORD), T(TK_RUID), NT(NT_FIELD_DEFINITIONS), T(TK_ENDRECORD)}, 4, r}; r++;

    /* 24: typeDefinition → TK_UNION TK_RUID fieldDefinitions TK_ENDUNION */
    grammar[r] = (Production){NT_TYPE_DEFINITION,
        {T(TK_UNION), T(TK_RUID), NT(NT_FIELD_DEFINITIONS), T(TK_ENDUNION)}, 4, r}; r++;

    /* 25: fieldDefinitions → fieldDefinition fieldDefinition moreFields */
    grammar[r] = (Production){NT_FIELD_DEFINITIONS,
        {NT(NT_FIELD_DEFINITION), NT(NT_FIELD_DEFINITION), NT(NT_MORE_FIELDS)}, 3, r}; r++;

    /* 26: fieldDefinition → TK_TYPE fieldType TK_COLON TK_FIELDID TK_SEM */
    grammar[r] = (Production){NT_FIELD_DEFINITION,
        {T(TK_TYPE), NT(NT_FIELD_TYPE), T(TK_COLON), T(TK_FIELDID), T(TK_SEM)}, 5, r}; r++;

    /* 27: fieldType → primitiveDatatype */
    grammar[r] = (Production){NT_FIELD_TYPE, {NT(NT_PRIMITIVE_DATATYPE)}, 1, r}; r++;

    /* 28: fieldType → constructedDatatype */
    grammar[r] = (Production){NT_FIELD_TYPE, {NT(NT_CONSTRUCTED_DATATYPE)}, 1, r}; r++;

    /* 29: moreFields → fieldDefinition moreFields */
    grammar[r] = (Production){NT_MORE_FIELDS,
        {NT(NT_FIELD_DEFINITION), NT(NT_MORE_FIELDS)}, 2, r}; r++;

    /* 30: moreFields → eps */
    grammar[r] = (Production){NT_MORE_FIELDS, {}, 0, r}; r++;

    /* 31: declarations → declaration declarations */
    grammar[r] = (Production){NT_DECLARATIONS,
        {NT(NT_DECLARATION), NT(NT_DECLARATIONS)}, 2, r}; r++;

    /* 32: declarations → eps */
    grammar[r] = (Production){NT_DECLARATIONS, {}, 0, r}; r++;

    /* 33: declaration → TK_TYPE dataType TK_COLON TK_ID global_or_not TK_SEM */
    grammar[r] = (Production){NT_DECLARATION,
        {T(TK_TYPE), NT(NT_DATA_TYPE), T(TK_COLON), T(TK_ID), NT(NT_GLOBAL_OR_NOT), T(TK_SEM)},
        6, r}; r++;

    /* 34: global_or_not → TK_COLON TK_GLOBAL */
    grammar[r] = (Production){NT_GLOBAL_OR_NOT, {T(TK_COLON), T(TK_GLOBAL)}, 2, r}; r++;

    /* 35: global_or_not → eps */
    grammar[r] = (Production){NT_GLOBAL_OR_NOT, {}, 0, r}; r++;

    /* 36: otherStmts → stmt otherStmts */
    grammar[r] = (Production){NT_OTHER_STMTS, {NT(NT_STMT), NT(NT_OTHER_STMTS)}, 2, r}; r++;

    /* 37: otherStmts → eps */
    grammar[r] = (Production){NT_OTHER_STMTS, {}, 0, r}; r++;

    /* 38-42: stmt alternatives */
    grammar[r] = (Production){NT_STMT, {NT(NT_ASSIGNMENT_STMT)}, 1, r}; r++;  /* 38 */
    grammar[r] = (Production){NT_STMT, {NT(NT_ITERATIVE_STMT)},  1, r}; r++;  /* 39 */
    grammar[r] = (Production){NT_STMT, {NT(NT_CONDITIONAL_STMT)},1, r}; r++;  /* 40 */
    grammar[r] = (Production){NT_STMT, {NT(NT_IO_STMT)},         1, r}; r++;  /* 41 */
    grammar[r] = (Production){NT_STMT, {NT(NT_FUN_CALL_STMT)},   1, r}; r++;  /* 42 */

    /* 43: assignmentStmt → singleOrRecId TK_ASSIGNOP arithmeticExpression TK_SEM */
    grammar[r] = (Production){NT_ASSIGNMENT_STMT,
        {NT(NT_SINGLE_OR_REC_ID), T(TK_ASSIGNOP), NT(NT_ARITH_EXPR), T(TK_SEM)}, 4, r}; r++;

    /* 44: singleOrRecId → TK_ID optionSingleConstructed */
    grammar[r] = (Production){NT_SINGLE_OR_REC_ID,
        {T(TK_ID), NT(NT_OPTION_SINGLE_CONSTR)}, 2, r}; r++;

    /* 45: optionSingleConstructed → eps */
    grammar[r] = (Production){NT_OPTION_SINGLE_CONSTR, {}, 0, r}; r++;

    /* 46: optionSingleConstructed → oneExpansion moreExpansions */
    grammar[r] = (Production){NT_OPTION_SINGLE_CONSTR,
        {NT(NT_ONE_EXPANSION), NT(NT_MORE_EXPANSIONS)}, 2, r}; r++;

    /* 47: oneExpansion → TK_DOT TK_FIELDID */
    grammar[r] = (Production){NT_ONE_EXPANSION, {T(TK_DOT), T(TK_FIELDID)}, 2, r}; r++;

    /* 48: moreExpansions → oneExpansion moreExpansions */
    grammar[r] = (Production){NT_MORE_EXPANSIONS,
        {NT(NT_ONE_EXPANSION), NT(NT_MORE_EXPANSIONS)}, 2, r}; r++;

    /* 49: moreExpansions → eps */
    grammar[r] = (Production){NT_MORE_EXPANSIONS, {}, 0, r}; r++;

    /* 50: funCallStmt → outputParameters TK_CALL TK_FUNID TK_WITH TK_PARAMETERS inputParameters TK_SEM */
    grammar[r] = (Production){NT_FUN_CALL_STMT,
        {NT(NT_OUTPUT_PARAMETERS), T(TK_CALL), T(TK_FUNID), T(TK_WITH),
         T(TK_PARAMETERS), NT(NT_INPUT_PARAMETERS), T(TK_SEM)}, 7, r}; r++;

    /* 51: outputParameters → TK_SQL idList TK_SQR TK_ASSIGNOP */
    grammar[r] = (Production){NT_OUTPUT_PARAMETERS,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR), T(TK_ASSIGNOP)}, 4, r}; r++;

    /* 52: outputParameters → eps */
    grammar[r] = (Production){NT_OUTPUT_PARAMETERS, {}, 0, r}; r++;

    /* 53: inputParameters → TK_SQL idList TK_SQR */
    grammar[r] = (Production){NT_INPUT_PARAMETERS,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR)}, 3, r}; r++;

    /* 54: iterativeStmt → TK_WHILE TK_OP boolExpr TK_CL stmt otherStmts TK_ENDWHILE */
    grammar[r] = (Production){NT_ITERATIVE_STMT,
        {T(TK_WHILE), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL),
         NT(NT_STMT), NT(NT_OTHER_STMTS), T(TK_ENDWHILE)}, 7, r}; r++;

    /* 55: conditionalStmt → TK_IF TK_OP boolExpr TK_CL TK_THEN stmt otherStmts elsePart */
    grammar[r] = (Production){NT_CONDITIONAL_STMT,
        {T(TK_IF), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL), T(TK_THEN),
         NT(NT_STMT), NT(NT_OTHER_STMTS), NT(NT_ELSE_PART)}, 8, r}; r++;

    /* 56: elsePart → TK_ELSE stmt otherStmts TK_ENDIF */
    grammar[r] = (Production){NT_ELSE_PART,
        {T(TK_ELSE), NT(NT_STMT), NT(NT_OTHER_STMTS), T(TK_ENDIF)}, 4, r}; r++;

    /* 57: elsePart → TK_ENDIF */
    grammar[r] = (Production){NT_ELSE_PART, {T(TK_ENDIF)}, 1, r}; r++;

    /* 58: ioStmt → TK_READ TK_OP var TK_CL TK_SEM */
    grammar[r] = (Production){NT_IO_STMT,
        {T(TK_READ), T(TK_OP), NT(NT_VAR), T(TK_CL), T(TK_SEM)}, 5, r}; r++;

    /* 59: ioStmt → TK_WRITE TK_OP var TK_CL TK_SEM */
    grammar[r] = (Production){NT_IO_STMT,
        {T(TK_WRITE), T(TK_OP), NT(NT_VAR), T(TK_CL), T(TK_SEM)}, 5, r}; r++;

    /* 60: arithmeticExpression → term expPrime */
    grammar[r] = (Production){NT_ARITH_EXPR, {NT(NT_TERM), NT(NT_EXP_PRIME)}, 2, r}; r++;

    /* 61: expPrime → lowPrecedenceOperators term expPrime */
    grammar[r] = (Production){NT_EXP_PRIME,
        {NT(NT_LOW_PREC_OP), NT(NT_TERM), NT(NT_EXP_PRIME)}, 3, r}; r++;

    /* 62: expPrime → eps */
    grammar[r] = (Production){NT_EXP_PRIME, {}, 0, r}; r++;

    /* 63: term → factor termPrime */
    grammar[r] = (Production){NT_TERM, {NT(NT_FACTOR), NT(NT_TERM_PRIME)}, 2, r}; r++;

    /* 64: termPrime → highPrecedenceOperators factor termPrime */
    grammar[r] = (Production){NT_TERM_PRIME,
        {NT(NT_HIGH_PREC_OP), NT(NT_FACTOR), NT(NT_TERM_PRIME)}, 3, r}; r++;

    /* 65: termPrime → eps */
    grammar[r] = (Production){NT_TERM_PRIME, {}, 0, r}; r++;

    /* 66: factor → TK_OP arithmeticExpression TK_CL */
    grammar[r] = (Production){NT_FACTOR,
        {T(TK_OP), NT(NT_ARITH_EXPR), T(TK_CL)}, 3, r}; r++;

    /* 67: factor → var */
    grammar[r] = (Production){NT_FACTOR, {NT(NT_VAR)}, 1, r}; r++;

    /* 68-69: highPrecedenceOperators */
    grammar[r] = (Production){NT_HIGH_PREC_OP, {T(TK_MUL)}, 1, r}; r++;
    grammar[r] = (Production){NT_HIGH_PREC_OP, {T(TK_DIV)}, 1, r}; r++;

    /* 70-71: lowPrecedenceOperators */
    grammar[r] = (Production){NT_LOW_PREC_OP, {T(TK_PLUS)},  1, r}; r++;
    grammar[r] = (Production){NT_LOW_PREC_OP, {T(TK_MINUS)}, 1, r}; r++;

    /* 72: booleanExpression → TK_OP boolExpr TK_CL logicalOp TK_OP boolExpr TK_CL */
    grammar[r] = (Production){NT_BOOL_EXPR,
        {T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL), NT(NT_LOGICAL_OP),
         T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL)}, 7, r}; r++;

    /* 73: booleanExpression → var relationalOp var */
    grammar[r] = (Production){NT_BOOL_EXPR,
        {NT(NT_VAR), NT(NT_RELATIONAL_OP), NT(NT_VAR)}, 3, r}; r++;

    /* 74: booleanExpression → TK_NOT TK_OP boolExpr TK_CL */
    grammar[r] = (Production){NT_BOOL_EXPR,
        {T(TK_NOT), T(TK_OP), NT(NT_BOOL_EXPR), T(TK_CL)}, 4, r}; r++;

    /* 75: var → singleOrRecId */
    grammar[r] = (Production){NT_VAR, {NT(NT_SINGLE_OR_REC_ID)}, 1, r}; r++;

    /* 76: var → TK_NUM */
    grammar[r] = (Production){NT_VAR, {T(TK_NUM)}, 1, r}; r++;

    /* 77: var → TK_RNUM */
    grammar[r] = (Production){NT_VAR, {T(TK_RNUM)}, 1, r}; r++;

    /* 78-79: logicalOp */
    grammar[r] = (Production){NT_LOGICAL_OP, {T(TK_AND)}, 1, r}; r++;
    grammar[r] = (Production){NT_LOGICAL_OP, {T(TK_OR)},  1, r}; r++;

    /* 80-85: relationalOp */
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_LT)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_LE)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_EQ)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_GT)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_GE)}, 1, r}; r++;
    grammar[r] = (Production){NT_RELATIONAL_OP, {T(TK_NE)}, 1, r}; r++;

    /* 86: returnStmt → TK_RETURN optionalReturn TK_SEM */
    grammar[r] = (Production){NT_RETURN_STMT,
        {T(TK_RETURN), NT(NT_OPTIONAL_RETURN), T(TK_SEM)}, 3, r}; r++;

    /* 87: optionalReturn → TK_SQL idList TK_SQR */
    grammar[r] = (Production){NT_OPTIONAL_RETURN,
        {T(TK_SQL), NT(NT_ID_LIST), T(TK_SQR)}, 3, r}; r++;

    /* 88: optionalReturn → eps */
    grammar[r] = (Production){NT_OPTIONAL_RETURN, {}, 0, r}; r++;

    /* 89: idList → TK_ID moreIds */
    grammar[r] = (Production){NT_ID_LIST, {T(TK_ID), NT(NT_MORE_IDS)}, 2, r}; r++;

    /* 90: moreIds → TK_COMMA idList */
    grammar[r] = (Production){NT_MORE_IDS, {T(TK_COMMA), NT(NT_ID_LIST)}, 2, r}; r++;

    /* 91: moreIds → eps */
    grammar[r] = (Production){NT_MORE_IDS, {}, 0, r}; r++;

    /* 92: definetypeStmt → TK_DEFINETYPE A TK_RUID TK_AS TK_RUID */
    grammar[r] = (Production){NT_DEFINETYPE_STMT,
        {T(TK_DEFINETYPE), NT(NT_A), T(TK_RUID), T(TK_AS), T(TK_RUID)}, 5, r}; r++;

    /* 93-94: A */
    grammar[r] = (Production){NT_A, {T(TK_RECORD)}, 1, r}; r++;
    grammar[r] = (Production){NT_A, {T(TK_UNION)},  1, r}; r++;
}

/* ==========================================================
   FIRST / FOLLOW SET COMPUTATION  (automated, iterative)
   ========================================================== */

/* Add terminal t to first[nt]. Returns true if set changed. */
static bool addToFirst(bool first[][FIRST_SIZE], NonTerminal nt, int t) {
    if (!first[nt][t]) { first[nt][t] = true; return true; }
    return false;
}
static bool addToFollow(bool follow[][FOLLOW_SIZE], NonTerminal nt, int t) {
    if (!follow[nt][t]) { follow[nt][t] = true; return true; }
    return false;
}

/*
 * firstOfSequence: Compute FIRST of rhs[start..len-1] into result[].
 * result[0..NUM_TOKENS-1] = terminals; result[EPS_IDX] = can-derive-eps.
 * Uses the currently computed g_first table.
 */
static void firstOfSequence(GrammarSymbol *rhs, int start, int len,
                             bool result[], bool g_first[][FIRST_SIZE]) {
    memset(result, 0, FIRST_SIZE * sizeof(bool));

    if (start >= len) {          /* empty sequence → derives epsilon */
        result[EPS_IDX] = true;
        return;
    }

    bool allNullable = true;
    for (int i = start; i < len && allNullable; i++) {
        allNullable = false;
        GrammarSymbol sym = rhs[i];

        if (sym.type == SYM_TERMINAL) {
            result[sym.terminal] = true;
            /* terminal cannot derive epsilon */
        } else {
            /* Copy FIRST of this NT (excluding epsilon) into result */
            for (int t = 0; t < NUM_TOKENS; t++)
                if (g_first[sym.nonTerminal][t])
                    result[t] = true;

            if (g_first[sym.nonTerminal][EPS_IDX])
                allNullable = true;   /* NT is nullable → continue */
        }
    }

    if (allNullable)
        result[EPS_IDX] = true;
}

FirstAndFollow computeFirstAndFollowSets(void) {
    /* Local working copies */
    static bool g_first [NT_COUNT][FIRST_SIZE];
    static bool g_follow[NT_COUNT][FOLLOW_SIZE];

    memset(g_first,  0, sizeof(g_first));
    memset(g_follow, 0, sizeof(g_follow));

    /* ---------- Compute FIRST sets (iterate until stable) ---------- */
    bool changed = true;
    while (changed) {
        changed = false;
        for (int p = 0; p < NUM_PRODUCTIONS; p++) {
            Production *prod = &grammar[p];
            NonTerminal A    = prod->lhs;

            if (prod->rhsLen == 0) {
                /* epsilon production */
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

    /* ---------- Compute FOLLOW sets (iterate until stable) ---------- */
    /* Start symbol rule: $ (TK_EOF) in FOLLOW(program) */
    g_follow[NT_PROGRAM][TK_EOF] = true;

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

                /* FIRST(beta) where beta = rhs[i+1..end] */
                bool beta[FIRST_SIZE];
                firstOfSequence(prod->rhs, i + 1, prod->rhsLen, beta, g_first);

                /* Add FIRST(beta) \ {eps} to FOLLOW(B) */
                for (int t = 0; t < NUM_TOKENS; t++) {
                    if (beta[t])
                        if (addToFollow(g_follow, B, t)) changed = true;
                }

                /* If beta can derive eps, add FOLLOW(A) to FOLLOW(B) */
                if (beta[EPS_IDX]) {
                    for (int t = 0; t < FOLLOW_SIZE; t++) {
                        if (g_follow[A][t])
                            if (addToFollow(g_follow, B, t)) changed = true;
                    }
                }
            }
        }
    }

    /* Copy into the return structure */
    memcpy(FF.first,  g_first,  sizeof(g_first));
    memcpy(FF.follow, g_follow, sizeof(g_follow));
    return FF;
}

/* Print FIRST and FOLLOW sets (option in driver for verification) */
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

/* ==========================================================
   PREDICTIVE PARSE TABLE CONSTRUCTION
   ========================================================== */
void createParseTable(FirstAndFollow *ff, ParseTableType table) {
    /* Initialise all entries to -1 (no rule) */
    for (int i = 0; i < NT_COUNT; i++)
        for (int j = 0; j < NUM_TOKENS; j++)
            table[i][j] = -1;

    static bool g_first[NT_COUNT][FIRST_SIZE];
    memcpy(g_first, ff->first, sizeof(g_first));

    for (int p = 0; p < NUM_PRODUCTIONS; p++) {
        Production *prod = &grammar[p];
        NonTerminal A    = prod->lhs;

        /* Compute FIRST of the RHS */
        bool alpha[FIRST_SIZE];
        firstOfSequence(prod->rhs, 0, prod->rhsLen, alpha, g_first);

        /* For each terminal a in FIRST(alpha), set table[A][a] = p */
        for (int t = 0; t < NUM_TOKENS; t++) {
            if (!alpha[t]) continue;
            if (table[A][t] != -1) {
                /* LL(1) conflict — report but continue (last rule wins) */
                fprintf(stderr,
                    "Parse table conflict: NT=%s, token=%s "
                    "(rules %d and %d)\n",
                    ntNames[A], tokenNameStr[t], table[A][t], p);
            }
            table[A][t] = p;
        }

        /* If eps in FIRST(alpha), use FOLLOW(A) */
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

/* ==========================================================
   PARSE TREE NODE HELPERS
   ========================================================== */
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

/* Appends child to the end of parent's child list */
void addChild(ParseTreeNode *parent, ParseTreeNode *child) {
    if (!parent->firstChild) {
        parent->firstChild = child;
        return;
    }
    ParseTreeNode *cur = parent->firstChild;
    while (cur->nextSibling) cur = cur->nextSibling;
    cur->nextSibling = child;
}

/* ==========================================================
   PARSING  (stack-based LL(1))
   ========================================================== */
/*
 * Skip tokens that are TK_COMMENT, TK_ERROR, TK_LENGTH_ERROR.
 * Lexical errors are printed; errors/comments are not passed to parser.
 */
static tokenInfo nextMeaningfulToken(twinBuffer *tb) {
    tokenInfo tk;
    while (1) {
        tk = getNextToken(tb);
        if (tk.token == TK_COMMENT) continue;           /* skip comments silently */
        if (tk.token == TK_ERROR) {
            /* report lexical error, skip */
            if (strlen(tk.lexeme) == 1)
                printf("Line no %d: Lexical Error: Unknown symbol <%s>\n",
                       tk.lineNo, tk.lexeme);
            else
                printf("Line no %d: Lexical Error: Unknown pattern <%s>\n",
                       tk.lineNo, tk.lexeme);
            continue;
        }
        if (tk.token == TK_LENGTH_ERROR) {
            printf("Line no %d: Lexical Error: Identifier exceeds 20 characters\n",
                   tk.lineNo);
            continue;
        }
        break;
    }
    return tk;
}

ParseTreeNode *parseInputSourceCode(char *testcaseFile, ParseTableType table) {
    FILE *fp = fopen(testcaseFile, "r");
    if (!fp) {
        printf("Error: Cannot open source file '%s'\n", testcaseFile);
        return NULL;
    }

    initializeAcceptStateMap();
    twinBuffer *tb = initializeLexer(fp);

    /* ---- Build parse tree root ---- */
    GrammarSymbol startSym = NT(NT_PROGRAM);
    ParseTreeNode *root    = createParseTreeNode(startSym, NULL);
    root->parent           = NULL;   /* root's parent symbol printed as ROOT */

    /* ---- Initialise stack ---- */
    ParseStack stack;
    stack.top  = NULL;
    stack.size = 0;

    /* Push $ (end marker) then start symbol */
    GrammarSymbol dollarSym = {SYM_TERMINAL, {.terminal = TK_EOF}};
    stackPush(&stack, dollarSym, NULL);
    stackPush(&stack, startSym, root);

    /* ---- Get first token ---- */
    tokenInfo lookahead = nextMeaningfulToken(tb);

    bool syntaxOK = true;

    while (!stackIsEmpty(&stack)) {
        StackElem *topElem = stackPeek(&stack);
        GrammarSymbol top  = topElem->sym;

        /* ------- Top is terminal or $ ------- */
        if (top.type == SYM_TERMINAL) {
            if (top.terminal == lookahead.token) {
                /* Match: fill leaf node info */
                ParseTreeNode *leaf = topElem->treeNode;
                if (leaf) {
                    leaf->isLeaf  = true;
                    leaf->tkInfo  = lookahead;
                    leaf->symbol  = top;
                }
                StackElem *popped = stackPop(&stack);
                free(popped);

                if (lookahead.token != TK_EOF)
                    lookahead = nextMeaningfulToken(tb);
            } else {
                /* Mismatch error */
                if (top.terminal == TK_EOF) {
                    printf("Line no %d: Syntax Error: Extra tokens after end of program\n",
                           lookahead.lineNo);
                } else {
                    printf("Line no %d: Syntax Error: Expected %s but got %s (%s)\n",
                           lookahead.lineNo,
                           tokenNameStr[top.terminal],
                           lookahead.lexeme,
                           (lookahead.token < NUM_TOKENS) ? tokenNameStr[lookahead.token] : "UNKNOWN");
                }
                syntaxOK = false;
                StackElem *popped = stackPop(&stack);
                free(popped);
                /* Simple error recovery: skip the bad token if EOF not reached */
                if (lookahead.token != TK_EOF)
                    lookahead = nextMeaningfulToken(tb);
            }
            continue;
        }

        /* ------- Top is non-terminal ------- */
        NonTerminal A = top.nonTerminal;
        int token     = (int)lookahead.token;

        /* Guard: token must be within table range */
        if (token < 0 || token >= NUM_TOKENS) {
            printf("Line no %d: Syntax Error: Unexpected token '%s'\n",
                   lookahead.lineNo, lookahead.lexeme);
            syntaxOK = false;
            StackElem *popped = stackPop(&stack);
            free(popped);
            if (lookahead.token != TK_EOF)
                lookahead = nextMeaningfulToken(tb);
            continue;
        }

        int ruleIdx = table[A][token];

        if (ruleIdx == -1) {
            /* No rule: syntax error */
            printf("Line no %d: Syntax Error: Unexpected token <%s> while parsing <%s>\n",
                   lookahead.lineNo, lookahead.lexeme, ntNames[A]);
            syntaxOK = false;

            /* Error recovery: pop the NT and try to continue */
            StackElem *popped = stackPop(&stack);
            free(popped);
            /* Advance input to try to re-synchronise */
            if (lookahead.token != TK_EOF)
                lookahead = nextMeaningfulToken(tb);
            continue;
        }

        /* Apply rule: pop NT, push RHS in reverse order */
        Production *prod      = &grammar[ruleIdx];
        ParseTreeNode *ntNode = topElem->treeNode;
        StackElem *popped     = stackPop(&stack);
        free(popped);

        if (prod->rhsLen == 0) {
            /* Epsilon production: create an epsilon leaf child */
            GrammarSymbol epsSym = {SYM_EPSILON, {.terminal = 0}};
            ParseTreeNode *epsNode = createParseTreeNode(epsSym, ntNode);
            epsNode->isLeaf = true;
            if (ntNode) addChild(ntNode, epsNode);
        } else {
            /* Create child nodes in order, push in reverse */
            ParseTreeNode *children[MAX_RHS];
            for (int i = 0; i < prod->rhsLen; i++) {
                children[i] = createParseTreeNode(prod->rhs[i], ntNode);
                if (ntNode) addChild(ntNode, children[i]);
            }
            for (int i = prod->rhsLen - 1; i >= 0; i--)
                stackPush(&stack, prod->rhs[i], children[i]);
        }
    }

    fclose(fp);
    free(tb);

    if (syntaxOK)
        printf("\nInput source code is syntactically correct...........\n");
    else
        printf("\nInput source code has syntax errors (see above).\n");

    stackFree(&stack);
    return root;
}

/* ==========================================================
   PARSE TREE PRINTING  (N-ary inorder traversal to file)
   Inorder for N-ary: leftmost child → parent → remaining siblings
   
   Column format per line:
     lexeme  lineNo  tokenName  valueIfNum  parentSym  isLeaf  nodeSym
   ========================================================== */

/* Returns the string name of a grammar symbol */
static const char *symName(GrammarSymbol sym) {
    if (sym.type == SYM_NON_TERMINAL)
        return ntNames[sym.nonTerminal];
    if (sym.type == SYM_TERMINAL && sym.terminal < NUM_TOKENS)
        return tokenNameStr[sym.terminal];
    return "eps";
}

/* Returns the parent symbol string (ROOT for root node) */
static const char *parentSymName(ParseTreeNode *node) {
    if (!node->parent) return "ROOT";
    return symName(node->parent->symbol);
}

static void printNode(ParseTreeNode *node, FILE *fp) {
    if (!node) return;

    /* lexeme field */
    char lexemeField[MAX_LEXEME_LEN + 4];
    if (node->isLeaf && node->symbol.type == SYM_TERMINAL)
        snprintf(lexemeField, sizeof(lexemeField), "%s", node->tkInfo.lexeme);
    else if (node->symbol.type == SYM_EPSILON)
        snprintf(lexemeField, sizeof(lexemeField), "eps");
    else
        snprintf(lexemeField, sizeof(lexemeField), "----");

    /* line number */
    int lineNo = node->isLeaf ? node->tkInfo.lineNo : 0;

    /* token name */
    const char *tokName = symName(node->symbol);

    /* value if number */
    char valueField[32] = "---";
    if (node->isLeaf && node->symbol.type == SYM_TERMINAL) {
        if (node->tkInfo.token == TK_NUM)
            snprintf(valueField, sizeof(valueField), "%d", node->tkInfo.val.intValue);
        else if (node->tkInfo.token == TK_RNUM)
            snprintf(valueField, sizeof(valueField), "%f", node->tkInfo.val.realValue);
    }

    /* parent symbol */
    const char *parentSym = parentSymName(node);

    /* isLeaf */
    const char *isLeaf = node->isLeaf ? "yes" : "no";

    /* nodeSym (non-terminal symbol for internal nodes) */
    char nodeSymField[64];
    if (!node->isLeaf && node->symbol.type == SYM_NON_TERMINAL)
        snprintf(nodeSymField, sizeof(nodeSymField), "%s", ntNames[node->symbol.nonTerminal]);
    else
        snprintf(nodeSymField, sizeof(nodeSymField), "---");

    fprintf(fp, "%-20s %-6d %-20s %-12s %-25s %-6s %s\n",
            lexemeField, lineNo, tokName, valueField,
            parentSym, isLeaf, nodeSymField);
}

/* N-ary inorder: leftmost child → parent → rest of children */
static void inorderPrint(ParseTreeNode *node, FILE *fp) {
    if (!node) return;

    if (!node->firstChild) {
        /* Leaf node: print directly */
        printNode(node, fp);
        return;
    }

    /* Print leftmost child subtree */
    inorderPrint(node->firstChild, fp);

    /* Print this internal node */
    printNode(node, fp);

    /* Print remaining children (siblings of leftmost child) */
    ParseTreeNode *sib = node->firstChild->nextSibling;
    while (sib) {
        inorderPrint(sib, fp);
        sib = sib->nextSibling;
    }
}

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
    /* Column header */
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
