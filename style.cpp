#include "style.h"
#include "document.h"
#include "element.h"
#include "composite_element.h"
#include "helpers.h"
#include "composite_element.h"
#include "attribute.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>



#define XSL_NS "xsl"
#define XSL_TEMPLATES "template"
#define XSL_MATCH "match"
#define XSL_ROOT_MATCH "/"
#define XSL_STYLESHEET "stylesheet"
#define XSL_APPLY_TEMPLATES "apply-templates"
#define XSL_VALUE_OF "value-of"
#define XSL_FOR_EACH "for-each"
#define XSL_SELECT "select"

namespace Xsl
{
  typedef ::Document XMLDocument;

  bool is_element(Element const &, std::string const & = "");

  class Document
  {
    public:
      Document(XMLDocument const &);

      void apply_style_to(XMLDocument const &, std::ostream &) const;

    private:
      void handle_e(Element const &, Element const &, std::ostream &) const;
      void handle_ce(CompositeElement const &, Element const &, std::ostream &) const;
      void handle_xsl(Element const &, Element const &, std::ostream &) const;
      void handle_apply_templates(Element const &, Element const &, std::ostream &) const;
      void handle_for_each(Element const &, Element const &, std::ostream &) const;
      void handle_value_of(Element const &, Element const &, std::ostream &) const;
      void handle_apply_all_templates(Content const *, std::ostream &) const;

      CompositeElement const * _root_template;
      std::map<std::string, Element const *> _templates;
  };

  // Implementation

  bool is_element(Element const & element, std::string const & tag)
  {
    auto ns_split = element.ns_split();

    // F**king lower() method
    std::transform(ns_split.first.begin(), ns_split.first.end(),
        ns_split.first.begin(), ::tolower);
    std::transform(ns_split.second.begin(), ns_split.second.end(),
        ns_split.second.begin(), ::tolower);

    // Only xsl namespace
    return ns_split.first == XSL_NS && (tag.empty() || ns_split.second == tag);
  }

  Document::Document(XMLDocument const & doc):
    _root_template(), _templates()
  {
    for (auto child : doc.root()->children())
    {
      Element * element = dynamic_cast<Element *>(child);
      if (element != nullptr && is_element(*element, XSL_TEMPLATES))
      {
        for (auto attr : element->attributes())
        {
          if (attr->name() == XSL_MATCH && !attr->value().empty())
          {
            if (attr->value() == "/")
            {
              _root_template = dynamic_cast<CompositeElement const *>(element);
            }
            else
            {
              _templates[attr->value()] = element;
            }
            break; // because f**k you that's why !
          }
        }
      }
    }
  }

  void Document::apply_style_to(XMLDocument const & xml, std::ostream & os) const
  {
    handle_ce(*_root_template, *xml.root(), os);
  }

  void Document::handle_e(Element const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    if (is_element(template_element))
    {
      handle_xsl(template_element, root_element, os);
    }
    else
    {
      auto ce = dynamic_cast<CompositeElement const *>(&template_element);
      if (ce == nullptr)
      {
        os << template_element.str() << std::endl;
      }
      else
      {
        os << ce->begin_str() << std::endl;
        handle_ce(*ce, root_element, os);
        os << ce->end_str() << std::endl;
      }
    }
  }

  void Document::handle_ce(CompositeElement const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    for (auto child : template_element.children())
    {
      auto child_element = dynamic_cast<Element const *>(child);
      if (child_element != nullptr)
      {
        handle_e(*child_element, root_element, os);
      }
      else
      {
        os << child->str() << std::endl;
      }
    }
  }

  void Document::handle_xsl(Element const & template_element,
      Element const & element, std::ostream & os) const
  {
    std::string xsl_operation = template_element.ns_split().second;

    // F**king lower() method
    std::transform(xsl_operation.begin(), xsl_operation.end(),
        xsl_operation.begin(), ::tolower);

    // Very elegant code
    if (xsl_operation == XSL_APPLY_TEMPLATES)
    {
      handle_apply_templates(template_element, element, os);
    }
    else if (xsl_operation == XSL_FOR_EACH)
    {
      handle_for_each(template_element, element, os);
    }
    else if (xsl_operation == XSL_VALUE_OF)
    {
      handle_value_of(template_element, element, os);
    }
    else
    {
      os << template_element.str() << std::endl;
    }
  }

  void Document::handle_apply_templates(Element const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    Attribute * select_attr = template_element.find_attribute(XSL_SELECT);
    if (select_attr != nullptr)
    {
      auto it =_templates.find(select_attr->value());
      if (it != _templates.end())
      {
        auto ce = dynamic_cast<CompositeElement const *>(it->second);
        handle_ce(*ce, root_element, os);
      }
    }
    else // Output ALL the templates!
    {
      for (auto c : root_element.children())
      {
        handle_apply_all_templates(c, os);
      }
    }
    //os << XSL_APPLY_TEMPLATES << std::endl;
  }

  void Document::handle_apply_all_templates(Content const * c, std::ostream & os) const
  {
    auto e = dynamic_cast<Element const *>(c);
    if (e == nullptr)
    {
      return;
    }

    // Apply applicable templates
    auto it = _templates.find(e->name());
    if (it !=_templates.end())
    {
      auto ce = dynamic_cast<CompositeElement const *>(it->second);
      handle_ce(*ce, *e, os);
    }
    else // Recursion otherwise
    {
      for (auto c : e->children())
      {
        handle_apply_all_templates(c, os);
      }
    }
  }

  void Document::handle_for_each(Element const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    os << XSL_FOR_EACH << std::endl;
  }

  void Document::handle_value_of(Element const & template_element,
      Element const & root_element, std::ostream & os) const
  {
    Attribute const & select_attr = *template_element.find_attribute(XSL_SELECT);
    auto apply = [&](CompositeElement const & ce)
    {
      for (auto child : ce.children())
      {
        os << child->str() << std::endl;
      }
    };

    if (select_attr.value() == ".")
    {
      auto ce = dynamic_cast<CompositeElement const *>(&root_element);
      if (ce != nullptr)
      {
        apply(*ce);
      }
    }
    else
    {
      for (auto c : root_element.children())
      {
        auto ce = dynamic_cast<CompositeElement const *>(c);
        if (ce != nullptr && ce->name() == select_attr.value())
        {
          apply(*ce);
          break; // TODO only first one is written
        }
      }
    }
  }
}

void xml_apply_style(Document const & xml, Document const & xsl, std::ostream & os)
{
  Xsl::Document xsl_doc(xsl); // FIXME check for xsl:stylesheet root element
  xsl_doc.apply_style_to(xml, os);
}


int check_xsl(const Element & root)
{
 /*   //test presence of stylesheet on level 1
    if (root.name()!="xsl:stylesheet")
    {
        return 1;
    }

    bool root_template_exist=false;

    for (auto it: root.children())
    {
        CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
        if (ce!=nullptr)
        {
            //test presence of things different from a template on level 2
            if (ce->name()!="xsl:template"){
                return 2;
            } else {
                //test root presence and multiples roots
                if (std::any_of(ce->attributes().begin(), ce->attributes().end(),
                            [](Attribute* attr){return (attr->name()=="match")&&(attr->value()=="/");}))
                {
                    if (!root_template_exist){
                        root_template_exist = true;
                    }else{
                        return 4;
                    }
                }
                else
                {
                    //test chemin absolu
                    if(std::any_of(ce->attributes().begin(), ce->attributes().end(),
                            [](Attribute* attr){return (attr->name()=="match")&&(attr->value()!="/")
                                &&(attr->value()[0]=='/');})){
                        return 5;
                    }
                }
            }
        }else{
            //this would indicate that the is smth that differs from composite element so it can not be a template
            return 2;
        }
    }

    if (!root_template_exist){
        //root does not exist
        return 3;
    }

    //passed all tests
    return 0;*/

    check_xsl_tags_level(root,std::cout);

    int count_apply_all=0;
    check_xsl_multiples_apply_all(root,std::cout,count_apply_all);
    if (count_apply_all>1){
        std::cout<<"multiples apply all templates"<<count_apply_all<<std::endl;
    }

    check_xsl_apply_template_select(root,std::cout);


    return 0;
}


// vim:ft=cpp et sw=2 sts=2:


void check_xsl_tags_level(const Element &root, std::ostream & os)
{

    std::string xsl_operation_root = root.ns_split().second;
    //test presence of stylesheet on level 1
    if (xsl_operation_root!=XSL_STYLESHEET)
    {
        os<<"xsl level 1 tag incorrect"<<std::endl;
    }


    for (auto it: root.children())
    {
        CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
        if (ce!=nullptr)
        {

            std::string xsl_operation = ce->ns_split().second;
            //test presence of things different from a template on level 2
            if (xsl_operation!=XSL_TEMPLATES){
                os<<"xsl level 2 tag incorrect"<<std::endl;
            }else{
                check_xsl_lowers_tags_level(*ce,os);
            }
        }else{
            //this would indicate that the is smth that differs from composite element so it can not be a template
            os<<"xsl level 2 tag incorrect"<<std::endl;
        }
    }


}


void check_xsl_lowers_tags_level(const Element &root, std::ostream &os)
{
    for (auto it: root.children())
    {
        CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
        if (ce!=nullptr)
        {

            std::string xsl_operation = ce->ns_split().second;

            //test presence of things different from a template on level 2
            if ((xsl_operation==XSL_TEMPLATES)||(xsl_operation==XSL_STYLESHEET)){
                os<<"xsl lower level tag incorrect"<<std::endl;
            }else{
                check_xsl_lowers_tags_level(*ce,os);
            }
        }else{

        }
    }
}


void check_xsl_multiples_apply_all(const Element &root, std::ostream &os, int &count)
{
    for (auto it: root.children())
    {
        CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
        if (ce!=nullptr)
        {
            check_xsl_multiples_apply_all(*ce,os,count);
        }else{
            Element * e = dynamic_cast<Element *>(it);
            if (e!=nullptr){
                std::string xsl_operation = e->ns_split().second;
                if ((xsl_operation==XSL_APPLY_TEMPLATES)&&(!std::any_of(e->attributes().begin(), e->attributes().end(),
                                [](Attribute* attr){return (attr->name()==XSL_SELECT);}))){
                    count++;
                }
            }
        }
    }
}


void check_xsl_apply_template_select(const Element &root, std::ostream &os)
{

    std::vector<std::string> vect_select;
    get_all_apply_template_select(root,vect_select);

    for (int i=0; i<vect_select.size(); ++i){
        os<<vect_select[i]<<std::endl;
        for (auto it: root.children())
        {
            CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
            if (ce!=nullptr)
            {
                for (auto it: ce->attributes()){
                    if (it->name()==XSL_MATCH){

                    }
                }
            }
        }
    }

}


void get_all_apply_template_select(const Element &root, std::vector<std::string> &vect_select)
{
    for (auto it: root.children())
    {
        CompositeElement * ce = dynamic_cast<CompositeElement *>(it);
        if (ce!=nullptr)
        {
            get_all_apply_template_select(*ce,vect_select);
        }else{
            Element * e = dynamic_cast<Element *>(it);
            if (e!=nullptr){
                std::string xsl_operation = e->ns_split().second;
                if (xsl_operation==XSL_APPLY_TEMPLATES){
                    for (auto it: e->attributes()){
                        if (it->name()==XSL_SELECT){
                            vect_select.push_back(it->value());
                        }
                    }
                }
            }
        }
    }
}
