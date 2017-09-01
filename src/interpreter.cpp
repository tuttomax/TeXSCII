#include "interpreter.hpp"

#include <stack>

glyph_container interpreter::run(token_container &tokens)
{
    glyph_container container;

    token_container previous_backtrack;

    while (!tokens.empty())
    {
        auto current_token = tokens.front();
        tokens.pop();
        token next_token;
        if (!tokens.empty())
            next_token = tokens.front();

        if (current_token.type == open_p)
        {
            token_container backtrack;
            std::stack<token> backtrack_bracket;

            add_glyph(container, current_token);

            while (current_token.type != close_p || !backtrack_bracket.empty())
            {
                current_token = tokens.front();
                tokens.pop();

                if (current_token.type == open_b)
                    backtrack_bracket.push(current_token);
                if (current_token.type == close_b)
                    backtrack_bracket.pop();

                backtrack.push(current_token);
            }

            auto internal_container = run(backtrack);

            while (!internal_container.empty())
            {
                auto item = std::move(internal_container.front());
                internal_container.pop();
                container.push(std::move(item));
            }
        }
        else if (current_token.type == general_string ||
                 current_token.type == number)
        {
            if (next_token.type == sup)
            {
                tokens.pop();

                token_container backtrack;
                std::stack<token> backtrack_bracket;

                auto ptr = std::make_unique<sup_glyph>(current_token.identifier);

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

                ptr->sup = run(backtrack);

                container.push(std::move(ptr));
            }
            else if (next_token.type == sub)
            {
                tokens.pop();

                token_container backtrack;
                std::stack<token> backtrack_bracket;

                auto ptr = std::make_unique<sub_glyph>(current_token.identifier);

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

                ptr->sub = run(backtrack);

                container.push(std::move(ptr));
            }
            else
            {
                add_glyph(container, current_token);
            }
        }
        else if (current_token.type == symbol)
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