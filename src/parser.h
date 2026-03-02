

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include "lexerDef.h"
#include <stdio.h>

// Fill `grammar` with all productions.
void initGrammar(void);

// Compute FIRST and FOLLOW sets.
FirstAndFollow computeFirstAndFollowSets(void);

// Print FIRST/FOLLOW sets to stdout.
void printFirstAndFollow(FirstAndFollow *ff);

// Build LL(1) parse table from FIRST/FOLLOW.
void createParseTable(FirstAndFollow *ff, ParseTableType table);

// Print non-empty parse table cells.
void printParseTable(ParseTableType table);

// Free full parse tree recursively.
void freeParseTree(ParseTreeNode *root);

// Parse source file and return parse-tree root.
ParseTreeNode *parseInputSourceCode(char *testcaseFile,
                                    ParseTableType table);

// Write parse tree to output file.
void printParseTree(ParseTreeNode *root, char *outfile);

// Push one symbol node pair on parse stack.
void       stackPush   (ParseStack *s, GrammarSymbol sym, ParseTreeNode *node);
// Pop and return top stack element.
StackElem *stackPop    (ParseStack *s);
// Peek top stack element.
StackElem *stackPeek   (ParseStack *s);
// True when stack has no elements.
int        stackIsEmpty(ParseStack *s);
// Free all stack elements.
void       stackFree   (ParseStack *s);

// Allocate a parse-tree node.
ParseTreeNode *createParseTreeNode(GrammarSymbol sym, ParseTreeNode *parent);
// Attach child as last child of parent.
void           addChild(ParseTreeNode *parent, ParseTreeNode *child);

#endif
