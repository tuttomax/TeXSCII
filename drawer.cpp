#include "drawer.hpp"
#include <algorithm>

//UTILITY FUNCTION

static int height(glyph_container &);
static int count_all_chars(glyph_container &);
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

static int count_all_chars(glyph_container &container)
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
            int top_l = count_all_chars(f->top);
            int bottom_l = count_all_chars(f->bottom);
            count += std::max(top_l, bottom_l);
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            count += count_all_chars(r->argument);
            int depth = height(r->argument);
            count += depth;
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

static int height(glyph_container &container)
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
            int t_c = abs_n(height(f->top));
            int b_c = height(f->bottom);
            current = std::max(t_c, b_c);
        }
        else if (r = dynamic_cast<root *>(item.get()))
        {
            current = std::max(current, height(r->argument));
        }
        else if (g = dynamic_cast<glyph *>(item.get()))
        {
            current = std::max(current, g->level);
        }

        container.push_back(std::move(item));
        start_size--;
    }
    return current;
}

void drawer::draw_fracture(fracture *f)
{
    int top_width = count_all_chars(f->top);
    int bottom_width = count_all_chars(f->bottom);
    int top_depth = height(f->top);
    int bottom_depth = height(f->bottom);

    table[f->level].position(pos);
    table[f->level].write(std::string(std::max(top_width, bottom_width) + std::max(top_depth, bottom_depth), '-'));

    //set start position for writing
    table[f->level - 1].position(pos);
    table[f->level + 1].position(pos);

    while (!f->top.empty() || !f->bottom.empty())
    {
        base_glyph_ptr top_item;
        base_glyph_ptr bottom_item;

        //for now avoid align to center
        int temp = pos;

        if (!f->top.empty())
        {
            top_item = std::move(f->top.front());
            f->top.pop_front();
            add_glyph(top_item);
        }
        if (!f->bottom.empty())
        {
            bottom_item = std::move(f->bottom.front());
            f->bottom.pop_front();
            pos = temp;
            add_glyph(bottom_item);
        }
    }
}

void drawer::draw_root(root *r)
{
    int depth = height(r->argument);
    int width = count_all_chars(r->argument) + depth;

    for (int level = r->level; level >= depth - 1; level--)
    {
        table[level].position(pos);

        if (level == r->level)
        {
            table[level].write('V');
            pos++;
        }
        else if (level == depth - 1)
        {
            if (!dynamic_cast<sqrt *>(r))
                table[level].write(r->power);
            table[level].write(std::string(width, '_'));
        }
        else
        {
            table[level].write('/');
            pos++;
        }
    }

    add_glyphes(r->argument);
}

void drawer::draw_glyph(glyph *g)
{
    table[g->level].position(pos);
    table[g->level].write(g->data);
    pos += g->data.size();
}

int depth_root(root* r)
{
    int start = r->argument.size();
    root* r_ptr;
    fracture* f_ptr;
    glyph* g_ptr;

    int current = 0;

    while (start>0)
    {
        auto item = std::move(r->argument.front());
        r->argument.pop_front();

        if (r_ptr = dynamic_cast<root*>(item.get()))
        {
            current += depth_root(r_ptr);
        }
        else if(f_ptr = dynamic_cast<fracture*>(item.get()))
        {
            int start_top = f_ptr->top.size();
            int start_bottom = f_ptr->bottom.size();
            while (start_top > 0 || start_bottom > 0)
            {
                if (start_top > 0)
                {
                    auto item = std::move(f_ptr->top.front());
                    f_ptr->top.pop_front();
                    if (r_ptr = dynamic_cast<root*>(item.get()))
                    {
                        current += depth_root(r_ptr);
                    }
                    else if (f_ptr = dynamic_cast<fracture*>(item.get()))
                    {

                    }
                    else if (g_ptr = dynamic_cast<glyph*>(item.get()))
                    {
                        current += g_ptr->level;
                    }
                    f_ptr->top.push_back(std::move(item));
                    start_top--;
                }
                if (start_bottom > 0)
                {
                    auto item = std::move(f_ptr->bottom.front());
                    f_ptr->bottom.pop_front();

                    f_ptr->bottom.push_back(std::move(item));
                    start_bottom--;
                }
            }


        }
        else if (g_ptr = dynamic_cast<glyph*>(item.get()))
        {
            current += g_ptr->level;
        }

        r->argument.push_back(std::move(item));
    }

    return current;
} 

