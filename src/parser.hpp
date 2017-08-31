#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <stack>
#include "token.hpp"

class parser{

public:
    explicit parser(std::string equation) : expression(equation) { /**/ }
    token_container parse();
      
private:
    std::string expression;

   
};

#endif //PARSER_HPP