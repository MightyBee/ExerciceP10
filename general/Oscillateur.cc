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

// retourne le vecteur de l'axe
Vecteur3D Oscillateur::get_a() const{return a;}

// retourne le vecteur de l'origine
Vecteur3D Oscillateur::get_O() const{return O;}

//retourne l'angle de précession
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
  }else{
    P=p;
  }
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
  }else{
    Q=q;
  }
}

// permet de modifier une vitesse de l'Oscillateur
void Oscillateur::set_Q(unsigned int n, double newValeur){
  try{Q.set_coord(n,newValeur);}
  catch(Erreur err){
    err.add_fct("Oscillateur::set_Q(unsigned int, double)");
    throw err;
  }
}

Vecteur Oscillateur::PQ() const{
  return Vecteur({P.get_coord(1),Q.get_coord(1)});
}

void Oscillateur::phase(Integrateur const& integrat) const{
  if(support!=nullptr){
    support->phase(*this,integrat);
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
                  if(P.taille()!=1){
                    Erreur err("initialisation Pendule", "Pendule::Pendule(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, SupportADessin*)",
                               "Le pendule doit avoir un seul paramètre (et sa dérivée temporelle) : l'angle du pendule (et sa vitesse angulaire). Ici : "+to_string(P.taille())+"paramètre(s).");
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

// retourne l'angle de nutation
double Pendule::get_angleNutation(bool degre) const{
  double angle(P.get_coord(1));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne l'angle de rotation propre
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
                  ,k(raideur), m(masse), frott(frottement)
                  {
                    if(P.taille()!=1){
                      Erreur err("initialisation Ressort", "Ressort::Ressort(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, SupportADessin*)",
                                 "Le ressort doit avoir un seul paramètre (et sa dérivée temporelle) : la distance à la position d'équilibre (et sa vitesse). Ici : "+to_string(P.taille())+"paramètre(s).");
                      throw err;
                    }
                  }

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

// retourne l'angle de nutation
double Ressort::get_angleNutation(bool degre) const{
  double angle(M_PI/2);
  if(a.x()!=0 or a.y()!=0){angle=a.angle(a.projXY());}
  if(a.z()<0){angle*=-1;}
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne l'angle de rotation propre
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

/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Torsion                         ###
###                                                                          ###
##############################################################################*/
//constructeur
Torsion::Torsion(const initializer_list<double>& liP,
                 const initializer_list<double>& liQ,
                 const Vecteur3D& a, const Vecteur3D& O,
                 double frottement, double inertie, double C_torsion,
                 SupportADessin* support)
                 : Oscillateur(liP,liQ,a,O,support)//TODO ERREUR
                  , frott(frottement), I(inertie), C(C_torsion){}



unique_ptr<Torsion> Torsion::clone() const{
  return unique_ptr<Torsion>(new Torsion(*this));
}

unique_ptr<Oscillateur> Torsion::copie() const{
  return clone();
}

void Torsion::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

//fonction d'évolution
Vecteur Torsion::f(const double& t) const{
  Vecteur retour({-(C*P.get_coord(1)+ frott*Q.get_coord(1))/I});
  return retour;
}

// retourne l'angle de nutation
double Torsion::get_angleNutation(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne l'angle de rotattion propre
double Torsion::get_angleRotPro(bool degre) const{
  double angle(P.get_coord(1));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//permet l'affichage d'un oscillateur de façon standarisée
ostream& Torsion::affiche(ostream& sortie) const{
  sortie << "# Torsion :" << endl;
  sortie << P << " # parametre (angle)" << endl;
  sortie << Q << " # vitesse angulaire" << endl;
  return sortie;
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE PenduleDouble                   ###
###                                                                          ###
##############################################################################*/

//constructeur
PenduleDouble::PenduleDouble(const std::initializer_list<double>& liP,
                       const std::initializer_list<double>& liQ,
                       const Vecteur3D& a,
                       const Vecteur3D& O,
                       double masse1, double longueur1,
                       double masse2, double longueur2,
                       SupportADessin* support) :
                       Oscillateur(liP, liQ, a, O, support),
                       m1(masse1), L1(longueur1),
                       m2(masse2), L2(longueur2) {}


unique_ptr<PenduleDouble> PenduleDouble::clone() const{
  return unique_ptr<PenduleDouble>(new PenduleDouble(*this));
}

unique_ptr<Oscillateur> PenduleDouble::copie() const{
  return clone();
}

void PenduleDouble::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

//fonction d'évolution
Vecteur PenduleDouble::f(const double& t) const{
  double M(m1+m2);
  double theta1(P.get_coord(1));
  double theta2(P.get_coord(2));
  double delta(theta1-theta2);
  double a((m2*g.norme()*cos(delta)*sin(theta2)-M*g.norme()*sin(theta1)
  -m2*L1*Q.get_coord(1)*Q.get_coord(1)*cos(delta)*sin(delta)
  -m2*L2*Q.get_coord(2)*Q.get_coord(2)*sin(delta))/(m1*L1+m2*L1*sin(delta)*sin(delta)));

  double b((M*g.norme()*cos(delta)*sin(theta1)-M*g.norme()*sin(theta2)
  +m2*L2*Q.get_coord(2)*Q.get_coord(2)*cos(delta)*sin(delta)
  +M*L1*Q.get_coord(1)*Q.get_coord(1)*sin(delta))/(m1*L2+m2*L2*sin(delta)*sin(delta)));

  Vecteur retour({a, b});
  return retour;
}

//retourne la position du 1er pendule
Vecteur3D PenduleDouble::pos1()const{
  Vecteur3D retour(O + L1*cos(P.get_coord(1))*(~g) + L1*sin(P.get_coord(1))*a);
  return retour;
}

//retourne la position du 2eme pendule
Vecteur3D PenduleDouble::pos2()const{
  Vecteur3D retour(pos1() + L2*cos(P.get_coord(2))*(~g) + L2*sin(P.get_coord(2))*a);
  return retour;
}

//vittesse du deuxième pendule
double PenduleDouble::vit2() const{
  return sqrt(pow(L1*Q.get_coord(1),2)+pow(L2*Q.get_coord(2),2)+2*cos(P.get_coord(1)-P.get_coord(2))*L1*L2*Q.get_coord(1)*Q.get_coord(2));
}

// retourne l'angle de nutation
double PenduleDouble::get_angleNutation(bool degre) const{
  double angle(P.get_coord(1));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne le deuxième angle de nutation
double PenduleDouble::get_angleNutation2(bool degre) const{
  double angle(P.get_coord(2)-P.get_coord(1));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne l'angle de rotation propre
double PenduleDouble::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}


// permet l'affichage d'un oscillateur de façon standardisée //
ostream& PenduleDouble::affiche(ostream& sortie) const{
  sortie << "# PenduleDouble :" << endl;
  sortie << P << " # parametres (angles)" << endl;
  sortie << Q << " # vitesses angulaires" << endl;
  sortie << pos1() << "# position du 1er pendule" << endl;
  sortie << pos2() << "# position du 2eme pendule" << endl;
  return sortie;
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE PenduleRessort                  ###
###                                                                          ###
##############################################################################*/

//constructeur
PenduleRessort::PenduleRessort(const std::initializer_list<double>& liP,
                               const std::initializer_list<double>& liQ,
                               const Vecteur3D& a,
                               const Vecteur3D& O,
                               double masse, double longueur, double raideur,
                               SupportADessin* support):
                               Oscillateur(liP, liQ, a, O, support),
                               m(masse), L(longueur), k(raideur) {}


unique_ptr<PenduleRessort> PenduleRessort::clone() const{
  return unique_ptr<PenduleRessort>(new PenduleRessort(*this));
}

unique_ptr<Oscillateur> PenduleRessort::copie() const{
  return clone();
}

void PenduleRessort::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}

//fonction d'évolution
Vecteur PenduleRessort::f(const double& t) const{
  Vecteur retour(Vecteur({0,g.norme()}) - (k/m)*(1-(L/P.norme()))*P);
  return retour;
}

//retourne position d'un pendule
Vecteur3D PenduleRessort::position() const {
  Vecteur3D retour(O + P.get_coord(1)*a + P.get_coord(2)*(~g));
  return retour;
}

// retourne l'angle de nutation
double PenduleRessort::get_angleNutation(bool degre) const{
  double angle(0);
  double x(P.get_coord(1)), z(-P.get_coord(2));
  if(x!=0 or z!=0){angle=Vecteur3D(1,0,0).angle({x,0,z});}
  if(z<0){angle*=-1;}
  if(degre){angle=angle*180.0/M_PI+90;}
  else{angle+=0.5*M_PI;}
  return angle;
}

// retourne l'angle de rotation propre
double PenduleRessort::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}


// permet l'affichage d'un oscillateur de façon standardisée
ostream& PenduleRessort::affiche(ostream& sortie) const{
  sortie << "# PenduleRessort :" << endl;
  sortie << P << " # parametre (x et y)" << endl;
  sortie << Q << " # vitesses" << endl;
  sortie << position() << "# position" << endl;
  return sortie;
}

/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Chariot                         ###
###                                                                          ###
##############################################################################*/
//Constructeur
Chariot::Chariot(const initializer_list<double>& liP,
                 const initializer_list<double>& liQ,
                 const Vecteur3D& a, const Vecteur3D& O,
                 double raideur, double masseRessort, double frottRessort,
                 double longueur, double massePendule, double frottPendule,
                 SupportADessin* support)
                 : Oscillateur(liP,liQ,a,O,support),  //TODO ERREUR
                 k(raideur), m1(masseRessort), frott1(frottRessort),
                 L(longueur), m2(massePendule), frott2(frottPendule)
                 {if(a*g!=0){
                    Erreur err("initialisation Chariot", "Chariot::Chariot(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, double, doubel, SupportADessin*)",
                               "L'axe du chariot doit être orthogonal au vecteur g.");
                    throw err;
                  }
                  if(P.taille()!=2){
                    Erreur err("initialisation Chariot", "Chariot::Chariot(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, double, doubel, SupportADessin*)",
                               "Le chariot doit avoir deux paramètres (et leur dérivée temporelle) : la distance du chariot et l'angle du pendule (+dérivées). Ici : "+to_string(P.taille())+"paramètre(s).");
                    throw err;
                  }
                 }

  unique_ptr<Chariot> Chariot::clone() const{
   return unique_ptr<Chariot>(new Chariot(*this));
  }

  unique_ptr<Oscillateur> Chariot::copie() const{
    return clone();
  }

  void Chariot::dessine() const{
    if(support!=nullptr){
      support->dessine(*this);
    }
  }

// fonction d'évolution
Vecteur Chariot::f(const double& t) const {
  Vecteur retour (2);
  double P1(P.get_coord(1));
  double P2(P.get_coord(2));
  double Q1(Q.get_coord(1));
  double Q2(Q.get_coord(2));

  double A(m1+m2*pow(sin(P2), 2));
  double B(k*P1 + frott1* Q1 - m2*L*Q2*Q2*sin(P2));
  double C(g.norme()*sin(P2) + frott2*Q2);

  retour.set_coord(1, 1.0/A*(-B+m2*C*cos(P2)));
  retour.set_coord(2, 1.0/A*((B*cos(P2)-(m1+m2)*C)/L));

  return retour;
}

// retourne la position du chariot
Vecteur3D Chariot::posC()const{
  Vecteur3D retour(O+P.get_coord(1)*a);
  return retour;
}

// retourne la position du pendule
Vecteur3D Chariot::posP()const{
  Vecteur3D retour(posC()+L*sin(P.get_coord(2))*a+L*cos(P.get_coord(2))*(~g));
  return retour;
}

// vitesse du pendule
double Chariot::vitP() const{
  return sqrt(pow(Q.get_coord(1),2)+2*Q.get_coord(1)*cos(P.get_coord(2))*L*Q.get_coord(2)+pow(L*Q.get_coord(2),2));
}

// retourne l'angle de nutation
double Chariot::get_angleNutation(bool degre) const {
  double angle(P.get_coord(2));
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne l'angle de rotation propre
double Chariot::get_angleRotPro(bool degre) const {
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

 // permet l'affichage d'un oscillateur de facon standarisée 
ostream& Chariot::affiche(std::ostream& sortie) const {
  sortie << "# Chariot :" << endl;
  sortie << P << " # parametre (distance de l'origine du chariot et angle du pendule)" << endl;
  sortie << Q << " # vitesse (vitesse chariot et vitesse angulaire du pendule)" << endl;
  sortie << posC() << "# position chariot" << endl;
  sortie << posP() << "# position pendule" << endl;
  return sortie;
}
