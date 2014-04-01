#ifndef DOCUMENT_H
#define DOCUMENT_H

class Document;
class Element;
class PI;

class Document {
  public:
    Document(Prolog *, Element *, PI *);
    virtual ~Document();
};

#endif
