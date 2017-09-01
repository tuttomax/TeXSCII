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

};

#endif //DRAWER_HPP