#include "commun.h"
#include <iostream>
#include <cstring>

using namespace std;

int xmlparse(void);

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
