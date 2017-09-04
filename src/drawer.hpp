#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <ostream>
#include <map>
#include <stdexcept>
#include <stack>
#include <array>


#include "glyph.hpp"

class row;

class drawer
{
public:
  void show(std::ostream &);
  void add_glyphes(glyph_container &, base_glyph *sender = nullptr);

private:
  void add_glyph(base_glyph *, base_glyph *sender = nullptr);

  void draw_fraction(fraction *);
  void draw_root(root *);
  void draw_sup(sup_glyph *);
  void draw_sub(sub_glyph *);
  void draw_sub_sup(sub_sup_glyph *);
  void draw_glyph(glyph *, base_glyph *sender = nullptr);

  int pos = 0;
  int level = 0;

  std::map<int, row> table;

};

class row
{
 
public:
  void write(int i)
  {
    write((char)(i + '0'));
  }
  void write(const std::string& s)
  {
    for(const auto& c : s)
    {
      write(c);
    }
  }
  void write(const char c)
  {
    if (current_position > 1024 ) throw std::runtime_error("row error: pos is greater thand SIZE");
    
    internal_text[current_position] = c;
    current_position++;
  }

  int position() { return current_position; }
  void position(int pos) { current_position = pos; }
  
  std::string text() 
  {
    return std::string(internal_text.cbegin(),internal_text.cend());
  }

private:
  int current_position = 0;
  std::array<char,1024> internal_text;
  
};

#endif //DRAWER_HPP