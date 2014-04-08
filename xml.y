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
  : Misc_0_N doctypedecl_0_N element Misc_0_N           { std::cout << "document" << std::endl; }
  ;

doctypedecl_0_N
  : DOCTYPE Misc_0_N                                    { std::cout << "doctypedecl_0_N" << std::endl; }
  | /* empty */
  ;

Misc_0_N
  : Misc_0_N Misc                                       { std::cout << "Misc_0_N" << std::endl; }
  | /* empty */
  ;

Misc
  : COMMENT                                             { std::cout << "Misc - Comment" << std::endl; }
  | PI                                                  { std::cout << "Misc - PI" << std::endl; }
  ;

PI
  : INFSPECIAL NOM DONNEES SUPSPECIAL                   { std::cout << "PI" << std::endl; }
  ;


Attribute_0_N
  : Attribute_0_N NOM EGAL VALEUR                       { std::cout << "Attribute_0_N" << std::endl; }
  | /* empty */
  ;

/*Attribute*/
/*  : NOM EGAL VALEUR                                     { std::cout << "Attribute" << std::endl; }*/
/*  ;*/

element
  : INF NOM Attribute_0_N SLASH SUP                     { std::cout << "element - empty" << std::endl; }
  | INF NOM Attribute_0_N SUP content INF SLASH NOM SUP { std::cout << "element - composite" << std::endl; }
  ;

content
  : content sub_element                                 { std::cout << "content" << std::endl; }
  | /* empty */
  ;

sub_element
  : DONNEES | element | CDSect | PI | COMMENT           { std::cout << "sub_element" << std::endl; }
  ;

CDSect
  : CDATABEGIN CDATAEND                                 { std::cout << "CDSect" << std::endl; }
  ;
