#ifndef STYLE_H
#define STYLE_H

#include <ostream>
#include <vector>
#include <map>

class Document;
class Content;
class Element;
class CompositeElement;

namespace Xsl
{
  typedef ::Document XMLDocument;

  void apply_style(XMLDocument const &, XMLDocument const &, std::ostream &);
  int validate(Element const &, std::ostream &);
}

#endif // vim:ft=cpp et sw=2 sts=2:

