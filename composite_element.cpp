#include "composite_element.h"
#include "content.h"
#include "attribute.h"
#include "xml_exception.h"
#include <sstream>

CompositeElement::CompositeElement(std::string const & name,
    std::string const & end,
    Element::attribute_list const & attrs,
    CompositeElement::content_list const & content):
  Element(name, attrs),
  _end(end),
  _content(content)
{
}

CompositeElement::~CompositeElement()
{
  for (auto c : _content)
  {
    delete c;
  }
}

CompositeElement::content_list const & CompositeElement::content() const
{
  return _content;
}

std::string CompositeElement::str() const
{
  std::ostringstream oss;
  oss << "<" << name();
  for (auto attr : attributes())
  {
    oss << " " << attr->str();
  }
  oss << ">\n";
  for (auto c : _content)
  {
    std::string s = c->str();
    // Add space just after every \n
    // size_t from = 0;
    // size_t pos;
    // while ((pos = s.find("\n", from)) != std::string::npos)
    // {
    //   s.replace(pos, 1, "\n  ");
    //   from = pos + 3;
    // }
    // oss << "  " << s << "\n";
    oss << s << "\n";
  }
  oss << "</" << name() << ">";
  return oss.str();
}

std::string const & CompositeElement::begin_tag() const
{
  return name();
}

std::string const & CompositeElement::end_tag() const
{
  return _end;
}

std::vector<std::string> &split(const std::string & s, char delim, std::vector<std::string> & elems)
{
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim))
  {
    elems.push_back(item);
  }
  return elems;
}

std::vector<std::string> split(const std::string & s, char delim)
{
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}

void CompositeElement::to_be_or_not_to_be() const
{
  Element::to_be_or_not_to_be();
  for (auto c : _content)
  {
    c->to_be_or_not_to_be();
  }

  // Begin namespace must be single
  std::vector<std::string> begin_ns_split = split(begin_tag(), ':');
  if (begin_ns_split.size() > 2)
  {
    throw XmlException("Namespace specified more than once.",
        "Given " + std::to_string(begin_ns_split.size()) + " times");
  }

  // End namespace must be single
  std::vector<std::string> end_ns_split = split(end_tag(), ':');
  if (end_ns_split.size() > 2)
  {
    throw XmlException("Namespace specified more than once",
        "given " + std::to_string(end_ns_split.size()) + " times");
  }

  // Namespace, if specified, must be the same
  if (begin_ns_split.size() > 1)
  {
    if (end_ns_split.size() == 1)
    {
      throw XmlException("Missing closing namespace", begin_ns_split[0]);
    }
    else if (begin_ns_split[0] != end_ns_split[0])
    {
      throw XmlException("Non matching element namespaces",
          begin_ns_split[0] + " and " + end_ns_split[0]);
    }
    else if (begin_ns_split[1] != end_ns_split[1])
    {
      throw XmlException("Non matching element names",
          begin_ns_split[1] + " and " + end_ns_split[1]);
    }
  }

  // Begin/End must be the same
  if (begin_tag() != end_tag())
  {
    throw XmlException("Non matching element names",
        begin_tag() + " and " + end_tag());
  }
}

// vim:ft=cpp et sw=2 sts=2:
