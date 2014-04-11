#include "regex.h"

#include <iostream>
#include <algorithm>
#include <map>

#include "document.h"
#include "prolog.h"
#include "pi.h"
#include "doctype.h"
#include "attribute.h"
#include "element.h"
#include "composite_element.h"
#include "content.h"

static std::string schema_to_regex(CompositeElement *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types);
static std::string element_to_regex(Element *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types);
static std::string simple_element_to_regex(Element *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types);
static std::string complexe_type_to_regex(CompositeElement *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types);
static std::string choice_to_regex(CompositeElement *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types);
static std::string sequence_to_regex(CompositeElement *, std::string,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types, bool mixed=false);

static const std::string re_string("[^<]*");
static const std::string re_date("[0-9]{4}-[0-9]{2}-[0-9]{2}(((\\+|-)[0-2][0-9]:[0-5][0-9])|Z)?");
static const std::string re_mixed("[^<]*");
static const std::string re_blank("\\s*");
static const std::string re_S("(\\s+)");
static const std::string re_Eq(re_S + "?=" + re_S + "?");
static const std::string re_Name("[:A-Za-z\\200-\\377_][:A-Za-z\\200-\\377_0-9.-]*");
static const std::string re_AttValue("(\"[^<&\"]*\"|\'[^<&\"]*\')");
static const std::string re_Attr(re_Name + re_Eq + re_AttValue);
static const std::string re_version_info(re_S + "version" + re_Eq + "(\"1\\.[0-9]+\"|'1\\.[0-9]+')");
static const std::string re_encoding_decl(re_S + "encoding" + re_Eq + re_AttValue);
static const std::string re_sd_decl(re_S + re_Attr);
static const std::string re_xml_decl("<\\?xml(" +
    re_version_info + ")?(" +
    re_encoding_decl + ")?(" +
    re_sd_decl + ")? ?\\?>");
static const std::string re_comment("<!--([^-]|-[^-])*-->");
static const std::string re_PI("<\\?" + re_Name + "(" + re_S + re_Attr + ")*\\?>");
static const std::string re_Misc("(" + re_comment + "|" + re_PI + "|" + re_S + ")");
static const std::string re_doctype_decl("<!DOCTYPE" + re_S + re_Name + "(" + re_S + re_Name + ")?(" + re_S + re_AttValue + ")?" + re_S + "?>");
static const std::string re_prolog("(" + re_xml_decl + ")?" + re_Misc + "*(" + re_doctype_decl + re_Misc + "*)?");

std::string xsd_to_regex(Document * doc)
{
  // std::cout << "xml : " << re_xml_decl << std::endl;
  // std::cout << "doc : " << re_doctype_decl << std::endl;
  // std::cout << "com : " << re_comment << std::endl;
  // std::cout << "pi  : " << re_PI << std::endl;
  // std::cout << "pro : " << re_prolog << std::endl;
  CompositeElement * root = dynamic_cast<CompositeElement *>(doc->root());
  if (root == nullptr)
    return "^$";

  auto it_ns = std::find_if(root->attributes().begin(), root->attributes().end(),
      [](Attribute * a) { return a->name().substr(0, 5) == "xmlns" && a->value().substr(a->value().length() - 9, 9) == "XMLSchema"; });
  if (it_ns == root->attributes().end())
    return "^$";

  std::string xs_ns = (*it_ns)->name().substr(6);
  if (root->begin_tag() == xs_ns + ":schema")
  {
    std::map<std::string, std::string> refs;
    std::map<std::string, std::string> types;
    std::string re("^" + re_prolog + schema_to_regex(root, xs_ns, refs, types) + re_Misc + "*$");
    std::cout << re << std::endl; // Mais oui c'est clair
    return re;
  }
  return "^$";
}

std::string schema_to_regex(CompositeElement * s,
    std::string xs_ns,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  std::string r;
  for (auto c : s->content())
  {
    auto e = dynamic_cast<Element *>(c);
    if (e != nullptr && e->name() == xs_ns + ":element")
      r += element_to_regex(e, xs_ns, refs, types) + "|";
  }
  r = r.substr(0, r.length() - 1);
  return "(" + r + ")";
}

