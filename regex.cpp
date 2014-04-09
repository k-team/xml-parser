#include "regex.h"

#include <iostream>
#include <algorithm>

#include "document.h"
#include "prolog.h"
#include "pi.h"
#include "doctype.h"
#include "attribute.h"
#include "element.h"
#include "composite_element.h"
#include "content.h"

static std::string schema_to_regex(CompositeElement *);
static std::string element_to_regex(Element *);
static std::string simple_element_to_regex(Element *);
static std::string complexe_type_to_regex(CompositeElement *);
static std::string choice_to_regex(CompositeElement *);
static std::string sequence_to_regex(CompositeElement *);
static std::string string_to_regex();
static std::string date_to_regex();
static std::string mixed_to_regex();
static std::string blank_to_regex();

std::string xsl_to_regex(Document * doc)
{
  CompositeElement * root = dynamic_cast<CompositeElement *>(doc->root());
  if (root == nullptr)
    return "^$";
  if (root->begin_tag() == "xsd:schema")
    return "^" + schema_to_regex(root) + "$";
  return "^$";
}

std::string schema_to_regex(CompositeElement * s)
{
  std::string r;
  for (auto c : s->content())
  {
    auto e = dynamic_cast<Element *>(c);
    if (e != nullptr && e->name() == "xsd:element")
      r += element_to_regex(e);
  }
  return r;
}

std::string element_to_regex(Element * e)
{
  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name == e->attributes().end())
    return "";

  auto name = (*it_name)->value();

  auto ce = dynamic_cast<CompositeElement *>(e);
  if (ce != nullptr)
  {
    CompositeElement * cce = nullptr;
    if (ce->content().size() >= 1
        && (cce = dynamic_cast<CompositeElement *>(ce->content().front())) != nullptr
        && cce->begin_tag() == "xsd:complexType")
    {
      return "<" + name + ">\\s?" + complexe_type_to_regex(cce) + "</" + name + ">\\s?";
    }
  }
  else
  {
    return "<" + name + "\\s?>" + simple_element_to_regex(e) + "</" + name + "\\s?>";
  }
  return "";
}

std::string simple_element_to_regex(Element * e)
{
  auto it_type = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "type"; });
  if (it_type == e->attributes().end())
    return "";

  auto type = (*it_type)->value();
  if (type == "xsd:string")
    return string_to_regex();
  else if (type == "xsd:date")
    return date_to_regex();
  return "";
}

std::string complexe_type_to_regex(CompositeElement * e)
{
  if (e->content().size() < 1)
    return "";

  CompositeElement * ce = dynamic_cast<CompositeElement *>(e->content().front());
  if (ce == nullptr)
    return "";

  std::string s;
  if (ce->begin_tag() == "xsd:choice")
  {
    s += choice_to_regex(ce);
  }
  else if (ce->begin_tag() == "xsd:sequence")
  {
    s += sequence_to_regex(ce);
  }
  return s;
}

std::string choice_to_regex(CompositeElement * e)
{
  std::string s;
  for (auto c : e->content())
  {
    auto ce = dynamic_cast<Element *>(c);
    if (ce)
    {
      s += element_to_regex(ce);
      if (c != e->content().back())
        s += "|";
    }
  }
  return blank_to_regex() + "(" + s + ")" + blank_to_regex();
}

std::string sequence_to_regex(CompositeElement * e)
{
  std::string s;
  for (auto c : e->content())
  {
    auto ce = dynamic_cast<Element *>(c);
    if (ce)
    {
      s += element_to_regex(ce) + blank_to_regex();
    }
  }
  return blank_to_regex() + s;
}

std::string string_to_regex()
{
  return "[^<]*";
}

std::string date_to_regex()
{
  return "[0-9]{4}-[0-9]{2}-[0-9]{2}(((\\+|-)[0-2][0-9]:[0-5][0-9])|Z)?";
}

std::string mixed_to_regex()
{
  return "[^<]*";
}

std::string blank_to_regex()
{
  return "\\s*";
}
