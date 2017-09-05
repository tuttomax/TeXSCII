#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <queue>
#include <string>


enum token_type
{
    

    general_string,
    symbol,
    number,

    open_p,
    close_p,
    open_b,
    close_b,
    
    sup,
    sub,

    builtinfunc
};

struct token
{
    token_type type;
    std::string identifier;
};

using token_container = std::queue<token>;
static token empty_token = token();
#endif //TOKEN_HPP