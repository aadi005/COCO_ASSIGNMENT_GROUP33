

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include "lexerDef.h"
#include <stdio.h>



void initGrammar(void);



FirstAndFollow computeFirstAndFollowSets(void);


void printFirstAndFollow(FirstAndFollow *ff);



void createParseTable(FirstAndFollow *ff, ParseTableType table);


void printParseTable(ParseTableType table);



void freeParseTree(ParseTreeNode *root);



ParseTreeNode *parseInputSourceCode(char *testcaseFile,
                                    ParseTableType table);



void printParseTree(ParseTreeNode *root, char *outfile);


void       stackPush   (ParseStack *s, GrammarSymbol sym, ParseTreeNode *node);
StackElem *stackPop    (ParseStack *s);
StackElem *stackPeek   (ParseStack *s);
int        stackIsEmpty(ParseStack *s);
void       stackFree   (ParseStack *s);


ParseTreeNode *createParseTreeNode(GrammarSymbol sym, ParseTreeNode *parent);
void           addChild(ParseTreeNode *parent, ParseTreeNode *child);

#endif
