const int MAX_STATES = 64;

typedef enum {
    DIGIT,          // 0-9
    DIGIT1,         // 2-7 
    ALPHA,          // a-b 
    ALPHA1,         // b-d
    wALPHA2,         // e-z 
    ALPHABET,       // a-z, A-Z 
    DELIM,          // Space/Blank 
    NEWLINE,        // \n 
    EOF_TYPE,       // End of file 
    LESS_THAN,      // <
    GREATER_THAN,   // > 
    ZERO,           // 0
    MINUS,          // -
    DOT,            // . 
    SEMICOLON,      // ; 
    COMMA,          // , 
    AT_SYMBOL,      // @ 
    HASH,           // # 
    PERCENT,        // % 
    EQUAL,          // =
    FORWARD_SLASH,  // / 
    ASTERISK,       // * 
    EXCLAMATION,    // ! 
    AMPERSAND,      // &
    EPSILON,        // epsilon 
    CLOSE_PAREN,    // )
    COLON,          // : 
    OPEN_PAREN,     // (
    UNDERSCORE,     // _ 
    INPUT_COUNT     // Helper to track size
} InputType;

extern int transitionMatrix[MAX_STATES][INPUT_COUNT];