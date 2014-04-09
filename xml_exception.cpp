#include "xml_exception.h"

std::ostringstream XmlException::xml_exception_oss;

XmlException::XmlException(const std::string & standard,
    const std::string & extra):
  std::runtime_error("XmlException"),
  _standard(standard), _extra(extra)
{
}

XmlException::~XmlException() throw()
{
}

std::string const & XmlException::standard() const
{
  return _standard;
}

std::string const & XmlException::extra() const
{
  return _extra;
}

const char * XmlException::what() const throw()
{
  xml_exception_oss.str("");
  xml_exception_oss << _standard << " " << _extra;
  return xml_exception_oss.str().c_str();
}

// vim:ft=cpp et sw=2 sts=2:
