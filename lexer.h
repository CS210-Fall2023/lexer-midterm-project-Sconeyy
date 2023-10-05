// lexer.h

#ifndef LEXER_H
#define LEXER_H

// Define token categories
typedef enum
{
    COMMENT,
    MULTILINE_COMMENT,
    STRING,
    KEYWORD,
    CHAR_LITERAL,
    OPERATOR,
    NUMERIC_LITERAL,
    IDENTIFIER,
    UNK
} TokenType;

// Structure for tokens
typedef struct
{
    TokenType type;
    char *lexeme;
    char *commentText;
} Token;

// Function prototypes
void initLexer(FILE *input);
Token getNextToken();
void cleanupLexer();

#endif