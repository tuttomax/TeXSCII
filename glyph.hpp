#ifndef GLYPH_HPP
#define GLYPH_HPP

#include <string>
#include <deque>
#include <memory>
#include <ostream>

class base_glyph;
class glyph;
class fracture;
class root;
class sqrt;

using base_glyph_ptr = std::unique_ptr<base_glyph>;
using glyph_container = std::deque<base_glyph_ptr>;

using glyph_ptr = std::unique_ptr<glyph>;
using fracture_ptr = std::unique_ptr<fracture>;
using root_ptr = std::unique_ptr<root>;
using sqrt_ptr = std::unique_ptr<sqrt>;

class base_glyph
{
  public:
   
    virtual ~base_glyph() {}
};

class glyph : public base_glyph
{
  public:
    explicit glyph(int level, std::string data) : level(level), data(data) {}

    int level;
    std::string data;
};


class fracture : public glyph
{
  public:
    explicit fracture(int level) : glyph(level, std::string()) {}

    glyph_container top;
    glyph_container bottom;
};

class root : public glyph
{
  public:
    explicit root(int level) : glyph(level, std::string()) {}

    int power;
    glyph_container argument;
};

class sqrt : public root
{
  public:
    explicit sqrt(int level) : root(level) {  
      power = 2;
    }
};



#endif //GLYPH_HPP