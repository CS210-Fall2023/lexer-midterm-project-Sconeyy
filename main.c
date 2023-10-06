#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL)
    {
        printf("Error opening input file.\n");
        return 1;
    }

    initLexer(inputFile);

    Token token;
    do
    {
        token = getNextToken();
        // Print or process the token here
        if (token.lexeme != NULL)
        {
            switch (token.type)
            {
            case COMMENT:
                printf("%s (%s)\n", token.lexeme, "comment");
                break;
            case MULTILINE_COMMENT:
                printf("%s (%s)\n", token.lexeme, "multi-line comment");
                break;
            case STRING:
                printf("%s (%s)\n", token.lexeme, "string");
                break;
            case KEYWORD:
                printf("%s (%s)\n", token.lexeme, "keyword");
                break;
            case CHAR_LITERAL:
                printf("%s (%s)\n", token.lexeme, "char literal");
                break;
            case OPERATOR:
                printf("%s (%s)\n", token.lexeme, "operator");
                break;
            case NUMERIC_LITERAL:
                printf("%s (%s)\n", token.lexeme, "numeric literal");
                break;
            case IDENTIFIER:
                printf("%s (%s)\n", token.lexeme, "identifier");
                break;
            case UNK:
                printf("%s (%s)\n", token.lexeme, "unknown");
                break;
            default:
                break;
            }

            free(token.lexeme);
        }
    } while (token.type != UNK);

    cleanupLexer();

    return 0;
}