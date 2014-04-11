#include "style.h"
#include "document.h"
#include "element.h"
#include "helpers.h"
#include "attribute.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>

#define XSL_NS "xsl"
#define XSL_TEMPLATES "template"
#define XSL_APPLY_TEMPLATES "apply-templates"
#define XSL_MATCH "match"
#define XSL_ROOT_MATCH "/"

namespace Xsl {
  typedef ::Document XMLDocument;

  bool is_element(Element const & element, std::string const & tag);

  class Template;

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &);

    private:
      std::multimap<std::string, Template> _absolute_path_templates;
      std::multimap<std::string, Template> _relative_path_templates;
  };

  class Template
  {
    public:
      Template(Element const * element): _root_ptr(element) {}
      Template() = default;
      ~Template() = default;

      void apply_to(Element const &, std::ostream &) const;

    private:
      Element const * _root_ptr;
  };

  // Implementation

  bool is_element(Element const & element, std::string const & tag)
  {
    auto ns_split = element.ns_split();

    // F**king lower() method
    std::transform(ns_split.first.begin(), ns_split.first.end(),
        ns_split.first.begin(), ::tolower);
    std::transform(ns_split.second.begin(), ns_split.second.end(),
        ns_split.second.begin(), ::tolower);

    // Only xsl namespace
    return ns_split.first == XSL_NS && ns_split.second == tag;
  }

  Document::Document(XMLDocument const & doc):
    _absolute_path_templates(),
    _relative_path_templates()
  {
    for (auto child : doc.root()->children())
    {
      Element * element = dynamic_cast<Element *>(child);
      if (element != nullptr && is_element(*element, XSL_TEMPLATES))
      {
        for (auto attr : element->attributes())
        {
          if (attr->name() == XSL_MATCH && !attr->value().empty())
          {
            ((Helpers::trim(Helpers::split(attr->value(), '/')[0]).empty())
              ? _absolute_path_templates
              : _relative_path_templates
            ).insert({ attr->value(), { element } });

            break; // because f**k you that's why !
          }
        }
      }
    }
  }

  void Document::apply_style_to(XMLDocument const & xml, std::ostream & os)
  {
    Element const & root = *xml.root();
    auto it = _absolute_path_templates.find("/");
    if (it != _absolute_path_templates.end())
    {
      it->second.apply_to(root, os);
    }
    else
    {
      // TODO handle when to "/" template is given
    }
  }

  void Template::apply_to(Element const & element, std::ostream & os) const
  {
    os << "applying template to " << element.name() << std::endl;
    if (false)
    {
    }
    else
    {
      //os << element.str();
    }
  }
}

void xml_apply_style(Document const & xml, Document const & xsl, std::ostream & os)
{
  Xsl::Document xsl_doc(xsl); // FIXME check for xsl:stylesheet root element
  xsl_doc.apply_style_to(xml, os);
}

// vim:ft=cpp et sw=2 sts=2:
