#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <deque>
#include "token.hpp"

class parser{

public:
    explicit parser(std::string equation) : expression(equation) { /**/ }
    std::deque<token> parse();
      
private:
    std::string expression;

   
};

#endif //PARSER_HPP