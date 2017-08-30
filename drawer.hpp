#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <map>
#include <ostream>
#include <vector>
#include <string>
#include "glyph.hpp"

class row
{

public:
  row() : text(1024 * 4) { /* 4MB of string should be enough */}

  void write(const std::string text)
  {
    for (const auto &c : text)
    {
      write(c);
    }
  }

  void write(const char c)
  {
    text[current_position++] = c;
  }

  void write(const int i)
  {
    write((char)('0' + i));
  }

  void fill(int n, char c)
  {
    write(std::string(n, c));
  }

  void position(int pos)
  {
    if (current_position < pos) fill(pos-current_position,' ');
    
    current_position = pos;
  }
  int position() { return current_position; }
  void print(std::ostream &os)
  {
    for (const auto &c : text)
    {
      os << c;
    }
  }

private:
  int current_position = 0;
  std::vector<char> text;
};

class drawer
{

public:
  void add_glyph(base_glyph_ptr &glyph);
  void add_glyphes(glyph_container &container);
  void print(std::ostream &os);

private:
  std::map<int, row> table;
  int pos = 0;

  void draw_fracture(fracture* f);
  void draw_root(root* r);
  void draw_glyph(glyph* g);
  
};

#endif //DRAWER_HPP