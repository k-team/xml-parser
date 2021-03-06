#include "composite_element.h"
#include "content.h"
#include "attribute.h"
#include "helpers.h"
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
  oss << begin_str() << std::endl;
  for (auto c : _content)
  {
    std::string s = c->str();
    oss << s << std::endl;
  }
  oss << end_str();
  return oss.str();
}

std::string CompositeElement::begin_str() const
{
  std::ostringstream oss;
  oss << "<" << name();
  for (auto attr : attributes())
  {
    oss << " " << attr->str();
  }
  oss << ">";
  return oss.str();
}

std::string CompositeElement::end_str() const
{
  return "</" + name() + ">";
}

std::string const & CompositeElement::begin_tag() const
{
  return name();
}

std::string const & CompositeElement::end_tag() const
{
  return _end;
}

void CompositeElement::to_be_or_not_to_be(std::ostream & os) const
{
  Element::to_be_or_not_to_be(os);
  for (auto c : _content)
  {
    c->to_be_or_not_to_be(os);
  }

  using Helpers::split;

  // Begin namespace must be single
  std::vector<std::string> begin_ns_split = split(begin_tag(), ':');

  // End namespace must be single
  std::vector<std::string> end_ns_split = split(end_tag(), ':');
  if (begin_ns_split.size() > 2)
  {
    os << "Namespace specified more than once. Given "
      << begin_ns_split.size() - 1 << " times" << std::endl;
  }
  else if (end_ns_split.size() > 2)
  {
    os << "Namespace specified more than once. Given "
      << end_ns_split.size() - 1 << " times" << std::endl;
  }

  // Namespace, if specified, must be the same
  if (begin_ns_split.size() > 1)
  {
    if (end_ns_split.size() == 1)
    {
      os << "Missing closing namespace " << begin_ns_split[0] << std::endl;
    }
    else if (!end_tag().empty())
    {
      if (begin_ns_split[0] != end_ns_split[0])
      {
        os << "Non matching element namespaces "
          << begin_ns_split[0] << " and " << end_ns_split[0] << std::endl;
      }
      if (begin_ns_split[1] != end_ns_split[1])
      {
        os << "Non matching element names "
          << begin_ns_split[1] << " and " << end_ns_split[1] << std::endl;
      }
    }
  }
  else if (begin_tag() != end_tag() && !end_tag().empty()) // Begin/End must be the same
  {
    os << "Non matching element names " << begin_tag()
      << " and " << end_tag() << std::endl;
  }
}

CompositeElement::child_list CompositeElement::children() const
{
  return _content;
}

// vim:ft=cpp et sw=2 sts=2:
