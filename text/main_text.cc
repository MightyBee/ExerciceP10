#include <iostream>
#include <string>
#include "Systeme.h"
#include "TextViewer.h"
#include "constantes.h"
using namespace std;

int main(){
  try{
    TextViewer TV(cout);
    IntegrateurEulerCromer euler;
    Systeme syst(&TV,euler);
    Pendule p({M_PI/2});
    syst.add(p);
    Ressort r1({0.18}, {0}, {0.8,0,0.2}, {0,0,0}, 1, 0.25, 0.1, &TV);
    Ressort r2({0.18}, {0}, {0.8,0,0.2}, {0,0,0}, 2, 0.25, 0.1);
    syst.add(r1);
    syst.add(r2);
    cout << syst << endl;
    for(int i(1);i<=100;i++){
      syst.evolue();
      syst.dessine();
    }
  }catch(Erreur err){
    err.affiche("testSysteme.cc");
  }

  return 0;
}
