#include "commun.h"
#include <iostream>
#include <string>

using namespace std;

extern int xmlparse(void);

int main(void)
{
   int retour = xmlparse();
   if (!retour)
   {
      cout<<"Entrée standard reconnue"<<endl;
   }
   else
   {
      cout<<"Entrée standard non reconnue"<<endl;
   }
   return 1;
}

// vim:ft=cpp et sw=2 sts=2:
