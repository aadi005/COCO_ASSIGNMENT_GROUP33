/*
 * parser.h
 * CS F363 - Compiler Construction, BITS Pilani, Jan 2026
 * Function prototype declarations for parser.c
 */

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include "lexerDef.h"
#include <stdio.h>

/* -------------------------------------------------------
   Grammar Initialisation
   ------------------------------------------------------- */
/* Populate the global grammar[] array with all 95 rules. */
void initGrammar(void);

/* -------------------------------------------------------
   FIRST and FOLLOW Computation (Automated)
   ------------------------------------------------------- */
/*
 * Iteratively computes FIRST and FOLLOW sets for every
 * non-terminal in the grammar and returns the result in
 * a FirstAndFollow structure.
 */
FirstAndFollow computeFirstAndFollowSets(void);

/* Print FIRST and FOLLOW sets to stdout (for verification). */
void printFirstAndFollow(FirstAndFollow *ff);

/* -------------------------------------------------------
   Predictive Parse Table
   ------------------------------------------------------- */
/*
 * Uses the FIRST/FOLLOW information in *ff to populate
 * the predictive parse table.  Conflicts are reported
 * to stderr.
 */
void createParseTable(FirstAndFollow *ff, ParseTableType table);

/* Print all non-empty entries in the parse table. */
void printParseTable(ParseTableType table);

/* -------------------------------------------------------
   Parsing
   ------------------------------------------------------- */
/*
 * Reads tokens via the lexer from testcaseFile, drives a
 * stack-based LL(1) parser using table, builds a parse tree,
 * and returns a pointer to its root.  Lexical and syntactic
 * errors are printed to stdout with line numbers.
 * Returns NULL if the file cannot be opened.
 */
ParseTreeNode *parseInputSourceCode(char *testcaseFile,
                                    ParseTableType table);

/* -------------------------------------------------------
   Parse Tree Printing
   ------------------------------------------------------- */
/*
 * Prints the parse tree in n-ary inorder traversal to the
 * file named outfile.  Format (one node per line):
 *   lexeme  lineNo  tokenName  valueIfNum  parentSym  isLeaf  nodeSym
 */
void printParseTree(ParseTreeNode *root, char *outfile);

/* -------------------------------------------------------
   Stack Operations  (implementations in stack.c)
   ------------------------------------------------------- */
void       stackPush   (ParseStack *s, GrammarSymbol sym, ParseTreeNode *node);
StackElem *stackPop    (ParseStack *s);
StackElem *stackPeek   (ParseStack *s);
int        stackIsEmpty(ParseStack *s);
void       stackFree   (ParseStack *s);

/* -------------------------------------------------------
   Parse Tree Node Helpers
   ------------------------------------------------------- */
ParseTreeNode *createParseTreeNode(GrammarSymbol sym, ParseTreeNode *parent);
void           addChild(ParseTreeNode *parent, ParseTreeNode *child);

#endif /* PARSER_H */
