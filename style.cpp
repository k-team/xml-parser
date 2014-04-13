#include "style.h"
#include "document.h"
#include "element.h"
#include "composite_element.h"
#include "helpers.h"
#include "composite_element.h"
#include "attribute.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#define XSL_NS "xsl"
#define XSL_TEMPLATES "template"
#define XSL_MATCH "match"
#define XSL_ROOT_MATCH "/"
#define XSL_STYLESHEET "stylesheet"
#define XSL_APPLY_TEMPLATES "apply-templates"
#define XSL_VALUE_OF "value-of"
#define XSL_FOR_EACH "for-each"
#define XSL_SELECT "select"

namespace Xsl
{
  static bool is_element(Element const & element, std::string const & tag = "")
  {
    auto ns_split = element.ns_split();

    // F**king lower() method
    std::transform(ns_split.first.begin(), ns_split.first.end(),
        ns_split.first.begin(), ::tolower);
    std::transform(ns_split.second.begin(), ns_split.second.end(),
        ns_split.second.begin(), ::tolower);

    // Only xsl namespace
    return ns_split.first == XSL_NS && (tag.empty() || ns_split.second == tag);
  }

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &) const;

      void apply_templates(Element const *, Element const *, std::ostream &) const;
      void value_of(Element const *, Element const *, std::ostream &) const;
      void for_each(Element const *, Element const *, std::ostream &) const;

    private:
      void apply_all_templates(Content const *, std::ostream &) const;

      CompositeElement const * _root_template;
      std::map<std::string, Element const *> _templates;
  };

  class TemplateRenderer
  {
    public:
      TemplateRenderer(Document const & doc, Element const * root_element, std::ostream & os):
        _doc(doc), _root_element(root_element), _os(os) {}
      ~TemplateRenderer() = default;

      void render_empty(Element const *);
      void render_composite(CompositeElement const *);
      void render_xsl(Element const *);

    private:
      Document const & _doc;
      Element const * _root_element;
      std::ostream & _os;
  };

  Document::Document(XMLDocument const & doc):
    _root_template(), _templates()
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
            if (attr->value() == "/")
            {
              _root_template = dynamic_cast<CompositeElement const *>(element);
            }
            else
            {
              _templates[attr->value()] = element;
            }
            break; // because f**k you that's why !
          }
        }
      }
    }
  }

  void Document::apply_style_to(XMLDocument const & xml, std::ostream & os) const
  {
    TemplateRenderer tr(*this, xml.root(), os);
    tr.render_composite(_root_template);
  }

  void TemplateRenderer::render_composite(CompositeElement const * tmplt)
  {
    for (auto child : tmplt->children())
    {
      auto child_element = dynamic_cast<Element const *>(child);
      if (child_element != nullptr)
      {
        render_empty(child_element);
      }
      else
      {
        _os << child->str() << std::endl;
      }
    }
  }

  void TemplateRenderer::render_empty(Element const * tmplt)
  {
    if (is_element(*tmplt))
    {
      render_xsl(tmplt);
    }
    else
    {
      auto ce = dynamic_cast<CompositeElement const *>(tmplt);
      if (ce == nullptr)
      {
        _os << tmplt->str() << std::endl;
      }
      else
      {
        _os << ce->begin_str() << std::endl;
        render_composite(ce);
        _os << ce->end_str() << std::endl;
      }
    }
  }

  void TemplateRenderer::render_xsl(Element const * tmplt)
  {
    std::string xsl_operation = tmplt->ns_split().second;

    // F**king lower() method
    Helpers::lower(xsl_operation);

    // Very elegant code
    if (xsl_operation == XSL_APPLY_TEMPLATES)
    {
      _doc.apply_templates(tmplt, _root_element, _os);
    }
    else if (xsl_operation == XSL_FOR_EACH)
    {
      _doc.for_each(tmplt, _root_element, _os);
    }
    else if (xsl_operation == XSL_VALUE_OF)
    {
      _doc.value_of(tmplt, _root_element, _os);
    }
    else
    {
      _os << tmplt->str() << std::endl;
    }
  }

  void Document::apply_templates(Element const * tmplt, Element const * element, std::ostream & os) const
  {
    Attribute const * select_attr = tmplt->find_attribute(XSL_SELECT);
    if (select_attr != nullptr)
    {
      auto it =_templates.find(select_attr->value());
      if (it != _templates.end())
      {
        for (auto c : element->children())
        {
          auto cec = dynamic_cast<CompositeElement const *>(c);
          if (cec != nullptr && cec->name() == select_attr->value())
          {
            TemplateRenderer tr(*this, cec, os);
            tr.render_composite(static_cast<CompositeElement const *>(it->second));
            break;
          }
        }
      }
      else
      {
        // this should never happen, provided that we're checking input
      }
    }
    else // Output ALL the templates!
    {
      for (auto c : element->children())
      {
        apply_all_templates(c, os);
      }
    }
  }

  void Document::value_of(Element const * tmplt, Element const * element, std::ostream & os) const
  {
    Attribute const & select_attr = *tmplt->find_attribute(XSL_SELECT);

    if (select_attr.value() == ".")
    {
      auto ce = dynamic_cast<CompositeElement const *>(element);
      if (ce != nullptr)
      {
        for (auto child : ce->children())
        {
          os << child->str() << std::endl;
        }
      }
    }
    else
    {
      for (auto c : element->children())
      {
        auto ce = dynamic_cast<CompositeElement const *>(c);
        if (ce != nullptr && ce->name() == select_attr.value())
        {
          for (auto child : ce->children())
          {
            os << child->str() << std::endl;
          }
          break; // TODO only first one is written
        }
      }
    }
  }

  void Document::for_each(Element const *, Element const *, std::ostream & os) const
  {
    os << XSL_FOR_EACH << std::endl;
  }

  void Document::apply_all_templates(Content const * c, std::ostream & os) const
  {
    auto e = dynamic_cast<Element const *>(c);
    if (e == nullptr)
    {
      return;
    }

    // Apply applicable templates
    auto it = _templates.find(e->name());
    if (it !=_templates.end())
    {
      TemplateRenderer tr(*this, e, os);
      tr.render_composite(static_cast<CompositeElement const *>(it->second));
    }
    else // Recursion otherwise
    {
      for (auto c : e->children())
      {
        apply_all_templates(c, os);
      }
    }
  }

  void apply_style(XMLDocument const & xml, XMLDocument const & xsl, std::ostream & os)
  {
    Document xsl_doc(xsl); // FIXME check for xsl:stylesheet root element
    xsl_doc.apply_style_to(xml, os);
  }

  static void check_xsl_lowers_tags_level(Element const & root, std::ostream & os)
  {
    for (auto it : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
      if (ce != nullptr)
      {
        // Test presence of things different from a template on level 2
        std::string xsl_operation = ce->ns_split().second;
        if (xsl_operation == XSL_TEMPLATES || xsl_operation == XSL_STYLESHEET)
        {
          os << "xsl lower level tag incorrect" << std::endl;
        }
        else
        {
          check_xsl_lowers_tags_level(*ce, os);
        }
      }
    }
  }

  static void check_xsl_tags_level(const Element &root, std::ostream & os)
  {
    // Test presence of stylesheet on level 1
    std::string xsl_operation_root = root.ns_split().second;
    if (xsl_operation_root != XSL_STYLESHEET)
    {
      os << "xsl level 1 tag incorrect" << std::endl;
    }

    for (auto it : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
      if (ce != nullptr)
      {
        // Test presence of things different from a template on level 2
        std::string xsl_operation = ce->ns_split().second;
        if (xsl_operation != XSL_TEMPLATES)
        {
          os << "xsl level 2 tag incorrect" << std::endl;
        }
        else
        {
          check_xsl_lowers_tags_level(*ce, os);
        }
      }
      else
      {
        // This would indicate that the is smth that differs from composite element so it can not be a template
        os << "xsl level 2 tag incorrect" << std::endl;
      }
    }
  }

  static void check_xsl_multiples_apply_all(Element const & root, std::ostream & os, int & count)
  {
    for (auto it : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
      if (ce != nullptr)
      {
        check_xsl_multiples_apply_all(*ce, os, count);
      }
      else
      {
        Element * e = dynamic_cast<Element *>(it);
        if (e != nullptr)
        {
          std::string xsl_operation = e->ns_split().second;
          if (xsl_operation == XSL_APPLY_TEMPLATES &&
              !std::any_of(e->attributes().begin(), e->attributes().end(),
                [](Attribute* attr) { return (attr->name() == XSL_SELECT); }))
          {
            count++;
          }
        }
      }
    }
  }

  static void get_all_apply_template_select(const Element & root, std::vector<std::string> & vect_select)
  {
    for (auto c : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(c);
      if (ce != nullptr)
      {
        get_all_apply_template_select(*ce, vect_select);
      }
      else
      {
        Element * e = dynamic_cast<Element *>(c);
        if (e != nullptr)
        {
          std::string xsl_operation = e->ns_split().second;
          if (xsl_operation == XSL_APPLY_TEMPLATES)
          {
            for (auto attrs : e->attributes())
            {
              if (attrs->name() == XSL_SELECT)
              {
                vect_select.push_back(attrs->value());
              }
            }
          }
        }
      }
    }
  }

  static void check_xsl_apply_template_select(Element const & root, std::ostream & os)
  {
    std::vector<std::string> vect_select;
    get_all_apply_template_select(root, vect_select);

    for (auto c : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(c);
      if (ce != nullptr)
      {
        for (auto attr : ce->attributes())
        {
          if (attr->name() == XSL_MATCH)
          {
            for (unsigned int i = 0; i < vect_select.size(); i++)
            {
              if (vect_select[i] == attr->value())
              {
                vect_select.erase(vect_select.begin() + i);
                --i;
              }
            }
          }
        }
      }
    }

    if (vect_select.size() > 0)
    {
      os << "apply-template with no template corresponding" << std::endl;
    }
  }

  int validate(const Element & root, std::ostream & os)
  {
    check_xsl_tags_level(root, os);

    int count_apply_all = 0;
    check_xsl_multiples_apply_all(root, os, count_apply_all);
    if (count_apply_all > 1)
    {
      os << "multiple apply all templates" << count_apply_all << std::endl;
    }

    check_xsl_apply_template_select(root, os);

    // TODO check if for-each and value-of have a select attribute

    return 0;
  }
}

// vim:ft=cpp et sw=2 sts=2:
