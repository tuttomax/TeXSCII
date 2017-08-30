#include "drawer.hpp"
#include <algorithm>

//UTILITY FUNCTION


template<class Predicate>
static int height(glyph_container &,Predicate p);
static int count_all_glyph(glyph_container &);
static int abs_n(int number);
//SPECIFIC DRAWING FUNCTION


void drawer::add_glyphes(glyph_container &container)
{
    while (!container.empty())
    {
        auto _glyph_ptr = std::move(container.front());
        container.pop_front();
        add_glyph(_glyph_ptr);
    }
}

void drawer::add_glyph(base_glyph_ptr &_glyph_ptr)
{
    base_glyph *ptr = _glyph_ptr.get();

    glyph *g_ptr;
    fracture *f_ptr;
    root *r_ptr;
    sqrt *s_ptr;

    if ((f_ptr = dynamic_cast<fracture *>(ptr)))
    {
        draw_fracture(f_ptr);
    }
    else if ((r_ptr = dynamic_cast<root *>(ptr)))
    {
        draw_root(r_ptr);
    }
    else if ((g_ptr = dynamic_cast<glyph *>(ptr)))
    {
        draw_glyph(g_ptr);
    }
}

void drawer::print(std::ostream &os)
{
    for (auto &pair : table)
    {
        pair.second.print(os);
    }
}

static int count_all_glyph(glyph_container &container)
{
    int count = 0;
    int start_size = container.size();
    glyph *g;
    fracture *f;
    root *r;

    while (start_size > 0)
    {
        auto item = std::move(container.front());
        container.pop_front();
        if (f = dynamic_cast<fracture *>(item.get()))
        {
            int top_l = count_all_glyph(f->top);
            int bottom_l = count_all_glyph(f->bottom);
            count += std::max(top_l, bottom_l);
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            count += count_all_glyph(r->argument);
        }
        else if (g = dynamic_cast<glyph *>(item.get()))
        {
            count += g->data.size();
        }

        container.push_back(std::move(item));
        start_size--;
    }

    return count;
}

static int abs_n(int number)
{
    if (number < 0)
        return -number;
    else
        return number;
}

template<class Predicate>
static int height(glyph_container &container,Predicate p)
{
    int current = 0;
    int start_size = container.size();

    glyph *g;
    fracture *f;
    root *r;

    while (start_size > 0)
    {
        auto item = std::move(container.front());
        container.pop_front();
        if (f = dynamic_cast<fracture *>(item.get()))
        {
            current = p(current, height(f->top,p));
            current -= height(f->bottom,p);
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            current = p(current, height(r->argument,p));
        }
        else if (g = dynamic_cast<glyph *>(item.get()))
        {
            current = p(current, g->level);
        }

        container.push_back(std::move(item));
        start_size--;
    }
    return current;
}

void drawer::draw_fracture(fracture* f)
{
    int top_width = count_all_glyph(f->top);
    int bottom_width = count_all_glyph(f->bottom);
    int top_depth = height(f->top,[](int a,int b){return std::min(a,b); });
    int bottom_depth = height(f->top,[](int a,int b){return std::max(a,b); });
    
    table[f->level].position(pos);
    table[f->level].write(std::string(std::max(top_width,bottom_width),'-'));

    table[f->level - 1].position(pos); 
    while (!f->top.empty() || !f->bottom.empty())
    {
        base_glyph_ptr  top_item;
        base_glyph_ptr bottom_item;

        if (!f->top.empty()) 
        {
            top_item = std::move(f->top.front());
            f->top.pop_front();            
        }         
        if (!f->bottom.empty()) 
        {
            bottom_item = std::move(f->bottom.front());
            f->bottom.pop_front();    
        }

        int temp = pos;        
        add_glyph(top_item);
        pos = temp;
        add_glyph(bottom_item);        
    }
}

void drawer::draw_root(root* r)
{
    int depth = height(r->argument,[](int a,int b){return std::min(a,b); });
    int width = count_all_glyph(r->argument);

    for(int level = r->level; level >= depth - 1; level--)
    {
        if (level == r->level)
        {
            table[level].position(pos);            
            table[level].write('V');
            pos++;
        }
        else if (level == depth - 1)
        {
            table[level].position(pos);
            if (dynamic_cast<sqrt*>(r)) table[level].write(' ');
            else table[level].write(r->power);
            table[level].write(std::string(width,'_'));
        }
        else
        {
            table[level].write('/');
            pos++;
        }
    }

    add_glyphes(r->argument);    
}



void drawer::draw_glyph(glyph* g)
{
    table[g->level].position(pos);
    table[g->level].write(g->data);
    pos += g->data.size();
}