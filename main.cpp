#include "parser.hpp"
#include <iostream>
#include "interpreter.hpp"


int main(int length, char **args)
{
    try
    {
        parser p(args[1]);
        auto q = p.parse();
        interpreter i(std::cout);
        i.run(q);
    }

    catch (std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}