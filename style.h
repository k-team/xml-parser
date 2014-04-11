#ifndef STYLE_H
#define STYLE_H

#include <ostream>

class Document;

void xml_apply_style(Document const &, Document const &, std::ostream &);

#endif // vim:ft=cpp et sw=2 sts=2:
