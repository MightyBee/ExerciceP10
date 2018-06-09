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
                         const Vecteur3D& a, const Vecteur3D& O,
                         bool phase, SupportADessin* support)
                        : Dessinable(support), P(liP), Q(liQ), a(~a), O(O), phase(phase) // on veut toujours que l'axe soit unitaire d'où le a(~a)
                        {if(liP.size()!=liQ.size()){
                          Erreur err("dimension", "Oscillateur::Oscillateur(const initializer_list<double>& x4)",
                    		             "Les dimensions des vecteurs 'parametre' et 'vitesse' doivent être les mêmes (ici : "+to_string(liP.size())+" et "+to_string(liQ.size())+")");
                          throw err;
                         }
                        }

//##############################  accesseurs  ################################//
// retourne l'angle de précession
double Oscillateur::get_anglePrecession(bool degre) const{
  double angle(0);
  if(a.x()!=0 or a.y()!=0){ // si les coordonées x et y sont nulles, l'angle de précession est nul
    angle=Vecteur3D(1,0,0).angle(a.projXY()); // l'angle de précession est l'angle entre l'axe X et la projection du vecteur sur le plan XY
  } // note : retourne l'angle en valeur absolu
  if(a.y()<0){angle*=-1;} // on veut un angle orienté
  if(degre){angle=angle*(180.0/M_PI)-90;} // conversion en degré si nécessaire (et -90° voir raison si dessous)
  else{angle-=M_PI/2;} // on est parti de l'axe de l'oscillateur, alors que nous voulons l'angle avec l'axe de nutation, qui est a 90° avec l'axe de l'oscillateur
  return angle;
}


//#############################  manipulateurs  ##############################//
// permet de modifier l'intégralité des paramètres //
void Oscillateur::set_P(const Vecteur& p){
  if(p.taille() != P.taille()){ // on vérifie que les dimensions correspondent
    Erreur r("dimension", "Oscillateur::set_P(const Vecteur&)",
             "La dimension attendue du vecteur passé en paramètre était "+to_string(P.taille())+", et non pas "+to_string(p.taille()));
    throw r;
  }else{
    P=p;
  }
}

// permet de modifier un paramètre de l'Oscillateur //
void Oscillateur::set_P(unsigned int n, double newValeur){
  try{P[n-1]=newValeur;}
  catch(Erreur err){
    err.add_fct("Oscillateur::set_P(unsigned int, double)");
    throw err;
  }
}

// permet de modifier l'intégralité des dérivées //
void Oscillateur::set_Q(const Vecteur& q){
  if(q.taille() != Q.taille()){
    Erreur r("dimension", "Oscillateur::set_Q(const Vecteur&)",
             "La dimension attendue du vecteur passé en paramètre était "+to_string(Q.taille())+", et non pas "+to_string(q.taille()));
    throw r;
  }else{
    Q=q;
  }
}

// permet de modifier une dérivée de l'Oscillateur //
void Oscillateur::set_Q(unsigned int n, double newValeur){
  try{Q[n-1]=newValeur;}
  catch(Erreur err){
    err.add_fct("Oscillateur::set_Q(unsigned int, double)");
    throw err;
  }
}

// permet de changer l'attribut booléen phase //
void Oscillateur::changePhase(){
  if(phase) phase = false;
  else phase = true;
}

//###########################  autres opérations  ############################//
// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Oscillateur::affiche(ostream& sortie) const{
  sortie << "# Oscillateur :" << endl;
  sortie << P << " # parametre" << endl;
  sortie << Q << " # vitesse" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
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
                 double longueur, double masse,
                 double frottement, bool phase,
                 SupportADessin* support)
                 : Oscillateur(liP, liQ, a, O, phase, support), L(longueur), m(masse), frott(frottement)
                 {if(a*g!=0){
                    Erreur err("initialisation Pendule", "Pendule::Pendule(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
                               "L'axe du pendule doit être orthogonal au vecteur g.");
                    throw err;
                  }
                  if(P.taille()!=1){
                    Erreur err("initialisation Pendule", "Pendule::Pendule(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
                               "Le pendule doit avoir un seul paramètre (et sa dérivée temporelle) : l'angle du pendule (et sa vitesse angulaire). Ici : "+to_string(P.taille())+"paramètre(s).");
                    throw err;
                  }
                }

unique_ptr<Pendule> Pendule::clone() const{
  return unique_ptr<Pendule>(new Pendule(*this));
}

unique_ptr<Oscillateur> Pendule::copie() const{
  return clone();
}

// retourne l'angle de nutation
double Pendule::get_angleNutation(bool degre) const{
  double angle(P[0]);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne l'angle de rotation propre
double Pendule::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne position d'un pendule
Vecteur3D Pendule::position() const {
  Vecteur3D retour(O + L*cos(P[0])*(~g) + L*sin(P[0])*a);
  return retour;
}

//fonction d'évolution
Vecteur Pendule::f(const double& t) const{
    Vecteur retour({-(g.norme()/L)*sin(P[0])-(frott*Q[0]/(m*L*L))});
    return retour;
}

// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Pendule::affiche(ostream& sortie) const{
  sortie << "# Pendule :" << endl;
  sortie << P << " # parametre (angle)" << endl;
  sortie << Q << " # vitesse angulaire" << endl;
  sortie << position() << "# position" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void Pendule::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
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
                 double raideur,double masse,
                 double frottement, bool phase,
                 SupportADessin* support)
                : Oscillateur(liP,liQ,a,O,phase,support),
                  k(raideur), m(masse), frott(frottement)
                {
                  if(P.taille()!=1){
                    Erreur err("initialisation Ressort", "Ressort::Ressort(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
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

//retourne la position d'un ressort
Vecteur3D Ressort::position()const{
  Vecteur3D retour(O + P[0]*a);
  return retour;
}

//fonction d'évolution
Vecteur Ressort::f(const double& t) const{
  Vecteur retour({(-(k/m)*P[0]-(frott/m)*Q[0]+g*a)});
  return retour;
}

// permet l'affichage d'un oscillateur de façon standardisée //
ostream& Ressort::affiche(ostream& sortie) const{
  sortie << "# Ressort :" << endl;
  sortie << P << " # parametre (distance de l'origine)" << endl;
  sortie << Q << " # vitesse" << endl;
  sortie << position() << "# position" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void Ressort::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE RessortForce                    ###
###                                                                          ###
##############################################################################*/

//constructeur
RessortForce::RessortForce(const initializer_list<double>& liP,
                           const initializer_list<double>& liQ,
                           const Vecteur3D& a, const Vecteur3D& O,
                           double k,double m, double frot, double A,
                           double w, bool phase, SupportADessin* support)
                         : Ressort(liP,liQ,a,O,k,m,frot,phase,support),
                           amplitude(A),w(w)
                         { if(P.taille()!=1){
                             Erreur err("initialisation RessortForce", "RessortForce::RessortForce(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, double, bool, SupportADessin*)",
                                        "Le ressort doit avoir un seul paramètre (et sa dérivée temporelle) : la distance à la position d'équilibre (et sa vitesse). Ici : "+to_string(P.taille())+"paramètre(s).");
                             throw err;
                           }
                         }

unique_ptr<RessortForce> RessortForce::clone() const{
  return unique_ptr<RessortForce>(new RessortForce(*this));
}

unique_ptr<Oscillateur> RessortForce::copie() const{
  return clone();
}

//fonction d'évolution
Vecteur RessortForce::f(const double& t) const{
  Vecteur retour({(-(k/m)*P[0]-(frott/m)*Q[0]+g*a+(amplitude/m)*cos(w*t))});
  return retour;
}

// permet l'affichage d'un oscillateur de façon standardisée //
ostream& RessortForce::affiche(ostream& sortie) const{
  sortie << "# RessortForce :" << endl;
  sortie << P << " # parametre (distance de l'origine)" << endl;
  sortie << Q << " # vitesse" << endl;
  sortie << position() << "# position" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void RessortForce::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
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
                 bool phase, SupportADessin* support)
                : Oscillateur(liP,liQ,a,O,phase,support),
                  frott(frottement), I(inertie), C(C_torsion)
                {
                  if(P.taille()!=1){
                    Erreur err("initialisation Torsion", "Torsion::Torsion(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
                               "Le pendule de torsion doit avoir un seul paramètre (et sa dérivée temporelle). Ici : "+to_string(P.taille())+"paramètre(s).");
                    throw err;
                  }
                }

unique_ptr<Torsion> Torsion::clone() const{
  return unique_ptr<Torsion>(new Torsion(*this));
}

unique_ptr<Oscillateur> Torsion::copie() const{
  return clone();
}

// retourne l'angle de nutation
double Torsion::get_angleNutation(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne l'angle de rotattion propre
double Torsion::get_angleRotPro(bool degre) const{
  double angle(P[0]);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//fonction d'évolution
Vecteur Torsion::f(const double& t) const{
  Vecteur retour({-(C*P[0]+ frott*Q[0])/I});
  return retour;
}

//permet l'affichage d'un oscillateur de façon standarisée
ostream& Torsion::affiche(ostream& sortie) const{
  sortie << "# Torsion :" << endl;
  sortie << P << " # parametre (angle)" << endl;
  sortie << Q << " # vitesse angulaire" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void Torsion::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
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
                 bool phase, SupportADessin* support)
                : Oscillateur(liP,liQ,a,O,phase,support),
                  k(raideur),  m1(masseRessort), frott1(frottRessort),
                  L(longueur), m2(massePendule), frott2(frottPendule)
                 {if(a*g!=0){
                    Erreur err("initialisation Chariot", "Chariot::Chariot(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, double, double, bool, SupportADessin*)",
                               "L'axe du chariot doit être orthogonal au vecteur g.");
                    throw err;
                  }
                  if(P.taille()!=2){
                    Erreur err("initialisation Chariot", "Chariot::Chariot(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, double, double, bool, SupportADessin*)",
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

// retourne l'angle de nutation
double Chariot::get_angleNutation(bool degre) const {
  double angle(P[1]);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne l'angle de rotation propre
double Chariot::get_angleRotPro(bool degre) const {
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne la position du chariot
Vecteur3D Chariot::posC()const{
  Vecteur3D retour(O+P[0]*a);
  return retour;
}

// retourne la position du pendule
Vecteur3D Chariot::posP()const{
  Vecteur3D retour(posC()+L*sin(P[1])*a+L*cos(P[1])*(~g));
  return retour;
}

// vitesse du pendule
double Chariot::vitP() const{
  return sqrt(pow(Q[0],2)+2*Q[0]*cos(P[1])*L*Q[1]+pow(L*Q[1],2));
}

// fonction d'évolution
Vecteur Chariot::f(const double& t) const {
  Vecteur retour (2);
  double P1(P[0]);
  double P2(P[1]);
  double Q1(Q[0]);
  double Q2(Q[1]);

  double A(m1+m2*pow(sin(P2), 2));
  double B(k*P1 + frott1* Q1 - m2*L*Q2*Q2*sin(P2));
  double C(g.norme()*sin(P2) + frott2*Q2);

  retour[0]=1.0/A*(-B+m2*C*cos(P2));
  retour[1]=1.0/A*((B*cos(P2)-(m1+m2)*C)/L);

  return retour;
}

 // permet l'affichage d'un oscillateur de facon standarisée
ostream& Chariot::affiche(std::ostream& sortie) const {
  sortie << "# Chariot :" << endl;
  sortie << P << " # parametre (distance de l'origine du chariot et angle du pendule)" << endl;
  sortie << Q << " # vitesse (vitesse chariot et vitesse angulaire du pendule)" << endl;
  sortie << posC() << "# position chariot" << endl;
  sortie << posP() << "# position pendule" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void Chariot::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE PenduleDouble                   ###
###                                                                          ###
##############################################################################*/

//constructeur
PenduleDouble::PenduleDouble(const std::initializer_list<double>& liP,
                             const std::initializer_list<double>& liQ,
                             const Vecteur3D& a, const Vecteur3D& O,
                             double masse1, double longueur1,
                             double masse2, double longueur2,
                             bool phase, SupportADessin* support)
                            : Oscillateur(liP, liQ, a, O, phase, support),
                              m1(masse1), L1(longueur1),
                              m2(masse2), L2(longueur2)
                            {
                              if(a*g!=0){
                                Erreur err("initialisation PenduleDouble", "PenduleDouble::PenduleDouble(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, bool, SupportADessin*)",
                                           "L'axe du pendule double doit être orthogonal au vecteur g.");
                                throw err;
                              }
                              if(P.taille()!=2){
                                Erreur err("initialisation PenduleDouble", "PenduleDouble::PenduleDouble(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, double, bool, SupportADessin*)",
                                           "Le pendule double doit avoir deux paramètres (et leur dérivée temporelle). Ici : "+to_string(P.taille())+"paramètre(s).");
                                throw err;
                              }
                            }


unique_ptr<PenduleDouble> PenduleDouble::clone() const{
  return unique_ptr<PenduleDouble>(new PenduleDouble(*this));
}

unique_ptr<Oscillateur> PenduleDouble::copie() const{
  return clone();
}

// retourne l'angle de nutation
double PenduleDouble::get_angleNutation(bool degre) const{
  double angle(P[0]);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

// retourne le deuxième angle de nutation
double PenduleDouble::get_angleNutation2(bool degre) const{
  double angle(P[1]-P[0]);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne l'angle de rotation propre
double PenduleDouble::get_angleRotPro(bool degre) const{
  double angle(0);
  if(degre){angle*=180.0/M_PI;}
  return angle;
}

//retourne la position du 1er pendule
Vecteur3D PenduleDouble::pos1()const{
  Vecteur3D retour(O + L1*cos(P[0])*(~g) + L1*sin(P[0])*a);
  return retour;
}

//retourne la position du 2eme pendule
Vecteur3D PenduleDouble::pos2()const{
  Vecteur3D retour(pos1() + L2*cos(P[1])*(~g) + L2*sin(P[1])*a);
  return retour;
}

//vitesse du deuxième pendule
double PenduleDouble::vit2() const{
  return sqrt(pow(L1*Q[0],2)+pow(L2*Q[1],2)+2*cos(P[0]-P[1])*L1*L2*Q[0]*Q[1]);
}

//fonction d'évolution
Vecteur PenduleDouble::f(const double& t) const{
  double M(m1+m2);
  double theta1(P[0]);
  double theta2(P[1]);
  double delta(theta1-theta2);
  double a((m2*g.norme()*cos(delta)*sin(theta2)-M*g.norme()*sin(theta1)
  -m2*L1*Q[0]*Q[0]*cos(delta)*sin(delta)
  -m2*L2*Q[1]*Q[1]*sin(delta))/(m1*L1+m2*L1*sin(delta)*sin(delta)));

  double b((M*g.norme()*cos(delta)*sin(theta1)-M*g.norme()*sin(theta2)
  +m2*L2*Q[1]*Q[1]*cos(delta)*sin(delta)
  +M*L1*Q[0]*Q[0]*sin(delta))/(m1*L2+m2*L2*sin(delta)*sin(delta)));

  Vecteur retour({a, b});
  return retour;
}

// permet l'affichage d'un oscillateur de façon standardisée //
ostream& PenduleDouble::affiche(ostream& sortie) const{
  sortie << "# PenduleDouble :" << endl;
  sortie << P << " # parametres (angles)" << endl;
  sortie << Q << " # vitesses angulaires" << endl;
  sortie << pos1() << "# position du 1er pendule" << endl;
  sortie << pos2() << "# position du 2eme pendule" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void PenduleDouble::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}


/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE PenduleRessort                  ###
###                                                                          ###
##############################################################################*/

//constructeur
PenduleRessort::PenduleRessort(const std::initializer_list<double>& liP,
                               const std::initializer_list<double>& liQ,
                               const Vecteur3D& a, const Vecteur3D& O,
                               double masse, double longueur, double raideur,
                               bool phase, SupportADessin* support)
                              : Oscillateur(liP, liQ, a, O, phase, support),
                                m(masse), L(longueur), k(raideur)
                              {
                                if(a*g!=0){
                                  Erreur err("initialisation PenduleRessort", "PenduleRessort::PenduleRessort(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
                                             "L'axe du pendule double doit être orthogonal au vecteur g.");
                                  throw err;
                                }
                                if(P.taille()!=2){
                                  Erreur err("initialisation PenduleRessort", "PenduleRessort::PenduleRessort(const initializer_list<double>&, const initializer_list<double>&, const Vecteur3D&, const Vecteur3D&, double, double, double, bool, SupportADessin*)",
                                             "Le pendule double doit avoir deux paramètres (et leur dérivée temporelle). Ici : "+to_string(P.taille())+"paramètre(s).");
                                  throw err;
                                }
                              }


unique_ptr<PenduleRessort> PenduleRessort::clone() const{
  return unique_ptr<PenduleRessort>(new PenduleRessort(*this));
}

unique_ptr<Oscillateur> PenduleRessort::copie() const{
  return clone();
}

// retourne l'angle de nutation
double PenduleRessort::get_angleNutation(bool degre) const{
  double angle(0);
  double x(P[0]), z(-P[1]);
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

//retourne position d'un pendule
Vecteur3D PenduleRessort::position() const {
  Vecteur3D retour(O + P[0]*a + P[1]*(~g));
  return retour;
}

//fonction d'évolution
Vecteur PenduleRessort::f(const double& t) const{
  Vecteur retour(Vecteur({0,g.norme()}) - (k/m)*(1-(L/P.norme()))*P);
  return retour;
}

// permet l'affichage d'un oscillateur de façon standardisée
ostream& PenduleRessort::affiche(ostream& sortie) const{
  sortie << "# PenduleRessort :" << endl;
  sortie << P << " # parametre (x et y)" << endl;
  sortie << Q << " # vitesses" << endl;
  sortie << position() << "# position" << endl;
  if(phase) sortie << PQ() << " # phase" << endl;
  return sortie;
}

void PenduleRessort::dessine() const{
  if(support!=nullptr){
    support->dessine(*this);
  }
}
