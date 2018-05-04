#include <iostream>
#include <initializer_list>
#include <cmath>
#include "Oscillateur.h"
#include "constantes.h"
using namespace std;

/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Oscillateur                     ###
###                                                                          ###
##############################################################################*/

//#############################  constructeurs  ##############################//
// construit un Oscillateur à partir d'une liste pour les paramètres et une autre pour leurs dérivées //
Oscillateur::Oscillateur(const initializer_list<double>& liP,
                         const initializer_list<double>& liQ,
                         const Vecteur3D& a,
                         const Vecteur3D& O,
                         SupportADessin* support)
                        : Dessinable(support), P(liP), Q(liQ), a(~a), O(O) // TODO, question : comment intercepter l'erreur lancée par operator~ si l'axe est nul ?
                        {if(liP.size()!=liQ.size()){
                          Erreur err("dimension", "Oscillateur::Oscillateur(const initializer_list<double>& x4)",
                    		             "Les dimensions des vecteurs 'parametre' et 'vitesse' doivent être les mêmes (ici : "+to_string(liP.size())+" et "+to_string(liQ.size())+")");
                          throw err;
                         }
                        }

//##############################  accesseurs  ################################//
// retourne le vecteur des paramètres de l'oscillateur //
Vecteur Oscillateur::get_P() const{return P;}

// retourne le vecteur des "vitesses" de l'oscillateur //
Vecteur Oscillateur::get_Q() const{return Q;}

Vecteur3D Oscillateur::get_a() const{return a;}

Vecteur3D Oscillateur::get_O() const{return O;}

double Oscillateur::get_anglePrecession(bool degre) const{
  double angle(0);
  if(a.x()!=0 or a.y()!=0){angle=Vecteur3D(1,0,0).angle(a.projXY());}
  if(a.y()<0){angle*=-1;}
  if(degre){angle=angle*(180.0/M_PI)-90;}
  else{angle-=M_PI/2;} // car l'axe de nutation est a 90° avec l'axe de l'oscillateur
  return angle;
}


//#############################  manipulateurs  ##############################//
// permet de modifier l'intégralité des paramètres //
void Oscillateur::set_P(const Vecteur& p){
  if(p.taille() != P.taille()){
    Erreur r("dimension", "Oscillateur::set_P(const Vecteur&)",
             "La dimension attendue du vecteur passé en paramètre était "+to_string(P.taille())+", et non pas "+to_string(p.taille()));
    throw r;
  }
  else{
    P=p;}
}

// permet de modifier un paramètre de l'Oscillateur
void Oscillateur::set_P(unsigned int n, double newValeur){
  try{P.set_coord(n,newValeur);}
  catch(Erreur err){
    err.add_fct("Oscillateur::set_P(unsigned int, double)");
    throw err;
  }
}

// permet de modifier l'intégralité des "vitesses" //
void Oscillateur::set_Q(const Vecteur& q){
  if(q.taille() != Q.taille()){
    Erreur r("dimension", "Oscillateur::set_Q(const Vecteur&)",
             "La dimension attendue du vecteur passé en paramètre était "+to_string(Q.taille())+", et non pas "+to_string(q.taille()));
    throw r;
  }
  else{
    Q=q;}
}

// permet de modifier une vitesse de l'Oscillateur
void Oscillateur::set_Q(unsigned int n, double newValeur){
  try{Q.set_coord(n,newValeur);}
  catch(Erreur err){
    err.add_fct("Oscillateur::set_Q(unsigned int, double)");
    throw err;
  }
}



// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Oscillateur::affiche(ostream& sortie) const{
  sortie << "# Oscillateur :" << endl;
  sortie << P << " # parametre" << endl;
  sortie << Q << " # vitesse" << endl;
  return sortie;
}



/*##############################################################################
###                                                                          ###
###                           OPÉRATEURS EXTERNES                            ###
###                                                                          ###
##############################################################################*/

// permet l'affichage standard : sortie << oscillateur; //
ostream& operator<<(ostream& sortie, const Oscillateur& osc){
	return osc.affiche(sortie);
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Pendule                         ###
###                                                                          ###
##############################################################################*/
//constructeur
Pendule::Pendule(const initializer_list<double>& liP,
                 const initializer_list<double>& liQ,
                 const Vecteur3D& a, const Vecteur3D& O,
                 double longueur, double masse, double frottement,
                 SupportADessin* support)
                 : Oscillateur(liP, liQ, a, O, support), L(longueur), m(masse), frott(frottement)
                 {if(a*g!=0){
                    Erreur err("initialisation Pendule", "Pendule::Pendule(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, SupportADessin*)",
                               "L'axe du pendule doit être orthogonal au vecteur g.");
                    throw err;
                  }
                } //TODO, question : comment catcher une erreur lancée par le constructeur de Oscillateur ?

unique_ptr<Pendule> Pendule::clone() const{
  return unique_ptr<Pendule>(new Pendule(*this));
}

unique_ptr<Oscillateur> Pendule::copie() const{
  return clone();
}

void Pendule::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

//fonction d'évolution
Vecteur Pendule::f(const double& t) const{
    Vecteur retour({-(g.norme()/L)*sin(P.get_coord(1))-(frott*Q.get_coord(1)/(m*L*L))});
    return retour;
}
//retourne position d'un pendule
Vecteur3D Pendule::position() const {
  Vecteur3D retour(O + L*cos(P.get_coord(1))*(~g) + L*sin(P.get_coord(1))*a);
  return retour;
}

double Pendule::get_L() const{return L;}

double Pendule::get_angleNutation(bool degre) const{
  double angle(P.get_coord(1));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

double Pendule::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}


// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Pendule::affiche(ostream& sortie) const{
  sortie << "# Pendule :" << endl;
  sortie << P << " # parametre (angle)" << endl;
  sortie << Q << " # vitesse angulaire" << endl;
  sortie << position() << "# position" << endl;
  return sortie;
}

/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Ressort                         ###
###                                                                          ###
##############################################################################*/
//constructeur
Ressort::Ressort(const initializer_list<double>& liP,
                 const initializer_list<double>& liQ,
                 const Vecteur3D& a, const Vecteur3D& O,
                 double raideur,double masse, double frottement,
                 SupportADessin* support)
                  : Oscillateur(liP,liQ,a,O,support)//TODO ERREUR
                  ,k(raideur), m(masse), frott(frottement){}

unique_ptr<Ressort> Ressort::clone() const{
  return unique_ptr<Ressort>(new Ressort(*this));
}

unique_ptr<Oscillateur> Ressort::copie() const{
  return clone();
}

void Ressort::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

//fonction d'évolution
Vecteur Ressort::f(const double& t) const{
  Vecteur retour({(-(k/m)*P.get_coord(1)-(frott/m)*Q.get_coord(1)+g*a)});
  return retour;
}
//retourne la position d'un ressort
Vecteur3D Ressort::position()const{
  Vecteur3D retour(O + P.get_coord(1)*a);
  return retour;
}


double Ressort::get_angleNutation(bool degre) const{
  double angle(M_PI/2);
  if(a.x()!=0 or a.y()!=0){angle=a.angle(a.projXY());}
  if(a.z()<0){angle*=-1;}
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

double Ressort::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Ressort::affiche(ostream& sortie) const{
  sortie << "# Ressort :" << endl;
  sortie << P << " # parametre (distance de l'origine)" << endl;
  sortie << Q << " # vitesse" << endl;
  sortie << position() << "# position" << endl;
  return sortie;
}
