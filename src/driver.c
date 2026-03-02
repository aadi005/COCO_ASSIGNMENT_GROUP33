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
#include <time.h>

#include "lexer.h"
#include "lexerDef.h"
#include "stateHandlers.h"
#include "parser.h"
#include "parserDef.h"

// Prints static project status banner.
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

int main(int argc, char *argv[]) {

    // Expect input source and parse-tree output path.
    if (argc != 3) {
        printf("Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }

    // CLI file arguments.
    char *sourceFile      = argv[1];
    char *parseTreeFile   = argv[2];

    // one-time setup before menu loop
    initializeAcceptStateMap();
    initGrammar();
    FF = computeFirstAndFollowSets();
    createParseTable(&FF, parseTable);

    printImplementationStatus();

    int option; // menu choice
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

        case 1: {
            printf("\n--- Comment-free source code ---\n");
            removeComments(sourceFile, "cleaned_output.txt");

            // Print cleaned file on terminal.
            FILE *cf = fopen("cleaned_output.txt", "r");
            if (!cf) { printf("Error opening cleaned file.\n"); break; }
            int ch;
            while ((ch = fgetc(cf)) != EOF) putchar(ch);
            fclose(cf);
            break;
        }

        case 2: {
            printf("\n--- Lexical Analysis Token List ---\n");
            FILE *fp = fopen(sourceFile, "r");
            if (!fp) { printf("Error opening source file.\n"); break; }
            FILE *lexerTokenFile = fopen("lexer_tokens.txt", "w");
            FILE *lexerErrorFile = fopen("lexer_errors.txt", "w");
            if (!lexerTokenFile || !lexerErrorFile) {
                printf("Error opening lexer output files.\n");
                if (lexerTokenFile) fclose(lexerTokenFile);
                if (lexerErrorFile) fclose(lexerErrorFile);
                fclose(fp);
                break;
            }
            setLexerOutputFiles(lexerTokenFile, lexerErrorFile);

            // Run lexer until EOF.
            twinBuffer *tb = initializeLexer(fp);
            tokenInfo   tk;

            printf("\n%-10s  %-25s  %s\n", "Line No.", "Lexeme", "Token Name");
            printf("%-10s  %-25s  %s\n",
                   "--------", "------", "----------");
            fprintf(lexerTokenFile, "\n%-10s  %-25s  %s\n", "Line No.", "Lexeme", "Token Name");
            fprintf(lexerTokenFile, "%-10s  %-25s  %s\n",
                    "--------", "------", "----------");

            while (1) {
                tk = getNextToken(tb);
                if (tk.token == TK_EOF)   break;
                printToken(tk);
            }

            setLexerOutputFiles(NULL, NULL);
            fclose(lexerTokenFile);
            fclose(lexerErrorFile);
            fclose(fp);
            free(tb);
            break;
        }

        case 3: {
            printf("\n--- Parsing Source Code ---\n");
            FILE *parserErrorFile = fopen("parser_errors.txt", "w");
            if (!parserErrorFile) { printf("Error opening parser error file.\n"); break; }
            setParserErrorFile(parserErrorFile);
            // Build and dump parse tree.
            ParseTreeNode *tree =
                parseInputSourceCode(sourceFile, parseTable);
            if (tree) {
                printParseTree(tree, parseTreeFile);
                freeParseTree(tree);
            }
            setParserErrorFile(NULL);
            fclose(parserErrorFile);
            break;
        }

        case 4: {
            printf("\n--- Measuring Execution Time (Lexer + Parser) ---\n");

            // Timing variables.
            clock_t start_time, end_time;
            double  total_CPU_time, total_CPU_time_in_seconds;

            start_time = clock(); // start timing

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

            FILE *parserErrorFile = fopen("parser_errors.txt", "w");
            if (!parserErrorFile) { printf("Error opening parser error file.\n"); break; }
            setParserErrorFile(parserErrorFile);
            ParseTreeNode *tree =
                parseInputSourceCode(sourceFile, parseTable);
            setParserErrorFile(NULL);
            fclose(parserErrorFile);

            end_time = clock(); // stop timing
            total_CPU_time            = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            printf("Total CPU Time (clock ticks) : %.0f\n", total_CPU_time);
            printf("Total CPU Time (seconds)     : %lf\n",
                   total_CPU_time_in_seconds);

            if (tree) freeParseTree(tree);
            break;
        }

        default:
            printf("Invalid option. Please enter 0-4.\n");
        }
    }

    return 0;
}
