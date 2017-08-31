#include "interpreter.hpp"

glyph_container interpreter::run(token_container &tokens)
{
    glyph_container container;

    while (!tokens.empty())
    {
        token current_token = tokens.front();
        tokens.pop();
        token next_token = tokens.front();

        if (next_token.type == sup)
        {
            token.pop();
            
            std::stack<token> backtrack_bracket;
            token_container backtrack;

            auto sup_ptr = std::make_unique<sup_glyph>(current_token.identifier);

            while (current_token.type != close_b || !backtrack_bracket.empty())
            {
                current_token = tokens.front();
                tokens.pop();

                if (current_token.type == open_b)
                    backtrack_bracket.push(current_token);
                if (current_token.type == close_b)
                    backtrack_bracket.pop();

                backtrack.push(current_token);
            }

            sup_ptr->sup = run(backtrack);

            container.push(std::move(sup_ptr));
        }
        if (current_token.type == sub)
        {
            std::stack<token> backtrack_bracket;
            token_container backtrack;

            auto sub_ptr = std::make_unique<sub_glyph>();

            while (current_token.type != close_b || !backtrack_bracket.empty())
            {
                current_token = tokens.front();
                tokens.pop();

                if (current_token.type == open_b)
                    backtrack_bracket.push(current_token);
                if (current_token.type == close_b)
                    backtrack_bracket.pop();

                backtrack.push(current_token);
            }

            sub_ptr->sub = run(backtrack);

            container.push(std::move(sub_ptr));
        }
        else if (current_token.type == general_string ||
                 current_token.type == number ||
                 current_token.type == symbol)
        {
            add_glyph(container, current_token);
        }
    }

    return container;
}

void interpreter::add_glyph(glyph_container &container, const token &t)
{
    auto item = std::make_unique<glyph>(t.identifier);
    container.push(std::move(item));
}