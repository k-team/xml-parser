#ifndef XML_EXCEPTION_H
#define XML_EXCEPTION_H


#include <stdexcept>
#include <string>
#include <sstream>


class XmlException: public std::runtime_error {
  public:
    XmlException(const std::string& , const std::string& u_extra);
    virtual ~XmlException() throw();


    const std::string& standard();
    const std::string& extra();
    virtual const char* what() const throw();

  private:
    std::string _standard;
    std::string _extra;

    static std::ostringstream xml_exception_oss;
};


#endif
