#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <queue>
#include <memory>
#include <string>

#include <algorithm>
#include <sstream>

struct base_glyph;
struct glyph;
struct fraction;
struct root;
struct sqrt;

using base_glyph_ptr = std::unique_ptr<base_glyph>;
using glyph_container = std::queue<base_glyph_ptr>;
/*
using glyph_ptr = std::unique_ptr<glyph>;
using fraction = std::unique_ptr<fraction>;
using root = std::unique_ptr<root>;
using sqrt = std::unique_ptr<sqrt>;
*/
static int width(base_glyph *);     // FIXME: wrong output
static int height(base_glyph *);    // FIXME: wrong output

struct base_glyph
{
    virtual ~base_glyph() {}
};

struct glyph : base_glyph
{
    explicit glyph(const std::string &d) : data(d) {}
    std::string data;
};

struct sup_glyph : glyph
{
    explicit sup_glyph(const std::string &s) : glyph(s) {}
    glyph_container sup;
};

struct sub_glyph : glyph
{
    explicit sub_glyph(const std::string &s) : glyph(s) {}
    glyph_container sub;
};

struct fraction : glyph
{
    explicit fraction() : glyph(std::string()) {}
    glyph_container top;
    glyph_container bottom;
};

struct root : glyph
{
    explicit root() : glyph(std::string()) {}
    int power;
    glyph_container argument;
};

struct sqrt : root
{
    explicit sqrt() : root()
    {
        power = 2;
    }
};

static int width(base_glyph *ptr)
{
    int w = 0;

    fraction *f;
    root *r;
    sup_glyph *sup_g;
    sub_glyph *sub_g;
    glyph *g;

    if (f = dynamic_cast<fraction *>(ptr))
    {
        int w_top = 0, w_bottom = 0;
        int start = f->top.size();
        while (start > 0)
        {
            auto item = std::move(f->top.front());
            f->top.pop();
            w_top += width(item.get());
            f->top.push(std::move(item));
            start--;
        }
        start = f->bottom.size();
        while (start > 0)
        {
            auto item = std::move(f->bottom.front());
            f->bottom.pop();
            w_bottom += width(item.get());
            f->bottom.push(std::move(item));
            start--;
        }

        w += std::max(w_top, w_bottom);
    }
    else if (r = dynamic_cast<root *>(ptr))
    {
        int h = height(r);
        int start = r->argument.size();
        while (start > 0)
        {
            auto item = std::move(r->argument.front());
            r->argument.pop();
            w += width(item.get());
            r->argument.push(std::move(item));
            start--;
        }
    }
    else if (sup_g = dynamic_cast<sup_glyph *>(ptr))
    {
        int start = sup_g->sup.size();
        while (start > 0)
        {
            auto item = std::move(sup_g->sup.front());
            sup_g->sup.pop();
            w += width(item.get());
            sup_g->sup.push(std::move(item));
            start--;
        }
    }
    else if (sub_g = dynamic_cast<sub_glyph *>(ptr))
    {
        int start = sub_g->sub.size();
        while (start > 0)
        {
            auto item = std::move(sub_g->sub.front());
            sub_g->sub.pop();
            w += width(item.get());
            sub_g->sub.push(std::move(item));
            start--;
        }
    }
    else if (g = dynamic_cast<glyph *>(ptr))
    {
        w += g->data.size();
    }

    return w;
}

static int height(base_glyph *ptr)
{
    int h = 0;

    fraction *f;
    root *r;
    sup_glyph *sup_g;
    sub_glyph *sub_g;
    glyph *g;

    if (f = dynamic_cast<fraction *>(ptr))
    {
        int start = f->top.size();
        while (start > 0)
        {
            auto item = std::move(f->top.front());
            f->top.pop();
            h += height(item.get());
            f->top.push(std::move(item));
            start--;
        }
        start = f->bottom.size();
        while (start > 0)
        {
            auto item = std::move(f->bottom.front());
            f->bottom.pop();
            h += height(item.get());
            f->bottom.push(std::move(item));
            start--;
        }
        h++;
    }
    else if (r = dynamic_cast<root *>(ptr))
    {
        int start = r->argument.size();
        while (start > 0)
        {
            auto item = std::move(r->argument.front());
            r->argument.pop();
            h += height(item.get());
            r->argument.push(std::move(item));
            start--;
        }
    }
    else if (sup_g = dynamic_cast<sup_glyph *>(ptr))
    {
        int start = sup_g->sup.size();
        while (start > 0)
        {
            auto item = std::move(sup_g->sup.front());
            sup_g->sup.pop();
            h += height(item.get());
            sup_g->sup.push(std::move(item));
            start--;
        }
    }
    else if (sub_g = dynamic_cast<sub_glyph *>(ptr))
    {
        int start = sub_g->sub.size();
        while (start > 0)
        {
            auto item = std::move(sub_g->sub.front());
            sub_g->sub.pop();
            h += height(item.get());
            sub_g->sub.push(std::move(item));
            start--;
        }
    }
    else if (g = dynamic_cast<glyph *>(ptr))
    {
        h += 1;
    }

    return h;
}
#endif //GLYPH_HPP