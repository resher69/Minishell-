#include "utils.h"

int get_quote(char c)
{
    static int quote = QUOTE_NONE;

    if (c == '\'' && quote != QUOTE_DOUBLE)
    {
        if (quote == QUOTE_SINGLE)
            quote = QUOTE_NONE;
        else 
            quote = QUOTE_SINGLE;
    }
    else if (c == '\"' && quote != QUOTE_SINGLE)
    {
        if (quote == QUOTE_DOUBLE)
            quote = QUOTE_NONE;
        else 
            quote = QUOTE_DOUBLE;
    }
    return (quote);
}