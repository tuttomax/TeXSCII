#include "drawer.hpp"
#include <algorithm>

void drawer::add_glyphes(glyph_container &container, base_glyph *sender)
{
    while (!container.empty())
    {
        auto item = std::move(container.front());
        container.pop();

        add_glyph(item.get(), sender);
    }
}

void drawer::add_glyph(base_glyph *ptr, base_glyph *sender)
{
    fraction *f;
    root *r;
    sub_glyph *sub_g;
    sup_glyph *sup_g;
    sub_sup_glyph *ss_g;
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
    else if (ss_g = dynamic_cast<sub_sup_glyph *>(ptr))
    {
        draw_sub_sup(ss_g);
    }
    else if (g = dynamic_cast<glyph *>(ptr))
    {
        draw_glyph(g, sender);
    }
}

void drawer::show(std::ostream &os)
{
    for (auto &pair : table)
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
    pos += s->data.size();

    level++;
    add_glyphes(s->sub, s);
    level--;
}

void drawer::draw_sup(sup_glyph *s)
{
    table[level].write(s->data);
    pos += s->data.size();

    level--;
    add_glyphes(s->sup,s);
    level++;
}
void drawer::draw_sub_sup(sub_sup_glyph *s)
{

    level--;
    add_glyphes(s->sup,s);
    level++;

    table[level].write(s->data);
    pos += s->data.size();

    level++;
    add_glyphes(s->sub,s);    
    level--;
}
void drawer::draw_glyph(glyph *g, base_glyph *sender)
{
    if (sender)
    {
        if (dynamic_cast<sub_sup_glyph *>(sender))
        {
            
            table[level].write(g->data);
            pos += g->data.size();
        }
        else if (dynamic_cast<sub_glyph *>(sender))
        {
            
            table[level].write(g->data);
            pos += g->data.size();
        }
        else if (dynamic_cast<sup_glyph *>(sender))
        {
            
            table[level].write(g->data);
            pos += g->data.size();
        }
    }
    else
    {
        
        table[level].write(g->data);
        pos += g->data.size();
    }
}