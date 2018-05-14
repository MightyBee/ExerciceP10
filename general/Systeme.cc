#include "Systeme.h"
#include <iostream>
#include <string>
using namespace std;

Systeme::Systeme(SupportADessin* sup, Integrateur const& integrat, double t)
                : Dessinable(sup), integrateur(integrat.copie()), temps(t), collection(0) {}


void Systeme::initialize(){
  int choice(0);
  cout << endl << " --- INITIALISATION DU SYSTEME --- " << endl;
  do{
    cout << endl << "Système : " << collection.size() << " oscillateur(s)" << endl;
    cout << "Quel type d'oscillateur voulez-vous ajouter ?" << endl;
    cout << "   0. Lancer simulation (si aucun oscillateur entré, mode préconfiguré lancé)" << endl;
    cout << "   1. Pendule \n   2. Ressort \n   3. Chariot" << endl << endl;
    cout << "Votre choix : ";
    cin >> choice;
    switch(choice) {
      case 0 :
        if(collection.size()<1){
          add(Pendule({0},{1},{-1,0,0},{0,1,0},2,2));
          add(Pendule({M_PI-0.00001},{0},{1,1,0},{1,0,0},1,1,0.05));
          add(Ressort({1},{0},{-1,2,-1},{0,0,0},2,1));
          add(Chariot({0,0},{4,0},{1,-2,0},{-1,-1,0},100,10,0,1,1,0));
        }
        cout << endl << "Lancement de la simulation !" << endl;
        break;
      case 1 :
      {
        cout << endl << "Pendule : " << endl;
        double angle(0);
        cout << "Angle initiale (en degré) : ";
        cin >> angle;
        double vitesse(0);
        cout << "Vitesse angulaire initiale (en degré) : ";
        cin >> vitesse;
        double aX(0),aY(0);
        cout <<"Axe : \n x : "; cin >> aX;
        cout <<" y : "; cin >> aY;
        cout <<" z : 0" << endl;
        double OX(0), OY(0), OZ(0);
        cout <<"Origine : \n x : "; cin >> OX;
        cout <<" y : "; cin >> OY;
        cout <<" z : "; cin >> OZ;
        double L(0);
        cout << "Longueur : "; cin >> L;
        double m(0);
        cout << "Masse : "; cin >> m;
        double frott(0);
        cout << "Frottement : "; cin >> frott;
        add(Pendule({angle},{vitesse},{aX,aY,0},{OX,OY,OZ},L,m,frott));
        cout << endl << "Ajout du pendule au système ..." << endl << endl;
        break;
      }
      case 2 :
      {
        cout << endl << "Ressort : " << endl;
        double dist(0);
        cout << "Distance initiale (de la pos. d'eq.) : ";
        cin >> dist;
        double vitesse(0);
        cout << "Vitesse initiale : ";
        cin >> vitesse;
        double aX(0),aY(0), aZ(0);
        cout <<"Axe : \n x : "; cin >> aX;
        cout <<" y : "; cin >> aY;
        cout <<" z : "; cin >> aZ;
        double OX(0), OY(0), OZ(0);
        cout <<"Origine : \n x : "; cin >> OX;
        cout <<" y : "; cin >> OY;
        cout <<" z : "; cin >> OZ;
        double k(0);
        cout << "Raideur : "; cin >> k;
        double m(0);
        cout << "Masse : "; cin >> m;
        double frott(0);
        cout << "Frottement : "; cin >> frott;
        add(Ressort({dist},{vitesse},{aX,aY,aZ},{OX,OY,OZ},k,m,frott));
        cout << endl << "Ajout du ressort au système ..." << endl << endl;
        break;
      }
      case 3 :
      {
        cout << endl << "Chariot : " << endl;
        double dist(0);
        cout << "Distance initiale du chariot (de la pos. d'eq.) : "; cin >> dist;
        double vitesse(0);
        cout << "Vitesse initiale du chariot : "; cin >> vitesse;
        double angle(0);
        cout << "Angle initiale du pendule (en degré) : "; cin >> angle;
        double vitAng(0);
        cout << "Vitesse angulaire initiale (en degré) : "; cin >> vitAng;
        double aX(0),aY(0);
        cout <<"Axe : \n x : "; cin >> aX;
        cout <<" y : "; cin >> aY;
        cout <<" z : 0" << endl;
        double OX(0), OY(0), OZ(0);
        cout <<"Origine : \n x : "; cin >> OX;
        cout <<" y : "; cin >> OY;
        cout <<" z : "; cin >> OZ;
        double k(0);
        cout << "Raideur du ressort du chariot : "; cin >> k;
        double mC(0);
        cout << "Masse du chariot : "; cin >> mC;
        double frottC(0);
        cout << "Frottement chariot : "; cin >> frottC;
        double L(0);
        cout << "Longueur du pendule : "; cin >> L;
        double mP(0);
        cout << "Masse du pendule : "; cin >> mP;
        double frottP(0);
        cout << "Frottement pendule : "; cin >> frottP;
        add(Chariot({dist, angle},{vitesse, vitAng},{aX,aY,0},{OX,OY,OZ},k,mC,frottC, L, mP, frottP));
        cout << endl << "Ajout du chariot au système ..." << endl << endl;
        break;
      }
    }
  }while(choice!=0);
}

void Systeme::add(Oscillateur const& osc){
  collection.push_back(osc.copie());
  collection.back()->set_sup(support);
}

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
