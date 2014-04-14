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

struct Node
{
  std::string reg_tag;
  std::string reg_attr;
  std::string reg_content;
};

static void xsd_schema(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);
static Node xsd_empty_element(Element * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);
static Node xsd_composite_element(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);
static Node xsd_complexType(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);
static Node xsd_sequence(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);
static Node xsd_choice(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types);

static std::string schema_to_regex(CompositeElement *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types);
static std::string element_to_regex(Element *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types);
static std::string simple_element_to_regex(Element *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types);
static std::string complexe_type_to_regex(CompositeElement *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types);
static std::string choice_to_regex(CompositeElement *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types);
static std::string sequence_to_regex(CompositeElement *, std::string,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types,
    bool mixed=false);

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
    std::map<std::string, Node> nodes;
    std::map<std::string, Node> _nodes;
    std::map<std::string, std::string> refs;
    std::map<std::string, std::string> _types;
    std::map<std::string, Node> types;
    std::string re("^" + re_prolog + schema_to_regex(root, xs_ns, _nodes, refs, _types) + re_Misc + "*$");
    // std::cout << re << std::endl; // Mais oui c'est clair
    // for(auto p : refs)
    // {
    //   std::cout << p.first << " : " << p.second << std::endl;
    // }

    xsd_schema(root, xs_ns, nodes, types);
    xsd_schema(root, xs_ns, nodes, types); // Super hack
    for(auto p : nodes)
    {
      std::cout << p.first << " : " << std::endl;
      std::cout << "Tags : " << p.second.reg_tag << std::endl;
      std::cout << "Attr : " << p.second.reg_attr << std::endl;
      std::cout << "Cont : " << p.second.reg_content << std::endl;
    }
    std::cout << std::endl;
    for(auto p : types)
    {
      std::cout << p.first << " : " << std::endl;
      std::cout << "Tags : " << p.second.reg_tag << std::endl;
      std::cout << "Attr : " << p.second.reg_attr << std::endl;
      std::cout << "Cont : " << p.second.reg_content << std::endl;
    }
    return re;
  }
  return "^$";
}

void xsd_schema(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  Node n;
  for (auto c : e->content())
  {
    Element * ie = dynamic_cast<Element *>(c);
    if (ie == nullptr)
      continue;

    if (ie->name() == xs_ns + ":element")
    {
      auto it_name = std::find_if(ie->attributes().begin(), ie->attributes().end(),
          [](Attribute * a) { return a->name() == "name"; });
      if (it_name != ie->attributes().end())
      {
        n.reg_tag += "<" + (*it_name)->value() + ">|";
      }

      CompositeElement * ce = dynamic_cast<CompositeElement *>(ie);
      if (ce == nullptr)
      {
        xsd_empty_element(ie, xs_ns, nodes, types);
      }
      else
      {
        xsd_composite_element(ce, xs_ns, nodes, types);
      }
    }
    if (ie->name() == xs_ns + ":complexType")
    {
      CompositeElement * ce = dynamic_cast<CompositeElement *>(ie);
      if (ce == nullptr)
        continue;
      xsd_complexType(ce, xs_ns, nodes, types);
    }
  }

  if (!n.reg_tag.empty())
    n.reg_tag = n.reg_tag.substr(0, n.reg_tag.length() - 1);
  nodes[""] = n;
}

Node xsd_empty_element(Element * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name == e->attributes().end())
    return Node();
  auto name = (*it_name)->value();

  auto it_type = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "type"; });
  if (it_type != e->attributes().end())
  {
    auto type = (*it_type)->value();
    if (type == xs_ns + ":string")
    {
      Node n;
      n.reg_content = re_string;
      nodes[name] = n;
      return n;
    }
    else if (type == xs_ns + ":date")
    {
      Node n;
      n.reg_content = re_date;
      nodes[name] = n;
      return n;
    }
    else
    {
      auto it = types.find(type);
      if (it != types.end())
      {
        Node n(it->second);
        nodes[name] = n;
        return n;
      }
    }
  }
  return Node();
}

Node xsd_composite_element(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  Node n;
  if (e->content().size() != 1)
    return n;
  CompositeElement * ce = dynamic_cast<CompositeElement *>(e->content().front());
  if (ce == nullptr)
    return n;
  if (ce->name() != xs_ns + ":complexType")
    return n;

  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name == e->attributes().end())
    return n;

  auto name = (*it_name)->value();
  n = xsd_complexType(ce, xs_ns, nodes, types);
  nodes[name] = n;
  return n;
}

Node xsd_complexType(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  Node n;
  if (e->content().size() != 1)
    return n;
  CompositeElement * ce = dynamic_cast<CompositeElement *>(e->content().front());
  if (ce == nullptr)
    return n;
  if (ce->name() == xs_ns + ":sequence")
  {
    n = xsd_sequence(ce, xs_ns, nodes, types);
  }
  else if (ce->name() == xs_ns + ":choice")
  {
    n = xsd_choice(ce, xs_ns, nodes, types);
  }

  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  if (it_name != e->attributes().end())
  {
    types[(*it_name)->value()] = n;
  }
  return n;
}

Node xsd_sequence(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  Node n;
  for (auto c : e->content())
  {
    Element * ie = dynamic_cast<Element *>(c);
    if (ie == nullptr)
      continue;

    if (ie->name() == xs_ns + ":element")
    {
      auto it_name = std::find_if(ie->attributes().begin(), ie->attributes().end(),
          [](Attribute * a) { return a->name() == "name"; });
      auto it_ref = std::find_if(ie->attributes().begin(), ie->attributes().end(),
          [](Attribute * a) { return a->name() == "ref"; });
      if (it_name != ie->attributes().end())
      {
        n.reg_tag += "<" + (*it_name)->value() + ">";

        CompositeElement * ce = dynamic_cast<CompositeElement *>(ie);
        if (ce == nullptr)
        {
          xsd_empty_element(ie, xs_ns, nodes, types);
        }
        else
        {
          xsd_composite_element(ce, xs_ns, nodes, types);
        }
      }
      else if (it_ref != ie->attributes().end())
      {
        auto ref = (*it_ref)->value();
        auto it = nodes.find(ref);
        if (it != nodes.end())
        {
          auto it_minOccurs = std::find_if(ie->attributes().begin(), ie->attributes().end(),
              [](Attribute * a) { return a->name() == "minOccurs"; });
          auto it_maxOccurs = std::find_if(ie->attributes().begin(), ie->attributes().end(),
              [](Attribute * a) { return a->name() == "maxOccurs"; });
          std::string min = "1";
          std::string max = "1";
          if (it_minOccurs != ie->attributes().end())
          {
            auto minOccurs = (*it_minOccurs)->value();
            min = minOccurs;
          }
          if (it_maxOccurs != ie->attributes().end())
          {
            auto maxOccurs = (*it_maxOccurs)->value();
            if (maxOccurs == "unbounded")
              max = "";
            else
              max = maxOccurs;
          }
          n.reg_tag += "<" + ref + ">{" + min + "," + max + "}";
        }
      }
    }
  }
  return n;
}

Node xsd_choice(CompositeElement * e, std::string const & xs_ns, std::map<std::string, Node> & nodes, std::map<std::string, Node> & types)
{
  Node n;
  for (auto c : e->content())
  {
    Element * ie = dynamic_cast<Element *>(c);
    if (ie == nullptr)
      continue;

    if (ie->name() == xs_ns + ":element")
    {
      auto it_name = std::find_if(ie->attributes().begin(), ie->attributes().end(),
          [](Attribute * a) { return a->name() == "name"; });
      if (it_name != ie->attributes().end())
      {
        n.reg_tag += "<" + (*it_name)->value() + ">|";
      }

      CompositeElement * ce = dynamic_cast<CompositeElement *>(ie);
      if (ce == nullptr)
      {
        xsd_empty_element(ie, xs_ns, nodes, types);
      }
      else
      {
        xsd_composite_element(ce, xs_ns, nodes, types);
      }
    }
  }
  if (!n.reg_tag.empty())
    n.reg_tag = n.reg_tag.substr(0, n.reg_tag.length() - 1);
  n.reg_tag = "(" + n.reg_tag + ")";
  return n;
}


std::string schema_to_regex(CompositeElement * s,
    std::string xs_ns,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  std::string r;
  for (auto c : s->content())
  {
    auto e = dynamic_cast<Element *>(c);
    if (e != nullptr && e->name() == xs_ns + ":element")
      r += element_to_regex(e, xs_ns, nodes, refs, types) + "|";

    auto ce = dynamic_cast<CompositeElement *>(c);
    if (ce != nullptr && ce->begin_tag() == xs_ns + ":complexType")
    {
      complexe_type_to_regex(ce, xs_ns, nodes, refs, types);
    }
  }
  r = r.substr(0, r.length() - 1);
  return "(" + r + ")";
}

std::string element_to_regex(Element * e,
    std::string xs_ns,
    std::map<std::string, Node> & nodes,
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
      std::string r(begin + complexe_type_to_regex(cce, xs_ns, nodes, refs, types) + end);
      refs[name] = r;
      return r;
    }
  }
  else
  {
    std::string r(begin + simple_element_to_regex(e, xs_ns, nodes, refs, types) + end);
    refs[name] = r;
    return r;
  }
  return "";
}

std::string simple_element_to_regex(Element * e,
    std::string xs_ns,
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> &,
    std::map<std::string, std::string> & types)
{
  auto it_name = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "name"; });
  auto name = (*it_name)->value();

  auto it_type = std::find_if(e->attributes().begin(), e->attributes().end(),
      [](Attribute * a) { return a->name() == "type"; });
  if (it_type == e->attributes().end())
    return "";

  auto type = (*it_type)->value();
  if (type == xs_ns + ":string")
  {
    Node n;
    n.reg_content = re_string;
    nodes[name] = n;
    return re_string;
  }
  else if (type == xs_ns + ":date")
  {
    Node n;
    n.reg_content = re_date;
    nodes[name] = n;
    return re_date;
  }

  auto it_ptype = types.find(type);
  if (it_ptype != types.end())
  {
    Node n;
    n.reg_tag = it_ptype->second;
    nodes[name] = n;
    return "(" + it_ptype->second + ")";
  }

  return "";
}

std::string complexe_type_to_regex(CompositeElement * e,
    std::string xs_ns,
    std::map<std::string, Node> & nodes,
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
    s += choice_to_regex(ce, xs_ns, nodes, refs, types);
    if (mixed)
      s += re_mixed;
  }
  else if (ce->begin_tag() == xs_ns + ":sequence")
  {
    s += sequence_to_regex(ce, xs_ns, nodes, refs, types, mixed);
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
    std::map<std::string, Node> & nodes,
    std::map<std::string, std::string> & refs,
    std::map<std::string, std::string> & types)
{
  std::string s;
  for (auto c : e->content())
  {
    auto ce = dynamic_cast<Element *>(c);
    if (ce)
    {
      s += element_to_regex(ce, xs_ns, nodes, refs, types);
      if (c != e->content().back())
        s += "|";
    }
  }
  return re_blank + "(" + s + ")" + re_blank;
}

std::string sequence_to_regex(CompositeElement * e,
    std::string xs_ns,
    std::map<std::string, Node> & nodes,
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
      std::string reg = element_to_regex(ce, xs_ns, nodes, refs, types) + space;
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

