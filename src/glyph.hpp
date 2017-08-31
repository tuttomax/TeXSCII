#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <stack>
#include <memory>
#include <string>

struct base_glyph;
struct glyph;
struct fraction;
struct root;
struct sqrt;

using base_glyph_ptr = std::unique_ptr<base_glyph>;
using glyph_container = std::stack<base_glyph_ptr>;
/*
using glyph_ptr = std::unique_ptr<glyph>;
using fraction = std::unique_ptr<fraction>;
using root = std::unique_ptr<root>;
using sqrt = std::unique_ptr<sqrt>;
*/

struct base_glyph
{
    virtual ~base_glyph() { }
};

struct glyph : base_glyph
{
    explicit glyph(const std::string &d) : data(d) {}
    std::string data;
};

struct sup_glyph : glyph
{
    explicit sup_glyph(const std::string& s) : glyph(s) {}
    glyph_container sup;
};

struct sub_glyph : glyph
{
    explicit sub_glyph(const std::string& s) : glyph(s) {}

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

#endif //GLYPH_HPP