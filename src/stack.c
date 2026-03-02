

#include <stdlib.h>
#include <stdio.h>
#include "parserDef.h"
#include "parser.h"


void stackPush(ParseStack *s, GrammarSymbol sym, ParseTreeNode *node) {
    StackElem *elem = (StackElem *)malloc(sizeof(StackElem));
    if (!elem) {
        fprintf(stderr, "Fatal: stack memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    elem->sym      = sym;
    elem->treeNode = node;
    elem->next     = s->top;
    s->top         = elem;
    s->size++;
}


StackElem *stackPop(ParseStack *s) {
    if (!s->top) return NULL;
    StackElem *elem = s->top;
    s->top = s->top->next;
    s->size--;
    return elem;
}


StackElem *stackPeek(ParseStack *s) {
    return s->top;
}


int stackIsEmpty(ParseStack *s) {
    return (s->top == NULL);
}


void stackFree(ParseStack *s) {
    while (s->top) {
        StackElem *tmp = s->top;
        s->top = s->top->next;
        free(tmp);
    }
    s->size = 0;
}
