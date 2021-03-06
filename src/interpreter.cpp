#include "interpreter.hpp"

#include <exception>
#include <stack>
#include <iostream>

glyph_container interpreter::run(token_container &tokens)
{
    glyph_container container;

    while (!tokens.empty())
    {
        auto current_token = tokens.front();
        tokens.pop();
        token next_token = empty_token;
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

            next_token = tokens.front();

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
            else
            {
                add_glyph(container,token{close_p,")"});
            }
        }
        else if (current_token.type == builtinfunc)
        {
            if (current_token.identifier == "frac")
            {
                auto ptr = std::make_unique<fraction>();

                std::stack<token> backtrack_bracket;
                token_container backtrack;

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

                ptr->top = run(backtrack);

                do
                {
                    current_token = tokens.front();
                    tokens.pop();

                    if (current_token.type == open_b)
                        backtrack_bracket.push(current_token);
                    if (current_token.type == close_b)
                        backtrack_bracket.pop();

                    backtrack.push(current_token);

                } while (current_token.type != close_b || !backtrack_bracket.empty());

                ptr->bottom = run(backtrack);

                container.push(std::move(ptr));
            }
            else if (current_token.identifier == "root")
            {
                auto ptr = std::make_unique<root>();
                std::stack<token> backtrack_bracket;
                token_container backtrack;

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

                auto internal_container = run(backtrack);
                if (internal_container.size() > 1)
                    throw std::exception();
                auto item = std::move(internal_container.front());
                internal_container.pop();
                auto power_ptr = dynamic_cast<glyph *>(item.get());
                ptr->power = std::stoi(power_ptr->data);

                do
                {
                    current_token = tokens.front();
                    tokens.pop();

                    if (current_token.type == open_b)
                        backtrack_bracket.push(current_token);
                    if (current_token.type == close_b)
                        backtrack_bracket.pop();

                    backtrack.push(current_token);

                } while (current_token.type != close_b || !backtrack_bracket.empty());

                ptr->argument = run(backtrack);

                container.push(std::move(ptr));
            }
            else if (current_token.identifier == "sqrt")
            {
                auto ptr = std::make_unique<_sqrt>();
                std::stack<token> backtrack_bracket;
                token_container backtrack;

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

                ptr->argument = run(backtrack);

                container.push(std::move(ptr));
            }
            else if (current_token.identifier == "pi")
            {
                add_glyph(container, current_token);
            }
        }
        else if (current_token.type == general_string || //here handle also sup '^' and sub '^'
                 current_token.type == number)
        {
            std::string id = current_token.identifier;
            
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
            
                glyph_container sup_c, sub_c;

                tokens.pop();

                token_container backtrack;
                std::stack<token> backtrack_bracket;

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

                sub_c = run(backtrack);

                next_token = tokens.front();

                if (next_token.type == sup) //handle _{sub}^{sup}
                {

                    tokens.pop();

                    token_container backtrack;
                    std::stack<token> backtrack_bracket;

                    do
                    {
                        current_token = tokens.front();
                        tokens.pop();

                        if (current_token.type == open_b)
                            backtrack_bracket.push(current_token);
                        if (current_token.type == close_b)
                            backtrack_bracket.pop();

                        backtrack.push(current_token);
                    } while (current_token.type != close_b || !backtrack_bracket.empty());

                    sup_c = run(backtrack);

                    auto ptr = std::make_unique<sub_sup_glyph>(id);
                    
                    ptr->sub = std::move(sub_c);
                    ptr->sup = std::move(sup_c);

                    container.push(std::move(ptr));
                }
                else
                {
                    auto ptr = std::make_unique<sub_glyph>(id);
                    
                    ptr->sub = std::move(sub_c);
                    container.push(std::move(ptr));
                }
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