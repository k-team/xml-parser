#ifndef XML_EXCEPTION_H
#define XML_EXCEPTION_H

#include <stdexcept>
#include <string>
#include <sstream>

class XmlException: public std::runtime_error
{
  public:
    XmlException(std::string const &, std::string const &);
    virtual ~XmlException() throw();

    std::string const & standard() const;
    std::string const & extra() const;
    const char * what() const throw();

  private:
    std::string _standard;
    std::string _extra;

    static std::ostringstream xml_exception_oss;
};

#endif // vim:ft=cpp et sw=2 sts=2:
