%{

#include <stack>
#include <list>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include "commun.h"

extern char xmltext[];

int xmllex(void);  

void xmlerror(const char * msg)
{
   fprintf(stderr,"%s\n",msg);
}

%}

%union {
   char * s;
}

%token EGAL SLASH SUP SUPSPECIAL DOCTYPE COLON INFSPECIAL INF CDATABEGIN
%token <s> VALEUR DONNEES COMMENT NOM CDATAEND

%%

document
  : Misc_0_N doctypedecl_Misc_0_N_0_1 element Misc_0_N
  {
    std::cout << "document" << std::endl;
  }
  ;

Misc_0_N
  : Misc_0_N Misc
  {
    std::cout << "Misc_0_N" << std::endl;
  }
  | /* empty */
  {
    std::cout << "Misc_0_N empty" << std::endl;
  }
  ;

Misc
  : COMMENT
  {
    std::cout << "Misc - Comment" << std::endl;
  }
  | PI
  {
    std::cout << "Misc - PI" << std::endl;
  }
  ;

PI
  : INFSPECIAL NOM Attribute_0_N SUPSPECIAL
  {
    std::cout << "PI" << std::endl;
  }
  ;

doctypedecl_Misc_0_N_0_1
  : doctypedecl Misc_0_N
  {
    std::cout << "doctypedecl_Misc_0_N_0_1" << std::endl;
  }
  | /* empty */
  {
    std::cout << "doctypedecl_Misc_0_N_0_1 empty" << std::endl;
  }
  ;

doctypedecl
  : DOCTYPE NOM SUP
  {
    std::cout << "doctypedecl_0_N - 1" << std::endl;
  }
  | DOCTYPE NOM NOM SUP
  {
    std::cout << "doctypedecl_0_N - 2" << std::endl;
  }
  | DOCTYPE NOM NOM VALEUR SUP
  {
    std::cout << "doctypedecl_0_N - 3" << std::endl;
  }
  ;

Attribute_0_N
  : Attribute_0_N NOM EGAL VALEUR
  {
    std::cout << "Attribute_0_N" << std::endl;
  }
  | /* empty */
  {
    std::cout << "Attribute_O_N empty" << std::endl;
  }
  ;

element
  : INF NOM Attribute_0_N SLASH SUP
  {
    std::cout << "element - empty" << std::endl;
  }
  | INF NOM Attribute_0_N SUP content INF SLASH NOM SUP
  {
    std::cout << "element - composite : " << "" << std::endl;
  }
  ;

content
  : content sub_element
  {
    std::cout << "content" << std::endl;
  }
  | /* empty */
  {
    std::cout << "content empty" << std::endl;
  }
  ;

sub_element
  : DONNEES
  {
    std::cout << "sub_element - DONNEES : " << $1 << std::endl;
  }
  | element
  {
    std::cout << "sub_element - element : " << "" << std::endl;
  }
  | CDATABEGIN CDATAEND
  {
    std::cout << "sub_element - CDSect : " << "" << std::endl;
  }
  | PI
  {
    std::cout << "sub_element - PI : " << "" << std::endl;
  }
  | COMMENT
  {
    std::cout << "sub_element - COMMENT" << std::endl;
  }
  ;
