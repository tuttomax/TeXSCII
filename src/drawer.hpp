#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <ostream>
#include <map>

#include "glyph.hpp"

class drawer
{
  public:
    void show(std::ostream &);
    void add_glyphes(glyph_container &);
  private:
   
    void add_glyph(base_glyph *);

    void draw_fraction(fraction *);
    void draw_root(root *);
    void draw_sup(sup_glyph *);
    void draw_sub(sub_glyph *);
    void draw_glyph(glyph *);

    std::map<int, std::string> table;
};

#endif //DRAWER_HPP