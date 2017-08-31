#include "drawer.hpp"
#include <algorithm>

//UTILITY FUNCTION

static int abs_n(int number);

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

void drawer::draw_fracture(fracture *f)
{
   
}

void drawer::draw_root(root *r)
{
   int w = width(r->argument);
   int h = heiht(r->argument);
   
}

void drawer::draw_glyph(glyph *g)
{
    table[g->level].position(pos);
    table[g->level].write(g->data);
    pos += g->data.size();
}

static int abs_n(int number)
{
    if (number < 0)
        return -number;
    else
        return number;
}

int drawer::height(glyph_container &container)
{
    int h = 0;
    int n = container.size();
    
    fracture* f;
    root* r;
    glyph* g;
    
    while (n > 0)
    {
        auto item = std::move(container.front());
        container.pop_front();

        if (f = dynamic_cast<fracture*>(item.get()))
        {
            h = std::max(h,height(f->top)+height(f->bottom)+1); //add row for char '-'            
        }
        else if (r = dynamic_cast<root*>(item.get()))
        {
            h = std::max(h,height(r->argument)+1);  //add row for char '_'
        }
        else if (g = dynamic_cast<glyph*>(item.get()))
        {
            h = std::max(h,abs_n(g->level));
        }
        container.push_back(std::move(item));
    }
    return h;
}

int drawer::width(glyph_container& container)
{
    int w = 0;
    int n = container.size();

    fracture* f;
    root* r;
    glyph* g;
    
    while (n > 0)
    {
        auto item = std::move(container.front());
        container.pop_front();

        if (f = dynamic_cast<fracture*>(item.get()))
        {
            w += std::max(width(f->top),width(f->bottom));
        }
        else if (r = dynamic_cast<root*>(item.get()))
        {
            w += width(r->argument) + height(r->argument); //add width for char '/' and 'V';
        }
        else if (g = dynamic_cast<glyph*>(item.get()))
        {
            w += g->data.size();
        }
        container.push_back(std::move(item));
    }
    return w;
}