#include "style.h"
#include "document.h"
#include "element.h"
#include "attribute.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#define XSL_NS "xsl"
#define XSL_APPLY_TEMPLATES "apply-templates"
#define XSL_MATCH "match"

class XslDocument
{
  public:
    XslDocument(Document const &);

    void apply_style_to(Document const &, std::ostream &);

  private:
    class Template
    {
      public:
        Template(Element const *);
        Template() = default;
        ~Template() = default;

      private:
        Element const * root;
    };

    std::map<std::string, Template> _templates;
    Template _root;
};

void xml_apply_style(Document const & xml, Document const & xsl, std::ostream & os)
{
  XslDocument xsl_doc(xsl);
  xsl_doc.apply_style_to(xml, os);
}

XslDocument::XslDocument(Document const & doc):
  _templates(), _root(doc.root())
{
  for (auto child : doc.root()->children())
  {
    Element * element = dynamic_cast<Element *>(child);
    if (element == nullptr) { continue; }

    auto ns_split = element->ns_split();

    // F**king lower() method
    std::transform(ns_split.first.begin(), ns_split.first.end(),
        ns_split.first.begin(), ::tolower);
    std::transform(ns_split.second.begin(), ns_split.second.end(),
        ns_split.second.begin(), ::tolower);

    // Only xsl namespace
    if (ns_split.first == XSL_NS && ns_split.second == XSL_APPLY_TEMPLATES)
    {
      for (auto attr : element->attributes())
      {
        if (attr->name() == XSL_MATCH)
        {
          if (attr->value().empty()) { continue; }
          _templates[attr->value()] = { element }; // FIXME this overwrites older templates
          break;
        }
      }
    }
  }
}

void XslDocument::apply_style_to(Document const &, std::ostream &) {}

XslDocument::Template::Template(Element const * element):
  root(element)
{
}

// vim:ft=cpp et sw=2 sts=2:
