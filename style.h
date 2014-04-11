#ifndef STYLE_H
#define STYLE_H

#include <ostream>


class Document;
class Element;

void xml_apply_style(Document const &, Document const &, std::ostream &);

int test_xsl(const Element &root);

#endif // vim:ft=cpp et sw=2 sts=2:

