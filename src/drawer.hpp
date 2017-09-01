#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <ostream>
#include "glyph.hpp"

class drawer
{
public:
    void add_glyphes(glyph_container& container);
    void show(std::ostream& stream);

private:
    void add_glyph(base_glyph_ptr& smart_ptr);
};

#endif //DRAWER_HPP