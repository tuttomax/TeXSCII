#include "interpreter.hpp"
#include <stack>
#include <memory>
#include <stdexcept>
#include <iostream>

static void add_glyph(glyph_container &, const token &, const int);
static int abs_n(const int);


glyph_container interpreter::run(std::deque<token> &tokens, int &current_level)
{
    std::stack<int> backtrack_level;
    std::stack<token> backtrack_bracket;
    glyph_container container;
    
    token current;

    while (!tokens.empty())
    {
        current = tokens.front();
        tokens.pop_front();

        if (current.type == open_p) 
        {
            std::stack<token> backtrack_bracket;
            std::deque<token> backtrack;

            add_glyph(container, current, current_level);

            while (current.type != close_p || !backtrack_bracket.empty())
            {
                current = tokens.front();
                tokens.pop_front();

                if (current.type == open_b)
                    backtrack_bracket.push(current);
                if (current.type == close_b)
                    backtrack_bracket.pop();

                backtrack.push_back(current);
            }

            //delete token close_p from backstack
            current = backtrack.back(); 
            backtrack.pop_back(); 
            //return token close_p in tokens
            tokens.push_front(current);
            

            if (!backtrack_bracket.empty())
                throw std::runtime_error("");

            auto internal_container = run(backtrack, current_level);
            while (!internal_container.empty())
            {
                auto ptr = std::move(internal_container.front());
                internal_container.pop_front();
                container.push_back(std::move(ptr));
            }
        }
        else if (current.type == builtinfunc)
        {
            if (current.identifier == "frac")
            {
                auto f = std::make_unique<fracture>(current_level);

                std::stack<token> backtrack_bracket;
                std::deque<token> backtrack;

                while (current.type != close_b || !backtrack_bracket.empty())
                {
                    current = tokens.front();
                    tokens.pop_front();
                    if (current.type == sub) current_level--; //in order to insert extra space 
                    if (current.type == open_b) backtrack_bracket.push(current);
                    if (current.type == close_b) backtrack_bracket.pop();
                    
                    backtrack.push_back(current);
                }
                
                backtrack_level.push(current_level--);
                f->top = run(backtrack,current_level);
                current_level = backtrack_level.top();
                backtrack_level.pop();
                
                do
                {
                    current = tokens.front();
                    tokens.pop_front();

                    if (current.type == sup) current_level++; //in order to insert extra space
                    if (current.type == open_b) backtrack_bracket.push(current);
                    if (current.type == close_b) backtrack_bracket.pop();
                    
                    backtrack.push_back(current);

                } while (current.type != close_b || !backtrack_bracket.empty());

                backtrack_level.push(current_level++);
                f->bottom = run(backtrack,current_level);
                current_level = backtrack_level.top();
                backtrack_level.pop();

                container.push_back(std::move(f));
            }
            else if (current.identifier == "root")
            {
                auto r = std::make_unique<root>(current_level);
                std::stack<token> backtrack_bracket;
                std::deque<token> backtrack;

                while (current.type != close_b || !backtrack_bracket.empty())
                {
                    current = tokens.front();
                    tokens.pop_front();

                    if (current.type == open_b) backtrack_bracket.push(current);
                    if (current.type == close_b) backtrack_bracket.pop();

                    backtrack.push_back(current);
                }

                auto internal_container = run(backtrack,current_level);
                if (internal_container.size() != 1) throw std::runtime_error("root: internal_container invalid size");
                auto p = std::move(internal_container.back());
                internal_container.pop_back();
                int power = std::stoi(dynamic_cast<glyph*>(p.get())->data);
                r->power = power;

                do
                {
                    current = tokens.front();
                    tokens.pop_front();

                  
                    if (current.type == sup) current_level++;
                    if (current.type == sub) current_level--;
                    if (current.type == open_b) backtrack_bracket.push(current);
                    if (current.type == close_b) backtrack_bracket.pop();

                    backtrack.push_back(current);

                } while (current.type != close_b || !backtrack_bracket.empty());
                
                r->argument = run(backtrack,current_level);                

                container.push_back(std::move(r));
            }
            else if (current.identifier == "sqrt")
            {
                auto s = std::make_unique<sqrt>(current_level);
                std::stack<token> backtrack_bracket;
                std::deque<token> backtrack;

                while (current.type != close_b || !backtrack_bracket.empty())
                {
                    current = tokens.front();
                    tokens.pop_front();

                    
                    if (current.type == sup) current_level++;
                    if (current.type == sub) current_level--;
                    if (current.type == open_b) backtrack_bracket.push(current);
                    if (current.type == close_b) backtrack_bracket.pop();

                    backtrack.push_back(current);
                }

                s->argument = run(backtrack,current_level);

                container.push_back(std::move(s));
            }
            else if (current.identifier == "pi")
            {
                add_glyph(container, current, current_level);
            }
        }
        else if (current.type == sup)
        {
            backtrack_level.push(current_level--);
        }
        else if (current.type == sub)
        {
           backtrack_level.push(current_level++);
        }
        else if (current.type == close_p)
        {
            add_glyph(container,current,current_level);
        }
        else if (current.type == open_b)
        {
            backtrack_bracket.push(current);
        }
        else if (current.type == close_b)
        {
            backtrack_bracket.pop();
            if (!backtrack_level.empty())
            {
                current_level = backtrack_level.top();
                backtrack_level.pop();
            }            
        }
        else if (current.type == general_string ||
                 current.type == number ||
                 current.type == symbol)
        {
            add_glyph(container, current, current_level);
        }
    }

    return container;
}

static void add_glyph(glyph_container &container, const token &t, const int level)
{
    auto ptr = std::make_unique<glyph>(level, t.identifier);
    container.push_back(std::move(ptr));
}


static int abs_n(const int i)
{
    return i < 0 ? -i : i;
}

