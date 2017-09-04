#include "drawer.hpp"
#include <algorithm>

static int width(glyph_container &);

static int height(glyph_container &);

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
    else if (r = dynamic_cast<root *>(ptr)) 
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
    
    int w = std::max(width(f->top),width(f->bottom));
    int h_t = height(f->top);
    int h_b = height(f->bottom);


    int internal_pos = pos;

    
    level -= h_t;
    table[level].position(internal_pos + w/2 - width(f->top)/2);   
    add_glyphes(f->top,f);
    level += h_t;

    table[level].position(internal_pos);    
    table[level].write(std::string(w,'-'));

    level += h_b;
    table[level].position(internal_pos + w/2 - width(f->bottom)/2);     
    add_glyphes(f->bottom,f);
    level -= h_b;

    pos += w;
}

void drawer::draw_root(root *r)
{
    table[level].position(pos);

    int h = height(r->argument);
    int w = width(r->argument);
    int total_h = h + 1;
    int total_w = w + h;

    int internal_level = level;
    int internal_pos = pos;

    for (int i = 0; i <= h; i++)
    {
        table[internal_level].position(internal_pos);
        if (i == 0)
        {
            table[internal_level--].write('V');
            internal_pos++;
        }
        else if (i == h)
        {
            table[internal_level--].write(std::string(w, '_'));         
        }
        else
        {
            table[internal_level--].write('/');
            internal_pos++;
        }
    }

    pos = internal_pos;

    add_glyphes(r->argument, r);

    pos += total_w;
}

void drawer::draw_sub(sub_glyph *s)
{
    table[level].position(pos);
    table[level].write(s->data);
    pos = std::max(pos, table[level].position());

    int h = height(s->sub);
    level += h;
    add_glyphes(s->sub, s);
    level -= h;
}

void drawer::draw_sup(sup_glyph *s)
{
    table[level].position(pos);
    table[level].write(s->data);
    pos = std::max(pos, table[level].position());

    int h = height(s->sup);

    level -= h;
    add_glyphes(s->sup, s);
    level += h;
}
void drawer::draw_sub_sup(sub_sup_glyph *s)
{
    table[level].write(s->data);
    pos = std::max<int>(pos, s->data.size());

    int h_t = height(s->sup);
    int h_b = height(s->sub);
    int w_t = width(s->sup);
    int w_b = width(s->sub);

    level -= h_t;
    add_glyphes(s->sup, s);
    level += h_t;

    level += h_b;
    add_glyphes(s->sub, s);
    level -= h_b;

    pos += std::max(w_t, w_b);
}
void drawer::draw_glyph(glyph *g, base_glyph *sender)
{
    
    if (sender)
    {
        if (dynamic_cast<fraction *>(sender))
        {            
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());
        }
        else if (dynamic_cast<root *>(sender))
        {
            table[level].position(pos); 
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());
        }
        else if (dynamic_cast<sub_sup_glyph *>(sender))
        {
            table[level].position(pos); 
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());
        }
        else if (dynamic_cast<sub_glyph *>(sender))
        {
            table[level].position(pos);            
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());
        }
        else if (dynamic_cast<sup_glyph *>(sender))
        {
            table[level].position(pos);            
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());
        }
    }
    else
    {
        table[level].position(pos);        
        table[level].write(g->data);
        pos = std::max(pos, table[level].position());
    }
}

static int width(glyph_container &container)
{
    int w = 0;

    int start = container.size();

    fraction *f;
    root *r;
    sub_glyph *sub_g;
    sup_glyph *sup_g;
    sub_sup_glyph *ss_g;
    glyph *g;

    while (start > 0)
    {
        auto item = std::move(container.front());
        container.pop();

        if (f = dynamic_cast<fraction *>(item.get()))
        {
            int top_w = width(f->top);
            int bottom_w = width(f->bottom);
            w += std::max(top_w, bottom_w);
        }
        if (r = dynamic_cast<root *>(item.get()))
        {
            w += height(r->argument) + width(r->argument);
        }
        if (sub_g = dynamic_cast<sub_glyph *>(item.get()))
        {
            w += width(sub_g->sub);
        }
        if (sup_g = dynamic_cast<sup_glyph *>(item.get()))
        {
            w += width(sup_g->sup);
        }
        if (ss_g = dynamic_cast<sub_sup_glyph *>(item.get()))
        {
            int top_w = width(ss_g->sup);
            int bottom_w = width(ss_g->sub);
            w += std::max(top_w, bottom_w);
        }
        if (g = dynamic_cast<glyph *>(item.get()))
        {
            w += g->data.size();
        }

        container.push(std::move(item));
        start--;
    }
    return w;
}

static int height(glyph_container &container)
{
    int h = 0;
    int start = container.size();

    fraction *f;
    root *r;
    sub_glyph *sub_g;
    sup_glyph *sup_g;
    sub_sup_glyph *ss_g;
    glyph *g;

    while (start > 0)
    {
        auto item = std::move(container.front());
        container.pop();

        if (f = dynamic_cast<fraction *>(item.get()))
        {
            h = std::max(h, height(f->top) + height(f->bottom)) + 1; // + 1
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            h = std::max(h, height(r->argument)) + 1;  //+1
        }
        else if (sub_g = dynamic_cast<sub_glyph *>(item.get()))
        {
            h = std::max(h, height(sub_g->sub)) + 1;
        }
        else if (sup_g = dynamic_cast<sup_glyph *>(item.get()))
        {
            h = std::max(h, height(sup_g->sup)) + 1;
        }
        else if (ss_g = dynamic_cast<sub_sup_glyph *>(item.get()))
        {
            h = std::max(h, height(ss_g->sup) + height(ss_g->sub));
        }
        else if (g = dynamic_cast<glyph *>(item.get()))
        {
            h = std::max(h, 1);
        }

        container.push(std::move(item));
        start--;
    }

    return h;
}