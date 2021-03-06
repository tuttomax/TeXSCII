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
struct _sqrt;

using base_glyph_ptr = std::unique_ptr<base_glyph>;
using glyph_container = std::queue<base_glyph_ptr>;
/*
using glyph_ptr = std::unique_ptr<glyph>;
using fraction = std::unique_ptr<fraction>;
using root = std::unique_ptr<root>;
using _sqrt = std::unique_ptr<_sqrt>;
*/


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
struct sub_sup_glyph : glyph
{
    explicit sub_sup_glyph(const std::string& s) : glyph(s) { }
    glyph_container sup;    
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

struct _sqrt : root
{
    explicit _sqrt() : root()
    {
        power = 2;
    }
};


#endif //GLYPH_HPP