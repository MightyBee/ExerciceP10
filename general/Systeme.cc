#include "Systeme.h"
#include <iostream>
#include <string>
using namespace std;

//constructeur
Systeme::Systeme(SupportADessin* sup, Integrateur const& integrat, double t)
                : Dessinable(sup), integrateur(integrat.copie()), temps(t), collection(0) {}

// ajout d'un oscillateur
void Systeme::add(Oscillateur const& osc){
  collection.push_back(osc.copie());
  collection.back()->set_sup(support);
}

// evolution du système avec l'intégrateur
void Systeme::evolue(double dt){
  for(auto& osc : collection){
    integrateur->evolue(*osc, dt, temps);
  }
  temps+=dt;
}


void Systeme::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

void Systeme::phase() const{
  if(support!=nullptr){
    for(auto const& osc : collection){
      support->phase(*osc, *integrateur);
    }
  }
}

void Systeme::phase(unsigned int n) const{
  if(support!=nullptr and n>0 and n<=collection.size()){
    support->phase(*collection[n-1], *integrateur);
  }
}

void Systeme::changeIntegrateur(int i){
  switch(i){
    case 1:
      integrateur.reset(new IntegrateurEulerCromer());
      break;
    case 2:
      integrateur.reset(new IntegrateurNewmark());
      break;
    case 3:
      integrateur.reset(new IntegrateurRungeKutta());
      break;
  }
}

// permet l'affichage d'un systeme de façon standardisée //
ostream& Systeme::affiche(ostream& sortie) const{
  sortie << "# Systeme :" << endl << endl;
  sortie << temps << "   # temps" << endl << endl;
  for(size_t i(0); i<collection.size(); i++){
    sortie << "#" << i+1 << endl;
    sortie << *collection[i] << endl;
  }
  return sortie;
}




// permet l'affichage standard : sortie << oscillateur; //
ostream& operator<<(ostream& sortie, const Systeme& sys){
	return sys.affiche(sortie);
}
