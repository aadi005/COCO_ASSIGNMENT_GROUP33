/*
 * driver.c
 * CS F363 - Compiler Construction, BITS Pilani, Jan 2026
 *
 * Drives the full compiler pipeline.
 * Usage: ./stage1exe <sourcefile> <parsetreeoutfile>
 *
 * Menu options:
 *   0 : Exit
 *   1 : Remove comments - print comment-free code to console
 *   2 : Print token list (lexer only, line number wise)
 *   3 : Parse source code, print errors and parse tree to file
 *   4 : Print total CPU time taken by lexer + parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lexer.h"
#include "lexerDef.h"
#include "stateHandlers.h"
#include "parser.h"
#include "parserDef.h"

/* ==========================================================
   IMPLEMENTATION STATUS DISPLAY
   ========================================================== */
void printImplementationStatus(void) {
    printf("=====================================================\n");
    printf("  CS F363 Compiler Project - BITS Pilani Jan 2026  \n");
    printf("=====================================================\n");
    printf("  Status: Both Lexical and Syntax Analysis modules  \n");
    printf("          implemented.                               \n");
    printf("  FIRST and FOLLOW sets: Automated.                 \n");
    printf("  Predictive parse table: Constructed.              \n");
    printf("  Parse tree construction: Implemented.             \n");
    printf("=====================================================\n\n");
}

/* ==========================================================
   MAIN DRIVER
   ========================================================== */
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }

    char *sourceFile      = argv[1];
    char *parseTreeFile   = argv[2];

    /* ---------- One-time initialisations ---------- */
    initializeAcceptStateMap();   /* build DFA accept state map */
    initGrammar();                /* populate grammar rules array */
    FF = computeFirstAndFollowSets();     /* automated FIRST/FOLLOW */
    createParseTable(&FF, parseTable);    /* build LL(1) parse table */

    printImplementationStatus();

    int option;
    while (1) {
        printf("\n=========== MENU ===========\n");
        printf("0 : Exit\n");
        printf("1 : Remove comments (print comment-free code)\n");
        printf("2 : Print token list (lexer only)\n");
        printf("3 : Parse and print parse tree\n");
        printf("4 : Print total execution time\n");
        printf("============================\n");
        printf("Enter option: ");

        if (scanf("%d", &option) != 1) break;

        if (option == 0) {
            printf("Exiting...\n");
            break;
        }

        switch (option) {

        /* -------------------------------------------------- */
        case 1: {
            printf("\n--- Comment-free source code ---\n");
            removeComments(sourceFile, "cleaned_output.txt");
            /* Print the cleaned file to console */
            FILE *cf = fopen("cleaned_output.txt", "r");
            if (!cf) { printf("Error opening cleaned file.\n"); break; }
            int ch;
            while ((ch = fgetc(cf)) != EOF) putchar(ch);
            fclose(cf);
            break;
        }

        /* -------------------------------------------------- */
        case 2: {
            printf("\n--- Lexical Analysis Token List ---\n");
            FILE *fp = fopen(sourceFile, "r");
            if (!fp) { printf("Error opening source file.\n"); break; }

            twinBuffer *tb = initializeLexer(fp);
            tokenInfo   tk;

            printf("\n%-10s  %-25s  %s\n", "Line No.", "Lexeme", "Token Name");
            printf("%-10s  %-25s  %s\n",
                   "--------", "------", "----------");

            while (1) {
                tk = getNextToken(tb);
                if (tk.token == TK_EOF)   break;
                if (tk.token == TK_COMMENT) continue;
                printToken(tk);
            }

            fclose(fp);
            free(tb);
            break;
        }

        /* -------------------------------------------------- */
        case 3: {
            printf("\n--- Parsing Source Code ---\n");
            ParseTreeNode *tree =
                parseInputSourceCode(sourceFile, parseTable);
            if (tree) {
                printParseTree(tree, parseTreeFile);
            }
            break;
        }

        /* -------------------------------------------------- */
        case 4: {
            printf("\n--- Measuring Execution Time (Lexer + Parser) ---\n");

            clock_t start_time, end_time;
            double  total_CPU_time, total_CPU_time_in_seconds;

            start_time = clock();

            /* Run lexer */
            FILE *fp = fopen(sourceFile, "r");
            if (!fp) { printf("Error opening source file.\n"); break; }
            twinBuffer *tb = initializeLexer(fp);
            tokenInfo   tk;
            while (1) {
                tk = getNextToken(tb);
                if (tk.token == TK_EOF) break;
            }
            fclose(fp);
            free(tb);

            /* Run parser */
            ParseTreeNode *tree =
                parseInputSourceCode(sourceFile, parseTable);
            (void)tree;   /* result used for timing only */

            end_time = clock();
            total_CPU_time            = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("Total CPU Time (clock ticks) : %.0f\n", total_CPU_time);
            printf("Total CPU Time (seconds)     : %lf\n",
                   total_CPU_time_in_seconds);
            break;
        }

        /* -------------------------------------------------- */
        default:
            printf("Invalid option. Please enter 0-4.\n");
        }
    }

    return 0;
}
