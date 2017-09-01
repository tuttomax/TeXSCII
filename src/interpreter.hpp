#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <ostream>
#include "glyph.hpp"
#include "token.hpp"
#include "drawer.hpp"

class interpreter
{
public:
    explicit interpreter(std::ostream& output_stream) : os(output_stream) { }
    
    void execute(token_container& container)
    {
        auto items = run(container);
        blackboard.show(os,items);
    }


private:
    std::ostream& os;
    drawer blackboard;
    
    glyph_container run(token_container& container);
    void add_glyph(glyph_container& container,const token& t);
};

#endif //INTERPRETER_HPP