#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include "glyph.hpp"
#include "token.hpp"

class interpreter
{
public:
    explicit interpreter(std::ostream& output_stream) : os(output_stream) { }
    glyph_container run(token_container& container);
    
    


private:
    std::ostream& os;

    void add_glyph(glyph_container& container,const token& t);
};

#endif //INTERPRETER_HPP