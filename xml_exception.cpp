#include "xml_exception.h"


std::ostringstream XmlException::xml_exception_oss;


XmlException::XmlException(const std::string & u_standard, const std::string & u_extra)
    : runtime_error( "XmlException" ), _standard( u_standard ), _extra( u_extra )
{}

XmlException::~XmlException() throw()
{

}



const std::string& XmlException::standard()
{
    return _standard;
}

const std::string& XmlException::extra()
{
    return _extra;
}

const char* XmlException::what() const throw()
{
    xml_exception_oss.str("");
    xml_exception_oss<<_standard<<": "<<_extra<<std::endl;

    return xml_exception_oss.str().c_str();
}
