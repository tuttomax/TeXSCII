#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

enum token_type 
{
    general_string,       // any name: function,variable
    open_b,         // {
    close_b,        // }
    open_p,         // ( 
    close_p,        // )
    number,         // any digit
    symbol,         // handle +,-,*,/,^,=
    sub,            // handle ^ for writing on top
    sup,            // handle _ for writing on bottom    
   
    builtinfunc,

};

struct token
{
    token_type type;
    std::string identifier; // will be used by interpreter
};

#endif //TOKEN_HPp