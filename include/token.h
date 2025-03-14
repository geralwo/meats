#ifndef TOKEN_H
#define TOKEN_H
#include <stdint.h>
#include <stdlib.h>
// Token type enumeration
typedef enum
{
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_BINARY_OPERATOR,
    TOKEN_EQUAL,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LT,
    TOKEN_GT,
    TOKEN_HASH,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_SEMICOLON,
    TOKEN_SYMBOL,
    TOKEN_EOL,
    TOKEN_EOF,
    TOKEN_ANY,
} TokenType;

// Token structure
typedef struct
{
    TokenType Type;
    char *Value;
    size_t line;
    size_t column;
    int Precedence;
    uint64_t Metadata;
} Token;

static inline Token new_token(TokenType type, size_t line_number)
{
    Token token;
    token.Type = type;
    token.Value = NULL;
    token.line = line_number;
    token.column = -1;
    token.Metadata = 0;
    token.Precedence = 0;
    return token;
}

static inline const char *tokenType_name(TokenType type)
{
    switch (type)
    {
    case TOKEN_IDENTIFIER:
        return "IDENTIFIER";
    case TOKEN_NUMBER:
        return "NUMBER";
    case TOKEN_STRING:
        return "STRING";
    case TOKEN_OPERATOR:
        return "OPERATOR";
    case TOKEN_BINARY_OPERATOR:
        return "BINARY OPERATOR";
    case TOKEN_EQUAL:
        return "EQUAL";
    case TOKEN_LT:
        return "LESS THAN";
    case TOKEN_GT:
        return "GREATER THAN";
    case TOKEN_EOL:
        return "EOL";
    case TOKEN_EOF:
        return "EOF";
    case TOKEN_ANY:
        return "ANY";
    case TOKEN_RBRACE:
        return "RBRACE";
    case TOKEN_LBRACE:
        return "LBRACE";
    case TOKEN_LPAREN:
        return "LPAREN";
    case TOKEN_RPAREN:
        return "RPAREN";
    case TOKEN_COLON:
        return "COLON";
    case TOKEN_COMMA:
        return "COMMA";
    case TOKEN_SEMICOLON:
        return "SEMICOLON";
    case TOKEN_SYMBOL:
        return "SYMBOL";
    default:
        return "UNKNOWN";
    }
}

void free_token(void *token);

#endif // TOKEN_H
