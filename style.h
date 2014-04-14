#ifndef STYLE_H
#define STYLE_H

#include <ostream>

class Document;

namespace Xsl
{
  typedef ::Document XMLDocument;

  void apply_style(XMLDocument const &, XMLDocument const &,
      std::ostream &, std::ostream &);
}

#endif // vim:ft=cpp et sw=2 sts=2:

