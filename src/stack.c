/*
 * stack.c
 * CS F363 - Compiler Construction, BITS Pilani, Jan 2026
 * Stack ADT used by the predictive (LL(1)) parser.
 * Each stack element carries a grammar symbol and the
 * corresponding parse-tree node pointer.
 */

#include <stdlib.h>
#include <stdio.h>
#include "parserDef.h"
#include "parser.h"

/* ----------------------------------------------------------
   stackPush
   Allocates a new element and pushes it on top of the stack.
   ---------------------------------------------------------- */
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

/* ----------------------------------------------------------
   stackPop
   Removes and returns the top element.  Caller is responsible
   for freeing the returned pointer after use.
   Returns NULL on empty stack.
   ---------------------------------------------------------- */
StackElem *stackPop(ParseStack *s) {
    if (!s->top) return NULL;
    StackElem *elem = s->top;
    s->top = s->top->next;
    s->size--;
    return elem;
}

/* ----------------------------------------------------------
   stackPeek
   Returns the top element without removing it.
   Returns NULL on empty stack.
   ---------------------------------------------------------- */
StackElem *stackPeek(ParseStack *s) {
    return s->top;
}

/* ----------------------------------------------------------
   stackIsEmpty
   Returns 1 if the stack is empty, 0 otherwise.
   ---------------------------------------------------------- */
int stackIsEmpty(ParseStack *s) {
    return (s->top == NULL);
}

/* ----------------------------------------------------------
   stackFree
   Frees all elements remaining on the stack.
   ---------------------------------------------------------- */
void stackFree(ParseStack *s) {
    while (s->top) {
        StackElem *tmp = s->top;
        s->top = s->top->next;
        free(tmp);
    }
    s->size = 0;
}
