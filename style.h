#ifndef STYLE_H
#define STYLE_H

#include <ostream>


class Document;
class Element;

void xml_apply_style(Document const &, Document const &, std::ostream &);

int check_xsl(const Element &root);
void check_xsl_tags_level(const Element &root, std::ostream & os);
void check_xsl_lowers_tags_level(const Element &root, std::ostream & os);
void check_xsl_multiples_apply_all(const Element &root, std::ostream & os, int & count);

#endif // vim:ft=cpp et sw=2 sts=2:

