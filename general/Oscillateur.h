#pragma once
#include <initializer_list>
#include <memory>
#include "Vecteur.h"
#include "Dessinable.h"


/*##############################################################################
###                                                                          ###
###                           CLASSE Oscillateur                             ###
###                                                                          ###
##############################################################################*/

//super-classe Oscillateur abstraite
class Oscillateur : public Dessinable {
  public:
    //constructeurs
    explicit Oscillateur(const std::initializer_list<double>& liP, //construit un Oscillateur avec son origine et son axe
                         const std::initializer_list<double>& liQ,
                         const Vecteur3D& a=Vecteur3D(1,0,0),
                         const Vecteur3D& O=Vecteur3D(0,0,0),
                         bool phase=false,
                         SupportADessin* support=nullptr);
    //destructeur
    virtual ~Oscillateur(){}
    //copie polymorphique
    virtual std::unique_ptr<Oscillateur> copie() const = 0;
    //accesseurs
    Vecteur   get_P() const        {return P;} // retourne le vecteur des paramètres de l'oscillateur
    Vecteur   get_Q() const        {return Q;} // retourne le vecteur des "vitesses" de l'oscillateur
    Vecteur3D get_a() const        {return a;} // retourne le vecteur3d de l'axe
    Vecteur3D get_O() const        {return O;} // retourne le vecteur3d de l'origine
    bool      getPhaseBool() const {return phase;} // retourne la valeur du booléen phase (s'il faut afficher l'espace des phases ou pas)
    Vecteur   PQ() const           {return Vecteur({P[0],Q[0]});} // retourne un vecteur de dimension 2 : 1er paramètre et dérivée du 1er paramètre
    virtual double get_anglePrecession(bool degre=false) const; // retourne l'angle de précession
    virtual double get_angleNutation(bool degre=false) const = 0; // retourne l'angle de nutation
    virtual double get_angleRotPro(bool degre=false) const = 0; // retourne l'angle de rotation propre
    //manipulateurs
    void set_P(const Vecteur& p); // permet de modifier l'intégralité des paramètres
    void set_P(unsigned int n, double newValeur); // permet de modifier un des paramètre
    void set_Q(const Vecteur& q); // permet de modifier l'intégralité des "vitesses"
    void set_Q(unsigned int n, double newValeur); // permet de modifier une des "vitesses"
    void changePhase(); // permet de changer l'attribut booléen phase
    //autres méthodes
    virtual Vecteur f(const double& t=0) const = 0; // fonction P''=f(t,P,P') : détermine le mouvement de l'oscillateur
    virtual std::ostream& affiche(std::ostream& sortie) const; // permet d'afficher l'oscillateur (P,Q,...) sur un flot de sortie

  protected:
    //########## attributs ##########//
    Vecteur P; // vecteur contenant les paramètres de l'oscillateur
    Vecteur Q; // vecteur contenenat les dérivées des paramètres
    Vecteur3D a; // vecteur3D représentant la direction principale (axe)
    Vecteur3D O; // vecteur3D représentant la position de référence (origine)
    bool phase; // si on veut le dessin des phases ou pas
};

std::ostream& operator<<(std::ostream& sortie, const Oscillateur& osc); // permet l'affichage standard : sortie << oscillateur;


/*##############################################################################
###                                                                          ###
###                             CLASSE Pendule                               ###
###                                                                          ###
##############################################################################*/

class Pendule :public Oscillateur{
public:
  //constructeur
  explicit Pendule(const std::initializer_list<double>& liP={0.0},
                   const std::initializer_list<double>& liQ={0.0},
                   const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
                   const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
                   double longueur=1.0, double masse=1.0,
                   double frottement=0.0, bool phase=false,
                   SupportADessin* support=nullptr); /* construit un pendule avec son paramètre, la dérivée du paramètre,
                                                      son axe, son origine, sa longueur, sa masse, un coefficient de frottement et un support à dessin.
                                                      ce constructeur fait office de constructeur par défaut*/
  //destructeur
  virtual ~Pendule(){}
  //copie polymorphique
  std::unique_ptr<Pendule> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  //accesseurs
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre
  Vecteur3D position() const; // retourne le vecteur3d indiquant la position du pendule dans l'espace
  double vitesse() const{return abs(L*Q[0]);} // retourne la vitesse (pas la dérivée du paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double get_L() const{return L;} // retourne la longueur du pendule
  //autres méthodes
  virtual Vecteur f(const double& t) const override; //fonction déterminante du mouvement du pendule
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le pendule (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un pendule ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  double m; // masse du pendule
  double L; // longuer du pendule
  double frott; // coefficient de frottement
};


/*##############################################################################
###                                                                          ###
###                             CLASSE Ressort                               ###
###                                                                          ###
##############################################################################*/

class Ressort :public Oscillateur{
public:
  //constructeur
  explicit Ressort(const std::initializer_list<double>& liP={0.0},
                   const std::initializer_list<double>& liQ={0.0},
                   const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
                   const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
                   double raideur=1.0, double masse=1.0,
                   double frottement=0.0, bool phase=false,
                   SupportADessin* support=nullptr); /* construit un ressort avec son paramètre, la dérivée du paramètre,
                                                        son axe, son origine, sa constante de raideur, sa masse, un coefficient de Frottement
                                                        et un support à dessin.
                                                        Ce constructeur fait office de constructeur par défaut */

  //destructeur
  virtual ~Ressort(){}
  //copie polymorphique
  std::unique_ptr<Ressort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  //accesseurs
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre
  Vecteur3D position() const; // retourne la position du ressort
  double vitesse() const{return abs(Q[0]);} // retourne la vitesse
  double get_x() const{return P[0];} // retourne le paramètre
  //autres méthodes
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement du ressort
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le ressort (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un ressort ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  double m; // masse du ressort
  double k; // constante de raideur
  double frott; // constante de frottement
};


/*##############################################################################
###                                                                          ###
###                             CLASSE Torsion                               ###
###                                                                          ###
##############################################################################*/

class Torsion : public Oscillateur{
public:
  //constructeur
  explicit Torsion(const std::initializer_list<double>& liP={0.0},
          const std::initializer_list<double>& liQ={0.0},
          const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
          const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
          double frottement=0.0, double inertie=1.0,
          double C_torsion=1.0, bool phase=false,
          SupportADessin* support=nullptr); /*construit un pendule de torsion avec paramètre, dérivée, axe, origine,
                                              frottement, moment d'inertie, constante de torsion et un support à dessin.
                                              Ce constructeur fait office de constructeur par défaut*/
  // destructeur
  virtual ~Torsion(){}
  //copie polymorphique
  std::unique_ptr<Torsion> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  //accesseurs
  virtual double get_angleNutation(bool degre=false) const override; // angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // angle de rotation propre
  double vitesse() const{return abs(Q[0]);} // retourne la vitesse angulaire
  //autres méthodes
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le pendule de torsion (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un pendule de torsion ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  double I; // moment d'inertie
  double C; // constante de torsion
  double frott; // frottement
};


/*##############################################################################
###                                                                          ###
###                             CLASSE Chariot                               ###
###                                                                          ###
##############################################################################*/

class Chariot :public Oscillateur{
public:
  //constructeur
  explicit Chariot(const std::initializer_list<double>& liP={0.0,0.0},
                   const std::initializer_list<double>& liQ={0.0,0.0},
                   const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
                   const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
                   double raideur=1.0 , double masseRessort=1.0, double frottRessort=0.0,
                   double longueur=1.0, double massePendule=1.0, double frottPendule=0.0,
                   bool phase=false, SupportADessin* support=nullptr); /*construit un chariot avec paramètres, dérivées, axe, origine,
                                                      constante de raideur du ressort, masse du ressort, frottement du ressort,
                                                      longueur du pendule, masse du pendule, frottement du pendule.
                                                      Ce constructeur fait office de constructeur par defaut*/
  //destructeur
  virtual ~Chariot(){}
  //copie polymorphique
  std::unique_ptr<Chariot> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  //accesseurs
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre
  Vecteur3D posC()const; // position du chariot
  Vecteur3D posP()const; // position du pendule
  double vitC() const{return abs(Q[0]);} // retourne la vitesse du chariot (pas la dérivée du 1er paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double vitP() const; // retourne la vitesse du pendule (pas la dérivée du 2eme paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double get_x() const{return P[0];} // paramètre
  double get_L() const{return L;} // retourne la longueur du pendule
  //autres méthodes
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement du chariot
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le chariot (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un chariot ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  //chariot (ressort)
  double m1; // masse du ressort
  double k; // constante de raideur du ressort
  double frott1; // constante de frottement du ressort
  //pendule
  double m2; // masse du pendule
  double L; // longueur du pendule
  double frott2; // constante de frottement du pendule
};


/*##############################################################################
###                                                                          ###
###                          CLASSE PenduleDouble                            ###
###                                                                          ###
##############################################################################*/

class PenduleDouble : public Oscillateur{
public:
  //constructeur
  explicit PenduleDouble(const std::initializer_list<double>& liP={0.0,0.0},
                         const std::initializer_list<double>& liQ={0.0,0.0},
                         const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
                         const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
                         double masse1=1.0, double longueur1=1.0,
                         double masse2=1.0, double longueur2=1.0,
                         bool phase=false, SupportADessin* support=nullptr);
  //destructeur
  virtual ~PenduleDouble(){}
  //copie polymorphique
  std::unique_ptr<PenduleDouble> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  //accesseurs
  virtual double get_angleNutation(bool degre=false) const override; // premier angle de nutation
  double get_angleNutation2(bool degre=false) const; // deuxième angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // angle de rotation propre
  Vecteur3D pos1() const; // position du premier pendule
  Vecteur3D pos2() const; // position du deuxième pendule
  double vit1() const{return abs(L1*Q[0]);} // retourne la vitesse du 1er pendule (pas la dérivée du 1er paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double vit2() const; // retourne la vitesse du 2ème pendule (pas la dérivée du 2eme paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double get_L1() const{return L1;} // retourne longueur du premier pendule
  double get_L2() const{return L2;} // retourne longueur du deuxième pendule
  //autres méthodes
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le pendule double (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un pendule double ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  //1er pendule
  double m1; // première masse
  double L1; // première longueur
  //2eme pendule
  double m2; // deuxième masse
  double L2; // deuxième longueur
};


/*##############################################################################
###                                                                          ###
###                         CLASSE PenduleRessort                            ###
###                                                                          ###
##############################################################################*/

class PenduleRessort : public Oscillateur{
public:
  //constructeur
  explicit PenduleRessort(const std::initializer_list<double>& liP={0.0,0.0},
                          const std::initializer_list<double>& liQ={0.0,0.0},
                          const Vecteur3D& a=Vecteur3D(1.0,0.0,0.0),
                          const Vecteur3D& O=Vecteur3D(0.0,0.0,0.0),
                          double masse=1.0, double longueur=1.0,
                          double raideur=1.0, bool phase=false,
                          SupportADessin* support=nullptr); /* construit un pendule-ressort avec paramètres, dérivées, axe, origine,
                                                               masse, longueur, raideur et support à dessin.
                                                               Ce constructeur fait office de constructeur par defaut*/
  //destructeur
  virtual ~PenduleRessort(){}
  //copie polymorphique
  std::unique_ptr<PenduleRessort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
//accesseurs
  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  Vecteur3D position() const; // retourne la position du pendule-ressort
  double vitesse() const{return sqrt(pow(Q[0],2)+pow(Q[1],2));} // retourne la vitesse (pas la dérivée du paramètre, la "vraie" vitesse de déplacement dans l'espace)
  double get_L() const{return P.norme();}
  //autres méthodes
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le pendule-ressort (P,Q,position,...) sur un flot de sortie
  virtual void dessine() const override; // méthode appelée pour dessiner un pendule-ressort ("dessin" différent selon son support à dessin)

private:
  //########## attributs ##########//
  double m; // masse
  double L; // longueur
  double k; // constante de raideur
};
