#include "drawer.hpp"

void drawer::add_glyphes(glyph_container &container)
{
    while (!container.empty())
    {
        auto item = std::move(container.front());
        container.pop();

        add_glyph(item.get());
    }
}

void drawer::add_glyph(base_glyph *ptr)
{
    fraction* f;
    root* r;
    sub_glyph* sub_g;
    sup_glyph* sup_g;
    glyph* g;

    if (f = dynamic_cast<fraction*>(ptr))
    {
        draw_fraction(f);
    }
    else if (r = dynamic_cast<root*>(ptr)) //manage both root and sqrt
    {
        draw_root(r);
    }
    else if (sub_g = dynamic_cast<sub_glyph*>(ptr))
    {
        draw_sub(sub_g);
    }
    else if (sup_g = dynamic_cast<sup_glyph*>(ptr))
    {
        draw_sup(sup_g);
    }
    else if (g = dynamic_cast<glyph*>(ptr))
    {
        draw_glyph(g);
    }
}

void drawer::show(std::ostream &os)
{
    

}

#include <iostream>

void drawer::draw_fraction(fraction* f)
{
    std::clog << "fraction" << width(f) << "x" << height(f) << std::endl;
}

void drawer::draw_root(root* r)
{
    std::clog << "root" << width(r) << "x" << height(r) << std::endl;    
}

void drawer::draw_sub(sub_glyph* s)
{
    std::clog << "sub" << width(s) << "x" << height(s) << std::endl;    
}

void drawer::draw_sup(sup_glyph* s)
{
    std::clog << "sup" << width(s) << "x" << height(s) << std::endl;    
}

void drawer::draw_glyph(glyph* g)
{
    std::clog << "glyph" << width(g) << "x" << height(g) << std::endl;    
}