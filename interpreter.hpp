#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <deque>
#include <queue>

#include "token.hpp"
#include "glyph.hpp"
#include "drawer.hpp"

class interpreter
{
  public:
    explicit interpreter(std::ostream& output_stream) : os(output_stream) { }

    glyph_container run(std::deque<token> &, int &);

    void run(std::deque<token> &tokens)
    {
        int current_level = 0;
        glyph_container expression = run(tokens, current_level);
        d.add_glyphes(expression);
        d.print(os);       
    }
private:
    drawer d;
    std::ostream& os;
};
#endif //INTERPRETER_HPP