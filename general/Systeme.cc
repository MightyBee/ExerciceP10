#include "Systeme.h"
#include <iostream>
#include <string>
using namespace std;

Systeme::Systeme(SupportADessin* sup, Integrateur const& integrat, double t)
                : Dessinable(sup), integrateur(integrat.copie()), temps(t), collection(0) {}

void Systeme::add(Oscillateur const& osc){
  collection.push_back(osc.copie());
  collection.back()->set_sup(support);
}

void Systeme::evolue(double dt){
  for(auto& osc : collection){
    integrateur->evolue(*osc, dt);
  }
  temps+=dt;
}

const vector<unique_ptr<Oscillateur>>& Systeme::get_col() const{
  return collection;
}

void Systeme::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
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
