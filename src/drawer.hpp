#ifndef DRAWER_HPP
#define DRAWER_HPP

#include <ostream>
#include <map>

#include "glyph.hpp"
class row;

class drawer
{
public:
  void show(std::ostream &);
  void add_glyphes(glyph_container &);

private:
  void add_glyph(base_glyph *, base_glyph* sender = nullptr);

  void draw_fraction(fraction *);
  void draw_root(root *);
  void draw_sup(sup_glyph *);
  void draw_sub(sub_glyph *);
  void draw_sub_sup(sub_sup_glyph*);
  void draw_glyph(glyph *, base_glyph* sender = nullptr);

  int pos = 0;
  int level = 0;
  std::map<int, row> table;
};

class row
{
public:
  void write(const std::string &text)
  {
    data += text;
    pos += text.size();
  }
  void write(const char c)
  {
    data += c;
    pos++;
  }

  void write(int i)
  {
    write((char)(i + '0'));
  }

  void fill_if(int n,char c)
  {
    if (pos < n) data += std::string(n-pos,c);
  }

  int position() { return pos; }
  void position(int p) { pos = p; }
  int width() { return data.size(); }
  std::string text() { return data; }  
private:
  int pos;
  std::string data;
};


#endif //DRAWER_HPP