%{

#include <stack>
#include <list>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "content.h"
#include "element.h"
#include "composite_element.h"
#include "char_data.h"
#include "cd_sect.h"
#include "doctype.h"
#include "attribute.h"
#include "doctype.h"
#include "document.h"
#include "prolog.h"
#include "pi.h"

using namespace std;

extern char xmltext[];

int xmllex(void);

void xmlerror(const char * msg)
{
   fprintf(stderr,"%s\n",msg);
}

%}

%union {
   char * s;
   Content * content;
   Element * element;
   PI * pi;
   Doctype * doctype;
   Document * document;
   std::vector<PI *> * pi_list;
   std::pair<Doctype *, std::vector<PI *> *> * doctype_pis;
   std::vector<Attribute *> * attribute_list;
   std::vector<Content *> * content_list;
}

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <s> VALEUR DONNEES COMMENT NOM CDATAEND

%type<document> document
%type<content> sub_element
%type<element> element
%type<pi> Misc PI
%type<doctype> doctypedecl
%type<pi_list> Misc_0_N
%type<doctype_pis> doctypedecl_Misc_0_N_0_1
%type<attribute_list> Attribute_0_N
%type<content_list> content

%%

document
  : Misc_0_N doctypedecl_Misc_0_N_0_1 element Misc_0_N
  {
    std::cout << "document" << std::endl;
    $$ = new Document(new Prolog(*$1, $2->first, *$2->second), $3, *$4);
    delete $1;
    delete $2;
    delete $4;
  }
  ;

Misc_0_N
  : Misc_0_N Misc
  {
    std::cout << "Misc_0_N" << std::endl;
    if ($2 != nullptr)
      $1->push_back($2);
    $$ = $1;
  }
  | /* empty */
  {
    std::cout << "Misc_0_N empty" << std::endl;
    $$ = new std::vector<PI *>();
  }
  ;

Misc
  : COMMENT
  {
    std::cout << "Misc - Comment" << std::endl;
    $$ = nullptr;
    free($1);
  }
  | PI
  {
    std::cout << "Misc - PI" << std::endl;
    $$ = $1;
  }
  ;

PI
  : INFSPECIAL NOM Attribute_0_N SUPSPECIAL
  {
    std::cout << "PI" << std::endl;
    $$ = new PI($2, *$3);
    delete $3;
    free($2);
  }
  ;

doctypedecl_Misc_0_N_0_1
  : doctypedecl Misc_0_N
  {
    std::cout << "doctypedecl_Misc_0_N_0_1" << std::endl;
    $$ = new pair<Doctype *, std::vector<PI *> *>($1, $2);
  }
  | /* empty */
  {
    std::cout << "doctypedecl_Misc_0_N_0_1 empty" << std::endl;
    $$ = new pair<Doctype *, std::vector<PI *> *>(nullptr, nullptr);
  }
  ;

doctypedecl
  : DOCTYPE NOM SUP
  {
    std::cout << "doctypedecl_0_N - 1" << std::endl;
    $$ = new Doctype($2, std::string(), std::string());
    free($2);
  }
  | DOCTYPE NOM NOM SUP
  {
    std::cout << "doctypedecl_0_N - 2" << std::endl;
    $$ = new Doctype($2, $3, "");
    free($2);
    free($3);
  }
  | DOCTYPE NOM NOM VALEUR SUP
  {
    std::cout << "doctypedecl_0_N - 3" << std::endl;
    $$ = new Doctype($2, $3, $4);
    free($2);
    free($3);
    free($4);
  }
  ;

Attribute_0_N
  : Attribute_0_N NOM EGAL VALEUR
  {
    std::cout << "Attribute_0_N" << std::endl;
    $1->push_back(new Attribute($2, $4));
    $$ = $1;
    free($2);
    free($4);
  }
  | /* empty */
  {
    std::cout << "Attribute_0_N empty" << std::endl;
    $$ = new std::vector<Attribute *>();
  }
  ;

element
  : INF NOM Attribute_0_N SLASH SUP
  {
    std::cout << "element - empty" << std::endl;
    $$ = new Element($2, *$3);
    free($2);
    delete $3;
  }
  | INF NOM Attribute_0_N SUP content INF SLASH NOM SUP
  {
    std::cout << "element - composite : " << "" << std::endl;
    // TODO check $2 == $7
    $$ = new CompositeElement($2, *$3, *$5);
    free($2);
    delete $3;
    delete $5;
    free($8);
  }
  ;

content
  : content sub_element
  {
    std::cout << "content" << std::endl;
    if($2 != nullptr)
      $1->push_back($2);
    $$ = $1;
  }
  | /* empty */
  {
    std::cout << "content empty" << std::endl;
    $$ = new std::vector<Content *>();
  }
  ;

sub_element
  : DONNEES
  {
    std::cout << "sub_element - DONNEES : " << $1 << std::endl;
    $$ = new CharData($1);
  }
  | element
  {
    std::cout << "sub_element - element : " << "" << std::endl;
    $$ = $1;
  }
  | CDATABEGIN CDATAEND
  {
    std::cout << "sub_element - CDSect : " << "" << std::endl;
    $$ = new CDSect($2);
  }
  | PI
  {
    std::cout << "sub_element - PI : " << "" << std::endl;
    $$ = $1;
  }
  | COMMENT
  {
    $$ = nullptr;
  }
  ;
