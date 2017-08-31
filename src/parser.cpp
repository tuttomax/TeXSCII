#include "parser.hpp"
#include <sstream>
#include <exception>
#include <cctype>

token_container parser::parse()
{
    token_container output;

    std::istringstream base_stream(expression);
    base_stream >> std::noskipws;
    
    token t;

    char c;

    std::string base_name;
    std::string d = std::string();

    while (base_stream >> c)
    {
        if (c == '(')
        {
            t.type = open_p;
            t.identifier = "(";
        }
        else if (c == ')')
        {
            t.type = close_p;
            t.identifier = ")";
        }
        else if (c == '}')
        {
            t.type = close_b;
            t.identifier = std::string();
        }
        else if (c == '{')
        {
            t.type = open_b;
            t.identifier = std::string();
        }
        else if (c == '\\')
        {
            base_stream >> c;
            while (isalpha(c))
            {
                base_name += c;
                base_stream >> c;

                if (base_stream.fail())
                {
                    t.type = builtinfunc;
                    t.identifier = base_name;
                    output.push(t);
                    return output;
                }
            }

            base_stream.unget();

            t.type = builtinfunc;
            t.identifier = base_name;
            base_name = std::string();
        }
        else if (c == '_')
        {
            t.type = sub;
            t.identifier = std::string();
        }
        else if (c == '^')
        {
            t.type = sup;
            t.identifier = std::string();
        }
        else if (c == '=')
        {
            t.type = symbol;
            t.identifier = "=";
        }
        else if (c == '+')
        {
            t.type = symbol;
            t.identifier = "+";
        }
        else if (c == '-')
        {
            t.type = symbol;
            t.identifier = "-";
        }
        else if (c == '*')
        {
            t.type = symbol;
            t.identifier = "*";
        }
        else if (c == '/')
        {
            t.type = symbol;
            t.identifier = "/";
        }
        else if (c == ' ')
        {
            t.type = symbol;
            t.identifier = " ";
        }
        else if (isdigit(c))
        {

            while (isdigit(c))
            {
                d += c;
                base_stream >> c;

                if (base_stream.fail())
                {
                    base_stream.clear();
                    t.type = number;
                    t.identifier = d;
                    output.push(t);
                    return output;
                }
            }
            base_stream.unget();

            t.type = number;
            t.identifier = d;

            d = std::string();
        }
        else if (isalpha(c))
        {

            while (isalpha(c))
            {
                base_name += c;
                base_stream >> c;
                if (base_stream.fail())
                {
                    base_stream.clear();
                    t.type = general_string;
                    t.identifier = base_name;
                    output.push(t);
                    return output;
                }
            }
            base_stream.unget();

            t.type = general_string;
            t.identifier = base_name;
            base_name = std::string();
        }

        output.push(t);
    }
    return output;
}
