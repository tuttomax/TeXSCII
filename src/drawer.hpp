#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <ostream>
#include <map>

#include "glyph.hpp"

class drawer
{
public:
    void show(std::ostream& os,glyph_container& container)
    {
        while (!container.empty())
        {
            auto item = std::move(container.front());
            container.pop();

            os << item->to_string();
        }
    }
    
private:

};

#endif //DRAWER_HPP