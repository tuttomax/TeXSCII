#include "drawer.hpp"

#include <iomanip>

void drawer::add_glyphes(glyph_container &container)
{
    while (!container.empty())
    {
        auto item = std::move(container.front());
        container.pop();

        add_glyph(item.get());
    }
}

void drawer::add_glyph(base_glyph *ptr,base_glyph* sender)
{
    fraction *f;
    root *r;
    sub_glyph *sub_g;
    sup_glyph *sup_g;
    sub_sup_glyph* ss_g;
    glyph *g;

    if (f = dynamic_cast<fraction *>(ptr))
    {
        draw_fraction(f);
    }
    else if (r = dynamic_cast<root *>(ptr)) //manage both root and sqrt
    {
        draw_root(r);
    }
    else if (sub_g = dynamic_cast<sub_glyph *>(ptr))
    {
        draw_sub(sub_g);
    }
    else if (sup_g = dynamic_cast<sup_glyph *>(ptr))
    {
        draw_sup(sup_g);
    }
    else if (ss_g = dynamic_cast<sub_sup_glyph*>(ptr))
    {
        draw_sub_sup(ss_g);
    }
    else if (g = dynamic_cast<glyph *>(ptr))
    {
        draw_glyph(g,sender);
    }
}

void drawer::show(std::ostream &os)
{
    for (auto& pair : table)
    {
        os << pair.second.text() << std::endl;
    }
}

void drawer::draw_fraction(fraction *f)
{
}

void drawer::draw_root(root *r)
{
}

void drawer::draw_sub(sub_glyph *s)
{
    table[level].write(s->data);
    level++;
    while(!s->sub.empty())
    {
        auto item = std::move(s->sub.front());
        s->sub.pop();
        add_glyph(item.get());                
    }
    level--;    
}

void drawer::draw_sup(sup_glyph *s)
{
    level--;
    while(!s->sup.empty())
    {
        auto item = std::move(s->sup.front());
        s->sup.pop();
        add_glyph(item.get());                
    }
    level++;
    table[level].write(s->data);
}
void drawer::draw_sub_sup(sub_sup_glyph* s)
{
    level--;
    while(!s->sup.empty())
    {
        auto item = std::move(s->sup.front());
        s->sup.pop();
        add_glyph(item.get(),s);                
    }
    level++;
    table[level].write(s->data);
    level++;
    while(!s->sub.empty())
    {
        auto item = std::move(s->sub.front());
        s->sub.pop();
        add_glyph(item.get(),s);        
    }
    level--;
}
void drawer::draw_glyph(glyph *g,base_glyph* sender)
{
    if (sender)
    {
        if (dynamic_cast<sub_sup_glyph*>(sender))
        {
            table[level].fill_if(pos,' ');
            table[level].write(g->data);
        }
    }
    else 
    {
        table[level].fill_if(pos,' ');
        table[level].write(g->data);
        pos += g->data.size();    
    }
}