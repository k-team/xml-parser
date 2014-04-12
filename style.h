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

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &) const;

    private:
      void handle_e(Element const &, Element const &, std::ostream &) const;
      void handle_ce(CompositeElement const &, Element const &, std::ostream &) const;
      void handle_xsl(Element const &, Element const &, std::ostream &) const;
      void handle_apply_templates(Element const &, Element const &, std::ostream &) const;
      void handle_for_each(Element const &, Element const &, std::ostream &) const;
      void handle_value_of(Element const &, Element const &, std::ostream &) const;
      void handle_apply_all_templates(Content const *, std::ostream &) const;

      CompositeElement const * _root_template;
      std::map<std::string, Element const *> _templates;
  };

  void apply_style(XMLDocument const &, XMLDocument const &, std::ostream &);
  int validate(Element const &, std::ostream &);
}

#endif // vim:ft=cpp et sw=2 sts=2:

