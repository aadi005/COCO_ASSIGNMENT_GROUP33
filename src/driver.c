#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lexer.h"
#include "stateHandlers.h"

/* Parser will be added later */
//#include "parser.h"   /* Leave this include for later implementation */

/* Define the global matrix declared as extern in lexerDef.h */
int transitionMatrix[MAX_STATES][INPUT_COUNT];

/* ==========================================================
   TRANSITION TABLE LOADER
   Reads transitionTable.xlsx - Sheet1.csv into transitionMatrix
   ========================================================== */

void loadTransitionMatrix(const char *csvFile) {
    FILE *fp = fopen(csvFile, "r");
    if (!fp) {
        printf("Error: Could not open transition table file %s\n", csvFile);
        exit(1);
    }

    char line[1024];
    // Skip the header line
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return;
    }

    int row = 0;
    while (fgets(line, sizeof(line), fp) && row < MAX_STATES) {
        // Use strtok to parse the CSV line
        char *token = strtok(line, ","); // This is the "State" column (index)
        
        int col = 0;
        while ((token = strtok(NULL, ",")) != NULL && col < INPUT_COUNT) {
            // Convert token to integer; handle empty/NaN values as 65 (sink state)
            if (strlen(token) == 0 || strcmp(token, "\n") == 0 || strcmp(token, "\r\n") == 0) {
                transitionMatrix[row + 1][col] = 65; 
            } else {
                transitionMatrix[row + 1][col] = atoi(token);
            }
            col++;
        }
        row++;
    }

    fclose(fp);
    printf("Transition Matrix loaded successfully from %s.\n", csvFile);
}

/* ===========================
   STATUS DISPLAY FUNCTION
   =========================== */

void printImplementationStatus() {
    printf("=============================================\n");
    printf("Compiler Construction Project - Status\n");
    printf("=============================================\n");

    printf("Lexical Analyzer Module: IMPLEMENTED\n");
    printf("FIRST and FOLLOW: Pending / To be implemented\n");
    printf("Predictive Parser: Pending\n");
    printf("Parse Tree Construction: Pending\n");
    printf("Error Recovery: Pending\n");

    printf("=============================================\n\n");
}


/* ===========================
   MAIN DRIVER
   =========================== */

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <sourcecodefile> <parsetreeoutfile>\n", argv[0]);
        return 1;
    }

    char *sourceFile = argv[1];
    char *parseTreeOutFile = argv[2];

    /* Load the Transition Table from your CSV file */
    loadTransitionMatrix("/Users/aadityagoel/Downloads/BITS/4-2/COCO/project/data/transitionTable.xlsx");

    /* Initialize the accept state map (defined in lexer.c) */
    initializeAcceptStateMap();

    int option;

    printImplementationStatus();

    while (1) {
        printf("\n=========== MENU ===========\n");
        printf("0 : Exit\n");
        printf("1 : Remove comments\n");
        printf("2 : Print token list\n");
        printf("3 : Parse and print parse tree\n");
        printf("4 : Print total execution time\n");
        printf("============================\n");

        printf("Enter option: ");
        scanf("%d", &option);

        if (option == 0) {
            printf("Exiting...\n");
            break;
        }

        switch (option) {
            case 1: {
                printf("\nRemoving comments...\n");
                removeComments(sourceFile, "cleaned_output.txt");
                printf("Comment-free code printed to console.\n");
                break;
            }

            case 2: {
                printf("\nPerforming Lexical Analysis...\n");

                FILE *fp = fopen(sourceFile, "r");
                if (!fp) {
                    printf("Error opening source file.\n");
                    break;
                }

                twinBuffer *tb = initializeLexer(fp);
                tokenInfo tk;

                printf("\n%-20s %-20s %-10s\n", "LEXEME", "TOKEN", "LINE NO");
                printf("------------------------------------------------------------\n");

                while (1) {
                    tk = getNextToken(tb);
                    if (tk.token == TK_EOF)
                        break;
                    printToken(tk);
                }

                fclose(fp);
                break;
            }

            case 3: {
                printf("\nParser module not implemented yet.\n");
                break;
            }

            case 4: {
                printf("\nMeasuring execution time (Lexer + Parser)...\n");

                clock_t start_time, end_time;
                double total_CPU_time_in_seconds;

                start_time = clock();

                FILE *fp = fopen(sourceFile, "r");
                if (!fp) {
                    printf("Error opening source file.\n");
                    break;
                }

                twinBuffer *tb = initializeLexer(fp);
                tokenInfo tk;

                while (1) {
                    tk = getNextToken(tb);
                    if (tk.token == TK_EOF)
                        break;
                }
                fclose(fp);

                end_time = clock();
                total_CPU_time_in_seconds = (double)(end_time - start_time) / CLOCKS_PER_SEC;

                printf("Total CPU Time (seconds): %lf\n", total_CPU_time_in_seconds);
                break;
            }

            default:
                printf("Invalid option. Try again.\n");
        }
    }

    return 0;
}