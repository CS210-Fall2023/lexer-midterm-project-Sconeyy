#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static FILE *inputFile;
static char currentChar;
static char buffer[16384]; // Buffer for lexemes
static int bufferIndex;

// Define keywords in CCX
static const char *keywords[] = {
    "accessor", "and", "array", "begin", "bool", "case", "character", "constant",
    "else", "elsif", "end", "exit", "function", "if", "in", "integer", "interface",
    "is", "loop", "module", "mutator", "natural", "null", "of", "or", "other",
    "out", "positive", "procedure", "range", "return", "struct", "subtype", "then",
    "type", "when", "while", NULL};

// static const char *operators[] = {
//     ":=", "..", "<<", ">>", "<>", "<=", ">=", "**", "!=", "=>", ".", "<", ">", "(", ")",
//     "+", "-", "*", "/", "|", "&", ";", ",", ":", "[", "]", "=", NULL};

void initLexer(FILE *input)
{
    inputFile = input;
    currentChar = fgetc(inputFile);
    bufferIndex = 0;
}

static void addToBuffer(char c)
{
    if (bufferIndex < sizeof(buffer) - 1)
    {
        buffer[bufferIndex++] = c;
    }
}

static Token createToken(TokenType type, const char *commentText)
{
    Token token;
    token.type = type;
    token.lexeme = strdup(buffer);
    token.commentText = strdup(commentText);
    memset(buffer, 0, sizeof(buffer));
    bufferIndex = 0;
    return token;
}

Token getNextToken()
{
    Token token;
    token.lexeme = NULL;

    // Skip whitespace and handle comments
    while (currentChar == ' ' || currentChar == '\t' || currentChar == '\n' || currentChar == '\r')
    {
        currentChar = fgetc(inputFile);
    }

    if (currentChar == '/')
    {
        // Check for comments
        char nextChar = fgetc(inputFile);
        if (nextChar == '/')
        {
            // Single-line comment
            char commentText[16384];
            int commentIndex = 0;
            while (currentChar != '\n' && currentChar != EOF)
            {
                commentText[commentIndex++] = currentChar;
                currentChar = fgetc(inputFile);
            }
            token = createToken(COMMENT, commentText);
        }
        else if (nextChar == '*')
        {
            // Multiline comment
            char commentText[16384];
            int commentIndex = 0;
            commentText[commentIndex++] = '/';
            commentText[commentIndex++] = '*';
            while (1)
            {
                currentChar = fgetc(inputFile);
                if (currentChar == '*')
                {
                    char nextNextChar = fgetc(inputFile);
                    if (nextNextChar == '/')
                    {
                        commentText[commentIndex++] = '*';
                        commentText[commentIndex++] = '/';
                        currentChar = fgetc(inputFile); // Consume '/' of '*/'
                        token = createToken(MULTILINE_COMMENT, commentText);
                        break;
                    }
                }
                if (currentChar == EOF)
                {
                    // Handle unclosed comment
                    token = createToken(UNK, commentText);
                    break;
                }
                commentText[commentIndex++] = currentChar;
            }
        }
        else
        {
            // Operator '/'
            addToBuffer('/');
            token = createToken(OPERATOR, NULL);
        }
    }
    else if (currentChar == '"')
    {
        // String
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        while (currentChar != '"' && currentChar != EOF)
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
        }
        if (currentChar == '"')
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
            token = createToken(STRING, NULL);
        }
        else
        {
            // Handle unclosed string
            token = createToken(UNK, NULL);
        }
    }
    else if (isalpha(currentChar) || currentChar == '_')
    {
        // Keyword or Identifier
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        while (isalnum(currentChar) || currentChar == '_')
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
        }
        // Check if the lexeme is a keyword
        for (int i = 0; keywords[i] != NULL; i++)
        {
            if (strcmp(keywords[i], buffer) == 0)
            {
                token = createToken(KEYWORD, NULL);
                break;
            }
        }
        // If it's not a keyword, it's an identifier
        if (token.lexeme == NULL)
        {
            token = createToken(IDENTIFIER, NULL);
        }
    }
    else if (isdigit(currentChar))
    {
        // Numeric Literal
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        while (isdigit(currentChar) || currentChar == '.' || currentChar == '#')
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
        }
        token = createToken(NUMERIC_LITERAL, NULL);
    }
    else if (strchr(".()<>=+-*/|&;,:[],", currentChar) != NULL)
    {
        // Operator
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        while (strchr(".()<>=+-*/|&;,:[],", currentChar) != NULL)
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
        }
        token = createToken(OPERATOR, NULL);
    }
    else if (currentChar == '\'')
    {
        // Character Literal
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        while (currentChar != '\'' && currentChar != EOF)
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
        }
        if (currentChar == '\'')
        {
            addToBuffer(currentChar);
            currentChar = fgetc(inputFile);
            token = createToken(CHAR_LITERAL, NULL);
        }
        else
        {
            // Handle unclosed character literal
            token = createToken(UNK, NULL);
        }
    }
    else
    {
        // Unknown token
        addToBuffer(currentChar);
        currentChar = fgetc(inputFile);
        token = createToken(UNK, NULL);
    }

    return token;
}

void cleanupLexer()
{
    fclose(inputFile);
}