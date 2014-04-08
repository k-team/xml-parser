%{

#include <stack>
#include <list>
#include <cstring>
#include <cstdio>
#include <cstdlib>
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
  : prolog element Misc_0_N
  ;

prolog
  : XMLDecl_0_1 Misc_0_N doctypedecl_0_N
  ;

XMLDecl_0_1
  : XMLDecl
  | /* empty */
  ;

XMLDecl
  : PI
  ;

doctypedecl_0_N
  : doctypedecl Misc_0_N
  | /* empty */
  ;

doctypedecl
  : DOCTYPE
  ;

Misc_0_N
  : Misc_0_N Misc
  | /* empty */
  ;

Misc
  : COMMENT
  | PI
  ;

PI
  : INFSPECIAL NOM Attribute_0_N SUPSPECIAL /* FIXME check */
  ;

Attribute_0_N
  : Attribute_0_N | Attribute
  | /* empty */
  ;

Attribute
  : NOM EGAL VALEUR
  ;

element
  : EmptyElementTag
  | STag content ETag
  ;

EmptyElementTag
  : INF NOM Attribute_0_N SLASH SUP
  ;

STag
  : INF NOM Attribute_0_N SUP
  ;

ETag
  : INF SLASH NOM SUP
  ;

content
  : CharData_0_1 sub_element_0_N
  ;

CharData_0_1
  : CharData
  | /* empty */
  ;

CharData
  : DONNEES
  ;

sub_element_0_N
  : sub_element_0_N sub_element
  | /* empty */
  ;

sub_element
  : sub_sub_element CharData_0_1
  ;

sub_sub_element
  : element | CDSect | PI | COMMENT
  ;

CDSect
  : CDATABEGIN CDATAEND
  ;
