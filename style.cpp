#include "style.h"
#include "document.h"
#include <iostream>

void xml_apply_style(Document const & xml, Document const & xsl)
{
    std::cout << "Applying stylesheet " << xsl.str()
        << " to document " << xml.str() << std::endl;
}
