#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "lexerDef.h"
#include "stateHandlers.h"
#include <ctype.h>   // for isalnum, isalpha, etc

/* Definition of the global accept state map */
StateHandler acceptStateMap[MAX_STATES] = { NULL };

/* Helper to map characters to the transition matrix columns based on CSV header */
InputType mapCharToEnum(int ch) {
    // Treat both the EOF constant and any embedded nulls as end-of-file input
    if (ch == EOF || ch == '\0') return EOF_TYPE;
    if (ch == ' ' || ch == '\t' || ch == '\r') return DELIM;
    if (ch == '\n') return NEWLINE;
    
    // Priority: digit1 (2-7) must be checked before general digit (0-9)
    if (ch >= '2' && ch <= '7') return DIGIT1;
    if (ch >= '0' && ch <= '9') return DIGIT;
    
    // Character classes for identifiers as defined in regex
    
    if (ch=='b' || ch == 'c' || ch == 'd') return ALPHA1;
    if (ch >= 'a' && ch <= 'z' && (ch!='b' && ch != 'c' && ch != 'd')) return ALPHA2;
    if (ch >= 'a' && ch <= 'z') return ALPHA;
    
    if ((ch >= 'A' && ch <= 'Z')) return ALPHABET;

    switch (ch) {
        case '<': return LESS_THAN;
        case '>': return GREATER_THAN;
        case '=': return EQUAL;
        case '-': return MINUS;
        case '.': return DOT;
        case ';': return SEMICOLON;
        case ',': return COMMA;
        case '@': return AT_SYMBOL;
        case '#': return HASH;
        case '%': return PERCENT;
        case '+': return PLUS;
        case '/': return FORWARD_SLASH;
        case '*': return ASTERISK;
        case '!': return EXCLAMATION;
        case ':': return COLON;
        case '(': return OPEN_PAREN;
        case ')': return CLOSE_PAREN;
        case '[': return L_SQR;
        case ']': return R_SQR;
        case '_': return UNDERSCORE;
        case '&': return AMPERSAND;
        default: return INPUT_COUNT; 
    }
}

twinBuffer* initializeLexer(FILE *fp) {
    twinBuffer *tb = (twinBuffer*)malloc(sizeof(twinBuffer));
    tb->fp = fp;
    tb->forward = 0;
    tb->lexemeBegin = 0;
    tb->currentBuffer = 1;
    
    // Initial fill of buffer1
    memset(tb->buffer1, 0, BUFFER_SIZE);
    fread(tb->buffer1, 1, BUFFER_SIZE, tb->fp);
    return tb;
}

void refillBuffer(twinBuffer *tb) {
    if (tb->currentBuffer == 1) {
        memset(tb->buffer2, 0, BUFFER_SIZE);
        fread(tb->buffer2, 1, BUFFER_SIZE, tb->fp);
        tb->currentBuffer = 2;
    } else {
        memset(tb->buffer1, 0, BUFFER_SIZE);
        fread(tb->buffer1, 1, BUFFER_SIZE, tb->fp);
        tb->currentBuffer = 1;
    }
}

int getNextChar(twinBuffer *tb) {
    int ch;
    if (tb->currentBuffer == 1) {
        ch = (unsigned char)tb->buffer1[tb->forward];
    } else {
        ch = (unsigned char)tb->buffer2[tb->forward];
    }

    tb->forward++;
    if (tb->forward == BUFFER_SIZE) {
        refillBuffer(tb);
        tb->forward = 0;
    }
    /* If we've read past the end of actual input the buffer will contain
       zeroes (we memset when we refill). Treat a null character as EOF so
       that the DFA can terminate cleanly. */
    if (ch == '\0')
        return EOF;
    return ch;
}

void retract(twinBuffer *tb) {
    tb->forward--;
    if (tb->forward < 0) {
        tb->forward = BUFFER_SIZE - 1;
        tb->currentBuffer = (tb->currentBuffer == 1) ? 2 : 1;
    }
}

