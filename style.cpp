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
#define XSL_APPLY_TEMPLATES "apply-templates"
#define XSL_MATCH "match"
#define XSL_ROOT_MATCH "/"

namespace Xsl {
  typedef ::Document XMLDocument;

  class Path;
  class Template;

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &);
      void apply_style_to(Element const &, Path, std::ostream &);
      void apply_children_style_to(Element const &, Path, std::ostream &);

    private:
      std::map<Path, Template> _templates;
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

  class Path
  {
    public:
      Path(std::string const &);

      void append(std::string const &);
      bool operator<(const Path &) const;

      static char Delimiter;

      std::string str() const;

    private:
      std::vector<std::string> _split_path;
  };

  Document::Document(XMLDocument const & doc):
    _templates()
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
            if (attr->value().empty())
            {
              continue;
            }

            _templates[Path(attr->value())] = { element }; // FIXME this overwrites older templates
            break;
          }
        }
      }
    }
  }

  void Document::apply_style_to(XMLDocument const & xml, std::ostream & os)
  {
    Element const & root = *xml.root();
    Path path("/");
    auto it = _templates.find(path);
    if (it != _templates.end())
    {
      it->second.apply_to(root, os);
    }
    else
    {
      apply_style_to(root, path, os);
    }
  }

  void Document::apply_style_to(Element const & root, Path path, std::ostream & os)
  {
    auto it = _templates.find(path);
    path.append(root.name());
    if (it != _templates.end())
    {
      it->second.apply_to(root, os);
    }
    else
    {
      os << "template for " << path.str() << "not found" << std::endl;
      apply_children_style_to(root, path, os);
    }
  }

  void Document::apply_children_style_to(Element const & root, Path path, std::ostream & os)
  {
    for (auto child : root.children())
    {
      Element const *  child_element = dynamic_cast<Element *>(child);
      if (child_element == nullptr)
      {
        os << path.str() << "-> [content]" << std::endl;
        //os << child->str();
      }
      else
      {
        apply_style_to(*child_element, path, os);
      }
    }
  }

  void Template::apply_to(Element const & element, std::ostream & os) const
  {
    os << "Applying template to " << element.name() << std::endl;
  }

  char Path::Delimiter = '/';

  Path::Path(std::string const & str):
    _split_path(Helpers::split(str, Path::Delimiter))
  {
    // Enforce trailing slash
    if (_split_path.back() != "")
    {
      _split_path.push_back("");
    }
  }

  bool Path::operator<(const Path & other) const
  {
    if (_split_path.size() != other._split_path.size())
    {
      return _split_path.size() < other._split_path.size();
    }

    for (size_t i = 0; i < _split_path.size(); i++)
    {
      if (_split_path[i] != other._split_path[i])
      {
        return _split_path[i] < other._split_path[i];
      }
    }
    return false;
  }

  void Path::append(std::string const & str)
  {
    _split_path.insert(_split_path.end() - 1, str);
  }

  std::string Path::str() const
  {
    std::ostringstream oss;
    oss << Delimiter;
    for (auto path_part : _split_path)
    {
      oss << path_part << Delimiter;
    }
    return oss.str();
  }
}

void xml_apply_style(Document const & xml, Document const & xsl, std::ostream & os)
{
  Xsl::Document xsl_doc(xsl);
  xsl_doc.apply_style_to(xml, os);
}

// vim:ft=cpp et sw=2 sts=2:
