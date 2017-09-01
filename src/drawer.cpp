#include "drawer.hpp"

void drawer::show(std::ostream& os)
{
    
}

void drawer::add_glyphes(glyph_container& container)
{
    while (!container.empty())
    {
        auto item = std::move(container.front());
        container.pop();
        add_glyph(item);
    }
}

void drawer::add_glyph(base_glyph_ptr& smart_ptr)
{

}