tokenInfo getNextToken(twinBuffer *tb) {
    int currentState = 1;
    int nextState;
    int ch;                    // now holds EOF sentinel as an int
    tokenInfo tk;
    int lp = 0;
    static int lineNo = 1;

    // Reset lexeme and token structure
    memset(tk.lexeme, 0, MAX_LEXEME_LEN);
    tk.lineNo = lineNo;

    while (1) {
        ch = getNextChar(tb);

        /* --- EOF HANDLING ---
           Instead of returning immediately when EOF is encountered we defer the
           decision.  This allows a token that ended exactly at end-of-file to be
           flushed out before the EOF token itself is generated.  The variable
           "lp" tracks how many characters have been accumulated in the current
           lexeme.  If we are mid‑token we break the loop and handle the lexeme
           after the loop; otherwise we can return the EOF token right away. */
        if (ch == EOF) {
            if (lp > 0) {
                /* there is a partial lexeme waiting – break so it can be
                   finalised below */
                break;
            }
            tk.token = TK_EOF;
            strcpy(tk.lexeme, "EOF");
            tk.lineNo = lineNo;
            return tk;
        }

        /* --- DIRECT BRACKET HANDLING ---
           The DFA defined in the transition matrix did not previously support
           square brackets because the InputType enum lacked corresponding
           entries.  For simplicity we treat them as single-character tokens
           here before invoking the main DFA. */
        if (ch == '[') {
            tk.token = TK_SQL;
            strcpy(tk.lexeme, "[");
            tk.lineNo = lineNo;
            return tk;
        }
        if (ch == ']') {
            tk.token = TK_SQR;
            strcpy(tk.lexeme, "]");
            tk.lineNo = lineNo;
            return tk;
        }

        /* --- UNDERSCORE / FUNID HANDLING ---
           Function identifiers and other names start with an underscore.  Instead
           of letting the DFA drop them into a sink state, handle them explicitly.
           Collect characters until a non-alphanumeric/underscore is seen. */
        if (ch == '_') {
            int lp2 = 0;
            tk.lexeme[lp2++] = '_';
            tk.lexeme[lp2] = '\0';
            while (1) {
                int nxt = getNextChar(tb);
                if (nxt == EOF) break;
                if (isalnum(nxt) || nxt == '_') {
                    if (lp2 < MAX_LEXEME_LEN - 1) {
                        tk.lexeme[lp2++] = nxt;
                        tk.lexeme[lp2] = '\0';
                    }
                    continue;
                }
                /* push the non-matching character back so it can be processed by
                   the normal DFA on the next call */
                retract(tb);
                break;
            }
            tk.lineNo = lineNo;
            /* special case: ``_main'' is reserved */
            if (strcmp(tk.lexeme, "_main") == 0) {
                tk.token = TK_MAIN;
            } else {
                tk.token = TK_FUNID;
            }
            return tk;
        }

        /* --- COMMENT HANDLING ---
           A percent sign begins a comment; the entire rest of the line is ignored
           but a TK_COMMENT token with lexeme "%" is returned so that the parser
           (or test harness) can log its presence.  Subsequent characters up to and
           including the newline are consumed here. */
        if (ch == '%') {
            tk.token = TK_COMMENT;
            strcpy(tk.lexeme, "%");
            tk.lineNo = lineNo;
            /* eat until end of line or file */
            while ((ch = getNextChar(tb)) != EOF && ch != '\n');
            if (ch == '\n') lineNo++;
            return tk;
        }

        InputType input = mapCharToEnum(ch);
        /* guard against unmapped characters returning INPUT_COUNT which would
           index past the end of the transition table.  Unrecognised symbols
           are considered lexical errors and produce a one‑character error
           token. */

        if (currentState!=1 && (input==ALPHA1 || input==ALPHA2) ){
            input = ALPHA;
        }
        if (input == INPUT_COUNT) {
            tk.token = TK_ERROR;
            tk.lexeme[0] = ch;
            tk.lexeme[1] = '\0';
            tk.lineNo = lineNo;
            return tk;
        }

        nextState = transitionMatrix[currentState][input];

        /* --- SINK STATE / ERROR HANDLING --- */
        // If the DFA hits state 65, it's a lexical error. 
        // We stop here to prevent an infinite loop.
        if (nextState == 65) {
            tk.token = TK_ERROR;
            if (lp < MAX_LEXEME_LEN - 1) {
                tk.lexeme[lp++] = ch;
                tk.lexeme[lp] = '\0';
            }
            tk.lineNo = lineNo;
            return tk;
        }

        /* --- ERRATA HANDLING --- */

        // 1. Lexical Errata: <- should be reported as error
        if (currentState == 2 && ch == '-') {
            tk.token = TK_ERROR;
            strcpy(tk.lexeme, "<-");
            tk.lineNo = lineNo;
            return tk;
        }

        // 2. Lexical Errata: 23.abc handling (Split TK_NUM, TK_DOT, TK_FIELDID)
        // If we are in the integer state (52) and see a DOT, we accept the integer.
        if (currentState == 52 && input == DOT) {
            retract(tb); // Put the DOT back for the next getNextToken call
            tk.token = TK_NUM;
            tk.lexeme[lp] = '\0';
            tk.lineNo = lineNo;
            tk.val.intValue = atoi(tk.lexeme); 
            return tk;
        }

        /* --- ACCEPT STATE LOGIC --- */
        // If the next state is a registered final state in our map
        if (acceptStateMap[nextState] != NULL) {
            // Append the final character to the lexeme before finishing
            if (lp < MAX_LEXEME_LEN - 1) {
                tk.lexeme[lp++] = ch;
                tk.lexeme[lp] = '\0';
            }
            /* capture the current line number before invoking handler; the
               handlers themselves do not set the line number, so we overwrite
               it on whatever token they return. */
            tk.lineNo = lineNo;
            tokenInfo result = acceptStateMap[nextState](tk.lexeme, nextState);
            result.lineNo = lineNo;
            return result;
        }

        /* --- DELIMITER / NEWLINE HANDLING ---
           When in state 1, skip all leading whitespace (delimiters & newlines).
           When NOT in state 1 and a delimiter appears, treat it as end-of-token
           marker *only if* we're in an accept state or the DFA would reject it anyway. */
        if (currentState == 1 && (input == DELIM || input == NEWLINE)) {
            // Skip leading whitespace in state 1
            if (input == NEWLINE) lineNo++;
            tb->lexemeBegin = tb->forward;
            continue;
        }

        /* --- STATE TRANSITION LOGIC --- */

        // Update line count on newlines
        if (ch == '\n') lineNo++;

        // Append character to lexeme
        if (lp < MAX_LEXEME_LEN - 1) {
            tk.lexeme[lp++] = ch;
            tk.lexeme[lp] = '\0';
        }

        currentState = nextState;
    }

    /* --- EOF CLEANUP ---
       we exited the loop because ch was EOF while lp>0.  The lexeme buffer
       contains the characters that have been read so far; finalise a token
       based on the current DFA state.  If the state is accepting we simply
       invoke its handler, otherwise report a lexical error. */
    if (lp > 0) {
        if (acceptStateMap[currentState] != NULL) {
            tokenInfo result = acceptStateMap[currentState](tk.lexeme, currentState);
            result.lineNo = lineNo;
            return result;
        } else {
            tk.token = TK_ERROR;
            tk.lexeme[lp] = '\0';
            tk.lineNo = lineNo;
            return tk;
        }
    }

    /* no lexeme pending – fall through and return EOF token (should not
       ordinarily happen because it would have been returned earlier) */
    tk.token = TK_EOF;
    strcpy(tk.lexeme, "EOF");
    tk.lineNo = lineNo;
    return tk;
}
void initializeAcceptStateMap() {
    // Initialization of all entries to NULL is already handled by { NULL }
    
    // --- Operator Accept States ---
    acceptStateMap[5]  = handle_TK_ASSIGNOP;  // <---
    acceptStateMap[15] = handle_TK_PLUS;      // +
    acceptStateMap[16] = handle_TK_MINUS;     // -
    acceptStateMap[17] = handle_TK_MUL;       // *
    acceptStateMap[18] = handle_TK_DIV;       // /
    acceptStateMap[37] = handle_TK_AND;       // &&&

    // --- Identifier and Keyword Accept States ---
    acceptStateMap[10] = handle_TK_FUNID;     // Functions/Main
    acceptStateMap[14] = handle_TK_ID;        // ID using digit1 (2-7)
    acceptStateMap[48] = handle_TK_ID;        // Generic Identifier

    // --- Numeric Accept States ---
    acceptStateMap[61] = handle_TK_NUM;       // Integer
    acceptStateMap[62] = handle_TK_RNUM;      // Real Number

    // --- Delimiter Accept States ---
    acceptStateMap[22] = handle_TK_SEM;       // ;
    acceptStateMap[21] = handle_TK_COMMA;     // ,
    acceptStateMap[20] = handle_TK_SQL;       // [
    acceptStateMap[19] = handle_TK_SQR;       // ]
    acceptStateMap[26] = handle_TK_OP;        // (
    acceptStateMap[25] = handle_TK_CL;        // )
    acceptStateMap[23] = handle_TK_COLON;     // :
    acceptStateMap[24] = handle_TK_DOT;       // .

    // --- Special Accept States ---
    acceptStateMap[44] = handle_TK_EOF;       // End of file
    
    /* Note: State 64 handles comment removal. It typically does not 
       return a token but resets the DFA to State 1.
    */
}

void removeComments(char *inputFile, char *outputFile) {
    // Basic implementation: read from inputFile, skip text between % and \n, write to outputFile
    FILE *src = fopen(inputFile, "r");
    FILE *dest = fopen(outputFile, "w");
    if (!src || !dest) return;

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        if (ch == '%') {
            while ((ch = fgetc(src)) != EOF && ch != '\n');
            if (ch == '\n') fputc('\n', dest);
        } else {
            fputc(ch, dest);
        }
    }
    fclose(src);
    fclose(dest);
}

void printToken(tokenInfo tk) {
    /* A full string table matching the TokenName enum order.  If you add new
       tokens to lexerDef.h make sure to update this array accordingly. */
    static const char *tokenNames[] = {
        "TK_COMMENT",          /* added for "%" */
        "TK_WITH", "TK_PARAMETERS", "TK_END", "TK_WHILE", "TK_UNION",
        "TK_ENDUNION", "TK_DEFINETYPE", "TK_AS", "TK_TYPE", "TK_MAIN",
        "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_INPUT", "TK_OUTPUT",
        "TK_INT", "TK_REAL", "TK_ENDWHILE", "TK_IF", "TK_THEN",
        "TK_ENDIF", "TK_READ", "TK_WRITE", "TK_RETURN", "TK_CALL",
        "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_ASSIGNOP", "TK_PLUS",
        "TK_MINUS", "TK_MUL", "TK_DIV", "TK_AND", "TK_OR", "TK_NOT",
        "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE",
        "TK_SQL", "TK_SQR", "TK_OP", "TK_CL", "TK_COMMA", "TK_SEM",
        "TK_COLON", "TK_DOT", "TK_ID", "TK_FUNID", "TK_FIELDID",
        "TK_RUID", "TK_NUM", "TK_RNUM", "TK_EOF", "TK_ERROR"
    };

    const char *name = "UNKNOWN";
    if (tk.token >= 0 && tk.token < (int)(sizeof(tokenNames)/sizeof(tokenNames[0]))) {
        name = tokenNames[tk.token];
    }

    printf("%-20s %-20s %-10d\n", tk.lexeme, name, tk.lineNo);
}

TokenName checkKeyword(char *lexeme) {
    if (strcmp(lexeme, "with") == 0) return TK_WITH;
    if (strcmp(lexeme, "parameters") == 0) return TK_PARAMETERS;
    if (strcmp(lexeme, "end") == 0) return TK_END;
    if (strcmp(lexeme, "while") == 0) return TK_WHILE;
    if (strcmp(lexeme, "input") == 0) return TK_INPUT;
    if (strcmp(lexeme, "parameter") == 0) return TK_PARAMETER;
    if (strcmp(lexeme, "list") == 0) return TK_LIST;
    if (strcmp(lexeme, "type") == 0) return TK_TYPE;
    if (strcmp(lexeme, "int") == 0) return TK_INT;
    if (strcmp(lexeme, "real") == 0) return TK_REAL;
    if (strcmp(lexeme, "output") == 0) return TK_OUTPUT;
    if (strcmp(lexeme, "return") == 0) return TK_RETURN;
    if (strcmp(lexeme, "read") == 0) return TK_READ;
    if (strcmp(lexeme, "write") == 0) return TK_WRITE;
    if (strcmp(lexeme, "call") == 0) return TK_CALL;
    if (strcmp(lexeme, "endwhile") == 0) return TK_ENDWHILE;
    return TK_ID; // Default if not a keyword
}