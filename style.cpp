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
#include <map>
#include <algorithm>
#include <stdexcept>

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
    Helpers::lower(ns_split.first);

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
      Attribute const * get_directive_attribute(Element const *,
          std::string const &, Element const *, std::ostream &) const;

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

  class Validator
  {
    public:
      Validator(Element const &, std::ostream &);
      ~Validator() = default;

      bool operator()();

    private:
      // Checkers
      void check_tag_levels();
      void check_apply_templates_count();
      void check_apply_templates_select();
      void check_tag_lower_levels(Element const &);
      void check_that_directive_has_attribute(std::string const &, std::string const &);
      void check_that_directive_has_attribute_r(Element const &,
          std::string const &, std::string const &);

      // Helpers
      size_t count_apply_all_templates(Element const &);
      void get_all_apply_template_select(Element const &, std::vector<std::string> &);

      Element const & _root;
      std::ostream & _os;
      bool _good;
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
        auto tmplt_ce = dynamic_cast<CompositeElement const *>(tmplt);
        if (tmplt_ce != nullptr)
        {
          TemplateRenderer tr(*this, element, os);
          os << tmplt_ce->begin_str() << std::endl;
          tr.render_composite(tmplt_ce);
          os << tmplt_ce->end_str() << std::endl;
        }
        else
        {
          os << tmplt->str() << std::endl;
        }
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

  Attribute const * Document::get_directive_attribute(Element const * tmplt,
      std::string const & _attr, Element const * element, std::ostream & os) const
  {
    std::string attr(_attr);
    Helpers::trim(attr);
    Attribute const * select_attr_ptr = tmplt->find_attribute(attr);
    if (select_attr_ptr == nullptr)
    {
      auto tmplt_ce = dynamic_cast<CompositeElement const *>(tmplt);
      if (tmplt_ce != nullptr)
      {
        TemplateRenderer tr(*this, element, os);
        os << tmplt_ce->begin_str() << std::endl;
        tr.render_composite(tmplt_ce);
        os << tmplt_ce->end_str() << std::endl;
      }
      else
      {
        os << tmplt->str() << std::endl;
      }
    }
    return select_attr_ptr;
  }

  void Document::value_of(Element const * tmplt, Element const * element, std::ostream & os) const
  {
    Attribute const * select_attr_ptr = get_directive_attribute(tmplt, XSL_SELECT, element, os);
    if (select_attr_ptr == nullptr)
    {
      return;
    }

    auto select_attr = *select_attr_ptr;
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
          break;
        }
      }
    }
  }

  void Document::for_each(Element const * tmplt, Element const * element, std::ostream & os) const
  {
    Attribute const * select_attr_ptr = get_directive_attribute(tmplt, XSL_SELECT, element, os);
    if (select_attr_ptr == nullptr)
    {
      return;
    }

    // Normal case
    auto select_attr = *select_attr_ptr;
    std::string select_name = select_attr.value();

    // FIXME find a better solution than ignore selection
    auto slash_split = Helpers::split(select_name, '/');
    if (slash_split.size() == 2)
    {
      select_name = slash_split.back();
    }

    for (auto c : element->children())
    {
      auto e = dynamic_cast<Element const *>(c);
      if (e != nullptr && e->name() == select_name)
      {
        TemplateRenderer tr(*this, e, os);
        tr.render_composite(static_cast<CompositeElement const *>(tmplt));
      }
    }
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

  Validator::Validator(Element const & root, std::ostream & os):
    _root(root), _os(os), _good(true)
  {
  }

  void Validator::check_tag_lower_levels(Element const & root)
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
          _os << "xsl lower level tag incorrect" << std::endl;
          _good = false;
        }
        else
        {
          check_tag_lower_levels(*ce);
        }
      }
    }
  }

  void Validator::check_tag_levels()
  {
    if (!is_element(_root, XSL_STYLESHEET))
    {
      _os << "xsl level 1 tag incorrect" << std::endl;
      _good = false;
    }

    for (auto it : _root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
      if (ce != nullptr)
      {
        // Test presence of things different from a template on level 2
        std::string xsl_operation = ce->ns_split().second;
        if (xsl_operation != XSL_TEMPLATES)
        {
          _os << "xsl level 2 tag incorrect" << std::endl;
          _good = false;
        }
        else
        {
          check_tag_lower_levels(*ce);
        }
      }
      else
      {
        // This would indicate that this differs from a composite
        // element therefore it can't be a template
        _os << "xsl level 2 tag incorrect" << std::endl;
        _good = false;
      }
    }
  }

  size_t Validator::count_apply_all_templates(Element const & root)
  {
    size_t count = 0;
    for (auto it : root.children())
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
      if (ce != nullptr)
      {
        count += count_apply_all_templates(*ce);
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
    return count;
  }

  void Validator::get_all_apply_template_select(Element const & root, std::vector<std::string> & vect_select)
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

  void Validator::check_apply_templates_select()
  {
    std::vector<std::string> vect_select;
    get_all_apply_template_select(_root, vect_select);

    for (auto c : _root.children())
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
      _os << XSL_APPLY_TEMPLATES << " given with no corresponding template" << std::endl;
      _good = false;
    }
  }

  void Validator::check_apply_templates_count()
  {
    size_t count_apply_templates = count_apply_all_templates(_root);
    if (count_apply_templates > 1)
    {
      _os << "only one " << XSL_APPLY_TEMPLATES << " directive can be given,"
        << " got " << count_apply_templates;
      _good = false;
    }
  }

  void Validator::check_that_directive_has_attribute(std::string const & directive,
      std::string const & attr_name)
  {
    check_that_directive_has_attribute_r(_root, directive, attr_name);
  }

  void Validator::check_that_directive_has_attribute_r(Element const & root,
      std::string const & directive, std::string const & attr_name)
  {
    for (auto child : root.children())
    {
      auto e = dynamic_cast<Element const *>(child);
      if (e != nullptr)
      {
        if (is_element(*e, directive))
        {
          auto attr = e->find_attribute(attr_name);
          if (attr == nullptr || attr->value().empty())
          {
            _os << "directive needs a " << attr_name << " attribute" << std::endl;
            _good = false;
          }
        }
        check_that_directive_has_attribute_r(*e, directive, attr_name);
      }
    }
  }

  bool Validator::operator()()
  {
    // Tag levels should be as specified (xsl:stylesheet)
    check_tag_levels();

    // The "apply-templates" directive can only be given once with no arguments
    check_apply_templates_count();

    // The "apply-templates" directive should either be empty or refer to a given template
    check_apply_templates_select();

    // "for-each" and "value-of" directives should have a "select" attribute
    check_that_directive_has_attribute(XSL_FOR_EACH, XSL_SELECT);
    check_that_directive_has_attribute(XSL_VALUE_OF, XSL_SELECT);

    // Why so serious?
    return true;//_good;
  }

  // Only public function of the module
  void apply_style(XMLDocument const & xml, XMLDocument const & xsl,
      std::ostream & out, std::ostream & err)
  {
    if (Validator(*xsl.root(), err)())
    {
      Document xsl_doc(xsl);
      xsl_doc.apply_style_to(xml, out);
    }
  }
}

// vim:ft=cpp et sw=2 sts=2:
