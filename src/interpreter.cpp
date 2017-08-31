#include "interpreter.hpp"

glyph_container interpreter::run(token_container &tokens)
{
    glyph_container container;
    
    token_container previous_backtrack;

    while (!tokens.empty())
    {
        previous_backtrack.push(tokens.top());
        tokens.pop();
        
        token next_token = tokens.top();
        tokens.pop();
        
        if (next_token.type == sup)
        {

        }    

    }

    return container;
}

void interpreter::add_glyph(glyph_container &container, const token &t)
{
    auto item = std::make_unique<glyph>(t.identifier);
    container.push(std::move(item));
}