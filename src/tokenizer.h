#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string.h>

enum EToken
{
    TOK_COMMENT,
    TOK_EXIT
};

const char *token_to_string(EToken token)
{

    switch (token)
    {
    case TOK_COMMENT:"#" break;
    case TOK_EXIT:"exit" break;

    default:
        break;
    }
}

#endif // TOKENIZER_H