std::string element_to_regex(Element * e,
    std::string xs_ns,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name == e->attributes().end())
  {
    auto it_ref = std::find_if(e->attributes().begin(), e->attributes().end(),
        [](Attribute * a) { return a->name() == "ref"; });
    if (it_ref == e->attributes().end())
      return "";

    auto ref = (*it_ref)->value();
    auto r = refs.find(ref);
    if (r == refs.end())
      return "";
    return r->second;
  }

  auto name = (*it_name)->value();
  const std::string begin("<" + name + "(" + re_S + re_Attr + ")*" + re_blank + ">");
  const std::string end("</" + name + re_blank + ">");

  auto ce = dynamic_cast<CompositeElement *>(e);
  if (ce != nullptr)
  {
    CompositeElement * cce = nullptr;
    if (ce->content().size() >= 1
        && (cce = dynamic_cast<CompositeElement *>(ce->content().front())) != nullptr
        && cce->begin_tag() == xs_ns + ":complexType")
    {
      std::string r(begin + complexe_type_to_regex(cce, xs_ns, refs, types) + end);
      refs[name] = r;
      return r;
    }
  }
  else
  {
    std::string r(begin + simple_element_to_regex(e, xs_ns, refs, types) + end);
    refs[name] = r;
    return r;
  }
  return "";
}

std::string simple_element_to_regex(Element * e,
    std::string xs_ns,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types)
{
  auto it_type = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "type"; });
  if (it_type == e->attributes().end())
    return "";

  auto type = (*it_type)->value();
  if (type == xs_ns + ":string")
    return re_string;
  else if (type == xs_ns + ":date")
    return re_date;
  return "";
}

std::string complexe_type_to_regex(CompositeElement * e,
    std::string xs_ns,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  if (e->content().size() < 1)
    return "";

  CompositeElement * ce = dynamic_cast<CompositeElement *>(e->content().front());
  if (ce == nullptr)
    return "";

  bool mixed = false;
  auto it_mixed = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "mixed"; });
  if (it_mixed != e->attributes().end()
      && (*it_mixed)->value() == "true")
    mixed = true;

  std::string s;
  if (ce->begin_tag() == xs_ns + ":choice")
  {
    if (mixed)
      s += re_mixed;
    s += choice_to_regex(ce, xs_ns, refs, types);
    if (mixed)
      s += re_mixed;
  }
  else if (ce->begin_tag() == xs_ns + ":sequence")
  {
    s += sequence_to_regex(ce, xs_ns, refs, types, mixed);
  }


  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name != e->attributes().end())
  {
    types[(*it_name)->value()] = s;
  }

  return s;
}

std::string choice_to_regex(CompositeElement * e,
    std::string xs_ns,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  std::string s;
  for (auto c : e->content())
  {
    auto ce = dynamic_cast<Element *>(c);
    if (ce)
    {
      s += element_to_regex(ce, xs_ns, refs, types);
      if (c != e->content().back())
        s += "|";
    }
  }
  return re_blank + "(" + s + ")" + re_blank;
}

std::string sequence_to_regex(CompositeElement * e,
    std::string xs_ns,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types, bool mixed)
{
  std::string s;
  std::string space = mixed ? re_mixed : re_blank;
  for (auto c : e->content())
  {
    auto ce = dynamic_cast<Element *>(c);
    if (ce)
    {
      auto it_maxOccurs = std::find_if(ce->attributes().begin(),
          ce->attributes().end(),
          [](Attribute * a) { return a->name() == "maxOccurs"; });
      std::string reg = element_to_regex(ce, xs_ns, refs, types) + space;
      if (it_maxOccurs != ce->attributes().end())
      {
        int maxOccurs = atoi((*it_maxOccurs)->value().c_str());
        for (int i(0); i < maxOccurs; i++) {
          s += reg;
        }
      }
      else
      {
        s += reg;
      }
    }
  }
  return space + s;
}

