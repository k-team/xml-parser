#include "style.h"
#include "document.h"
#include "element.h"
#include "composite_element.h"
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

namespace Xsl
{
  typedef ::Document XMLDocument;

  bool is_element(Element const &, std::string const & = "");

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &) const;
      void apply_template_to(Element const &, Element const &, std::ostream &) const;
      void apply_template_to(CompositeElement const &, Element const &, std::ostream &) const;
      void do_something_special(Element const &, Element const &, std::ostream &) const;

    private:
      std::multimap<std::string, Element const *> _absolute_path_templates;
      std::multimap<std::string, Element const *> _relative_path_templates;
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
    bool ret = ns_split.first == XSL_NS && (tag.empty() || ns_split.second == tag);
    //std::cout << "is_element(" << element.str() << ")" << ret << std::endl;
    return ret;
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

  void Document::apply_style_to(XMLDocument const & xml, std::ostream & os) const
  {
    auto range = _absolute_path_templates.equal_range("/");
    for (auto it = range.first; it != range.second; it++)
    {
      if (it != _absolute_path_templates.end())
      {
        auto ce = dynamic_cast<CompositeElement const *>(it->second);
        apply_template_to(*ce, *xml.root(), os);
      }
      else
      {
        // TODO handle when to "/" template is given
      }
    }
  }

  void Document::apply_template_to(Element const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    if (is_element(template_element))
    {
      do_something_special(template_element, root_element, os);
    }
    else
    {
      auto ce = dynamic_cast<CompositeElement const *>(&template_element);
      if (ce != nullptr)
      {
        os << template_element.str() << std::endl;
      }
      else
      {
        os << "<" << ce->name() << ">" << std::endl;
        apply_template_to(*ce, root_element, os);
        os << "</" << ce->name() << ">" << std::endl;
      }
    }
  }

  void Document::apply_template_to(CompositeElement const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    for (auto child : template_element.children())
    {
      auto child_element = dynamic_cast<Element const *>(child);
      if (child_element == nullptr)
      {
        os << child->str() << std::endl;
      }
      else
      {
        apply_template_to(template_element, *child_element, os);
      }
    }
  }

  void Document::do_something_special(Element const & template_element,
      Element const & element, std::ostream & os) const
  {
    std::cout << template_element.name() << std::endl;
    //os << template_element.str()<< std::endl;
  }
}

void xml_apply_style(Document const & xml, Document const & xsl, std::ostream & os)
{
  Xsl::Document xsl_doc(xsl); // FIXME check for xsl:stylesheet root element
  xsl_doc.apply_style_to(xml, os);
}

// vim:ft=cpp et sw=2 sts=2:
