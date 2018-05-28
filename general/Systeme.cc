#include "Systeme.h"
#include <iostream>
#include <string>
using namespace std;

// constructeur
Systeme::Systeme(SupportADessin* sup, Integrateur const& integrat, double t)
                : Dessinable(sup), integrateur(integrat.copie()), temps(t), collection(0) {}


// pour savoir on veut afficher le portrait des phases du (i+1)ème oscillateur
bool Systeme::getPhaseBool(unsigned int i) const{
  if(i<collection.size()){
    return collection[i]->getPhaseBool();
  }
}


// ajout d'un oscillateur au système
void Systeme::add(Oscillateur const& osc){
  collection.push_back(osc.copie()); // copie polymorphique, on ajoute en vrai un unique_ptr<Oscillateur>
  collection.back()->set_sup(support); // le système a un support à dessin, tous les oscillateurs doivent avoir le même
}


// permet de choisir parmis les trois intégrateurs implémentés
void Systeme::changeIntegrateur(int i){
  switch(i){
    case 1: // méthode d'Euler-Cromer
      integrateur.reset(new IntegrateurEulerCromer());
      break;
    case 2: // méthode de Newmark
      integrateur.reset(new IntegrateurNewmark());
      break;
    case 3: // méthode de Runge-Kutta
      integrateur.reset(new IntegrateurRungeKutta());
      break;
  }
}


// permet d'afficher/ne plus afficher le portrait des phases du (i+1)ème oscillateur
void Systeme::changePhase(unsigned int i){
  if(i<collection.size()){
    collection[i]->changePhase();
  }
}


// evolution du système avec l'intégrateur
void Systeme::evolue(double dt){
  for(auto& osc : collection){
    integrateur->evolue(*osc, dt, temps);
  }
  temps+=dt;
}


// méthode appelée pour dessiner un système (dessin s'adaptant au support)
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
