#ifndef STYLE_H
#define STYLE_H

#include <ostream>
#include <vector>

class Document;
class Element;

void xml_apply_style(Document const &, Document const &, std::ostream &);

int check_xsl(const Element &root);
void check_xsl_tags_level(const Element &root, std::ostream & os);
void check_xsl_lowers_tags_level(const Element &root, std::ostream & os);
void check_xsl_multiples_apply_all(const Element &root, std::ostream & os, int & count);
void check_xsl_apply_template_select(const Element &root, std::ostream & os);
void get_all_apply_template_select(const Element &root, std::vector<std::string> & vect_select);

#endif // vim:ft=cpp et sw=2 sts=2:

