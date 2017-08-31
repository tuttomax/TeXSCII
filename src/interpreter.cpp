#include "interpreter.hpp"

glyph_container interpreter::run(token_container &tokens)
{
    glyph_container container;
    
    token_container previous_backtrack;

    while (!tokens.empty())
    {
        
    }

    return container;
}

void interpreter::add_glyph(glyph_container &container, const token &t)
{
    auto item = std::make_unique<glyph>(t.identifier);
    container.push(std::move(item));
}