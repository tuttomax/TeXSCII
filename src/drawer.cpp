#include "drawer.hpp"
#include <algorithm>
#include <functional>
#include <cmath>
#include <iostream>

#define LOG(x) std::clog << x << std::endl;

static int width(glyph_container &);
static int height(glyph_container &);

static bool find__if(glyph_container &, std::function<bool(base_glyph *)>);

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
        draw_fraction(f, sender);
    }
    else if (r = dynamic_cast<root *>(ptr))
    {
        draw_root(r, sender);
    }
    else if (sub_g = dynamic_cast<sub_glyph *>(ptr))
    {
        draw_sub(sub_g, sender);
    }
    else if (sup_g = dynamic_cast<sup_glyph *>(ptr))
    {
        draw_sup(sup_g, sender);
    }
    else if (ss_g = dynamic_cast<sub_sup_glyph *>(ptr))
    {
        draw_sub_sup(ss_g, sender);
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

void drawer::draw_fraction(fraction *f,base_glyph *sender )
{
    
    int top_w = width(f->top);
    int bottom_w = width(f->bottom);
    int w = std::max(top_w, bottom_w);
    int h_t = height(f->top);
    int h_b = height(f->bottom);
    int start_pos = pos;
    int top_x = start_pos + ceil(w / 2.0) - ceil(top_w / 2.0);
    int bottom_x = start_pos + ceil(w / 2.0) - ceil(bottom_w / 2.0);
    
    bool more_up = find__if(f->top, [](base_glyph *_ptr) -> bool {
        return (dynamic_cast<sub_glyph *>(_ptr) || dynamic_cast<sub_sup_glyph *>(_ptr));
    });

    bool more_down = find__if(f->bottom, [](base_glyph *_ptr) -> bool {
        return (dynamic_cast<sup_glyph *>(_ptr) || dynamic_cast<sub_sup_glyph *>(_ptr) || dynamic_cast<root *>(_ptr));
    });

    level -= more_up ? h_t : 1;
    pos = top_x;
    add_glyphes(f->top,f);
    level += more_up ? h_t : 1;

    level += more_down ? h_b : 1;
    pos = bottom_x;    
    add_glyphes(f->bottom,f);
    level -= more_down ? h_b : 1;

    pos = start_pos;
    table[level].write(std::string(w, '-'));

    pos = std::max(pos,start_pos+w);
}

void drawer::draw_root(root *r,base_glyph *sender)
{
    int h = height(r->argument);
    int w = width(r->argument);

    int internal_level = level;
    int start_pos = pos;

    for (int i = 0; i <= h; i++)
    {
        table[internal_level].position(start_pos);
        if (i == 0)
        {
            table[internal_level--].write('V');
            start_pos++;
        }
        else if (i == h)
        {
            if (!dynamic_cast<_sqrt *>(r))
            {
                table[internal_level].position(start_pos - 1);
                table[internal_level].write(r->power);
            }
            table[internal_level--].write(std::string(w, '_'));
        }
        else
        {
            table[internal_level--].write('/');
            start_pos++;
        }
    }

    bool adjust_level = find__if(r->argument, [](base_glyph *ptr) { return dynamic_cast<fraction *>(ptr) || dynamic_cast<sub_sup_glyph *>(ptr) || dynamic_cast<sub_glyph *>(ptr); });
    if (adjust_level)
        level--; //FIX
    pos = start_pos;
    add_glyphes(r->argument,sender);
    pos = std::max(pos, table[level].position());
}

void drawer::draw_sub(sub_glyph *s,base_glyph *sender)
{
    table[level].position(pos);
    table[level].write(s->data);
    pos = std::max(pos, table[level].position());

    int h = height(s->sub);
    level += h;
    add_glyphes(s->sub,sender);
    level -= h;
}

void drawer::draw_sup(sup_glyph *s,base_glyph *sender)
{
    table[level].position(pos);
    table[level].write(s->data);
    pos = std::max(pos, table[level].position());

    int h = height(s->sup);

    level -= h;
    add_glyphes(s->sup,sender);
    level += h;
}
void drawer::draw_sub_sup(sub_sup_glyph *s,base_glyph *sender)
{
    table[level].position(pos);
    table[level].write(s->data);
    pos = std::max(pos, table[level].position());

    int start_pos = pos;

    int h_t = height(s->sup);
    int h_b = height(s->sub);
    int w_t = width(s->sup);
    int w_b = width(s->sub);


    level -= h_t;
    table[level].position(start_pos);
    add_glyphes(s->sup,s);
    level += h_t;

    level += h_b;
    table[level].position(start_pos);    
    add_glyphes(s->sub,s);
    level -= h_b;

    
}
void drawer::draw_glyph(glyph *g, base_glyph *sender)
{
    if (sender)
    {
        if (dynamic_cast<sub_sup_glyph*>(sender))
        {
            table[level].write(g->data);
            pos = std::max(pos, table[level].position());  
        }
        else if (dynamic_cast<fraction*>(sender))
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
            h = std::max(h, height(f->top) + height(f->bottom) + 1);
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            h = std::max(h, height(r->argument) + 1);
        }
        else if (sub_g = dynamic_cast<sub_glyph *>(item.get()))
        {
            h = std::max(h, height(sub_g->sub) + 1);
        }
        else if (sup_g = dynamic_cast<sup_glyph *>(item.get()))
        {
            h = std::max(h, height(sup_g->sup) + 1);
        }
        else if (ss_g = dynamic_cast<sub_sup_glyph *>(item.get()))
        {
            h = std::max(h, height(ss_g->sup) + height(ss_g->sub) + 1);
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

static bool find__if(glyph_container &container, std::function<bool(base_glyph *)> predicate)
{
    int start = container.size();
    bool found = false;

    while (start > 0)
    {
        auto item = std::move(container.front());
        container.pop();

        if (!found)
            found = predicate(item.get());

        container.push(std::move(item));

        start--;
    }

    return found;
}
