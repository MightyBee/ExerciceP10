#pragma once
#include <initializer_list>
#include <memory>
#include "Vecteur.h"
#include "Dessinable.h"

//super-classe Oscillateur abstraite
class Oscillateur : public Dessinable {
  public:
    //constructeurs
    explicit Oscillateur(const std::initializer_list<double>& liP, //construit un Oscillateur avec son origine et son axe
                         const std::initializer_list<double>& liQ,
                         const Vecteur3D& a=Vecteur3D(1,0,0),
                         const Vecteur3D& O=Vecteur3D(0,0,0),
                         SupportADessin* support=nullptr);
    //destructeur
    virtual ~Oscillateur(){}
    virtual std::unique_ptr<Oscillateur> copie() const = 0;
    //accesseurs
    Vecteur get_P() const; // retourne le vecteur des paramètres de l'oscillateur
    Vecteur get_Q() const; // retourne le vecteur des "vitesses" de l'oscillateur
    Vecteur3D get_a() const; // retourne le vecteur3d de l'axe
    Vecteur3D get_O() const; // retourne le vecteur3d de l'origine
    virtual double get_anglePrecession(bool degre=false) const; // retourne l'angle de précession
    virtual double get_angleNutation(bool degre=false) const = 0; // retourne l'angle de nutation
    virtual double get_angleRotPro(bool degre=false) const = 0; // retourne l'angle de rotation propre

    //manipulateurs
    void set_P(const Vecteur& p); // permet de modifier l'intégralité des paramètres
    void set_P(unsigned int n, double newValeur); // permet de modifier un des paramètre
    void set_Q(const Vecteur& q); // permet de modifier l'intégralité des "vitesses"
    void set_Q(unsigned int n, double newValeur); // permet de modifier une des "vitesses"

    //autres opérations
    Vecteur PQ() const;
    void phase(Integrateur const& integrat) const;
    virtual Vecteur f(const double& t=0) const = 0; // fonction P''=f(t,P,P') : détermine le mouvement de l'oscillateur
    virtual std::ostream& affiche(std::ostream& sortie) const; // permet d'afficher le vecteur par composants sur un flot de sortie

  protected:
    //attributs
    Vecteur P; // vecteur contenant les paramètres de l'oscillateur
    Vecteur Q; // vecteur contenenat les dérivées des paramètres
    Vecteur3D a; // vecteur3D représentant la direction principale (axe)
    Vecteur3D O; // vecteur3D représentant la position de référence (origine)
};

std::ostream& operator<<(std::ostream& sortie, const Oscillateur& osc); // permet l'affichage standard : sortie << oscillateur;


class Pendule :public Oscillateur{
public:
  //constructeur
  explicit Pendule(const std::initializer_list<double>& liP={0},
                   const std::initializer_list<double>& liQ={0},
                   const Vecteur3D& a=Vecteur3D(1,0,0),
                   const Vecteur3D& O=Vecteur3D(0,0,0),
                   double longueur=1, double masse=1, double frottement=0,
                   SupportADessin* support=nullptr); /* construit un pendule avec son paramètre, la dérivée du paramètre,
                                                      son axe, son origine, sa longueur, sa masse, un coefficient de frottement et un support à dessin.
                                                      ce constructeur fait office de constructeur par défaut*/
  //destructeur
  virtual ~Pendule(){}
  std::unique_ptr<Pendule> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres opérations
  virtual Vecteur f(const double& t) const override; //fonction déterminante du mouvement du pendule
  Vecteur3D position() const; // retourne le vecteur3d indiquant la position du pendule
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le vecteur par composants sur un flot de sortie
  //accesseurs
  double vitesse() const{return abs(L*Q.get_coord(1));} // retourne la vitesse
  double get_L() const{return L;} // retourne la longueur du pendule
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre

private:
  //attributs
  double L; // longuer du pendule
  double m; // masse du pendule
  double frott; // coefficient de frottement
};


class Ressort :public Oscillateur{
public:
  //constructeur
  explicit Ressort(const std::initializer_list<double>& liP={0},
                   const std::initializer_list<double>& liQ={0},
                   const Vecteur3D& a=Vecteur3D(1,0,0),
                   const Vecteur3D& O=Vecteur3D(0,0,0),
                   double raideur=1, double masse=1, double frottement=0,
                   SupportADessin* support=nullptr); /* construit un ressort avec son paramètre, la dérivée du paramètre,
                                                        son axe, son origine, sa constante de raideur, sa masse, un coefficient de Frottement
                                                        et un support à dessin.
                                                        Ce constructeur fait office de constructeur par défaut */

  //destructeur
  virtual ~Ressort(){}
  std::unique_ptr<Ressort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autre fonctions
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement du ressort
  Vecteur3D position() const; // retourne la position du ressort
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet permet d'afficher le vecteur par composants sur un flot de sortie

  //accesseurs
  double vitesse() const{return abs(Q.get_coord(1));} // retourne la vitesse
  double get_x() const{return P.get_coord(1);} // retourne le paramètre
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre

private:
  //attributs
  double k; // constante de Raideur
  double m; // masse du ressort
  double frott; // constante de frottement
};


class Chariot :public Oscillateur{
public:
  //constructeur
  explicit Chariot(const std::initializer_list<double>& liP={0,0},
                   const std::initializer_list<double>& liQ={0,0},
                   const Vecteur3D& a=Vecteur3D(1,0,0),
                   const Vecteur3D& O=Vecteur3D(0,0,0),
                   double raideur=1, double masseRessort=1, double frottRessort=0,
                   double longueur=1, double massePendule=1, double frottPendule=0,
                   SupportADessin* support=nullptr); /*construit un chariot avec paramètres, dérivées, axe, origine,
                                                      constante de raideur du ressort, masse du ressort, frottement du ressort,
                                                      longueur du pendule, masse du pendule, frottement du pendule.
                                                      Ce constructeur fait office de constructeur par defaut*/
  //destructeur
  virtual ~Chariot(){}
  std::unique_ptr<Chariot> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement du chariot
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet permet d'afficher le vecteur par composants sur un flot de sortie
  Vecteur3D posC()const; // position du chariot
  Vecteur3D posP()const; // position du pendule
  //accesseurs
  double vitC() const{return abs(Q.get_coord(1));} //vitesse du chariot
  double vitP() const; // vitesse du pendule
  double get_L() const{return L;} // retourne la longueur du pendule
  double get_x() const{return P.get_coord(1);} // paramètre
  virtual double get_angleNutation(bool degre=false) const override; // retourne l'angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // retourne l'angle de rotation propre

private:
  //chariot (ressort)
  double frott1; // constante de frottement du ressort
  double m1; // masse du ressort
  double k; // constante de raideur du ressort
  //pendule
  double frott2; // constante de frottement du pendule
  double m2; // masse du pendule
  double L; // longueur du pendule

};


class Torsion : public Oscillateur{
public:
  //constructeur
  explicit Torsion(const std::initializer_list<double>& liP={0},
          const std::initializer_list<double>& liQ={0},
          const Vecteur3D& a=Vecteur3D(1,0,0),
          const Vecteur3D& O=Vecteur3D(0,0,0),
          double frottement = 0.0,
          double inertie = 1.0,
          double C_torsion = 1.0,
          SupportADessin* support=nullptr); /*construit un pendule de torsion avec paramètre, dérivée, axe, origine,
                                              frottement, moment d'inertie, constante de torsion et un support à dessin.
                                              Ce constructeur fait office de constructeur par défaut*/
  // destructeur
  virtual ~Torsion(){}
  std::unique_ptr<Torsion> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le vecteur par composants sur un flot de sortie
  //accesseurs
  double vitesse() const{return abs(Q.get_coord(1));} // vittesse
  virtual double get_angleNutation(bool degre=false) const override; // angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // angle de rotation propre

private:
  //attributs
  double I; // moment d'inertie
  double C; // constante de torsion
  double frott; // frottement
};


class PenduleDouble : public Oscillateur{
public:
  //constructeur
  explicit PenduleDouble(const std::initializer_list<double>& liP={0,0},
                         const std::initializer_list<double>& liQ={0,0},
                         const Vecteur3D& a=Vecteur3D(1,0,0),
                         const Vecteur3D& O=Vecteur3D(0,0,0),
                         double masse1 = 1.0, double longueur1 = 1.0,
                         double masse2 = 1.0, double longueur2 = 1.0,
                         SupportADessin* support=nullptr);
  //destructeur
  virtual ~PenduleDouble(){}
  std::unique_ptr<PenduleDouble> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  Vecteur3D pos1() const; // position du premier pendule
  double vit1() const{return abs(L1*Q.get_coord(1));} // vitesse du premier pendule
  Vecteur3D pos2() const; // position du deuxième pendule
  double vit2() const; // vitesse du deuxième pendule
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet d'afficher le vecteur par composants sur un flot de sortie
  //accesseurs
  double get_L1() const{return L1;} // retourne longueur du premier pendule
  double get_L2() const{return L2;} // retourne longueur du deuxième pendule
  virtual double get_angleNutation(bool degre=false) const override; // premier angle de nutation
  double get_angleNutation2(bool degre=false) const; // deuxième angle de nutation
  virtual double get_angleRotPro(bool degre=false) const override; // angle de rotation propre

private:
  //attributs
  double m1; //première masse
  double m2; // deuxième masse
  double L1; // première longueur
  double L2; // deuxième longueur
};


class PenduleRessort : public Oscillateur{
public:
  //constructeur
  explicit PenduleRessort(const std::initializer_list<double>& liP={0,0},
                          const std::initializer_list<double>& liQ={0,0},
                          const Vecteur3D& a=Vecteur3D(1,0,0),
                          const Vecteur3D& O=Vecteur3D(0,0,0),
                          double masse=1.0, double longueur=1.0, double raideur=1.0,
                          SupportADessin* support=nullptr); /* construit un pendule-ressort avec paramètres, dérivées, axe, origine,
                                                               masse, longueur, raideur et support à dessin.
                                                               Ce constructeur fait office de constructeur par defaut*/
  //destructeur
  virtual ~PenduleRessort(){}
  std::unique_ptr<PenduleRessort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override; // fonction déterminante du mouvement
  Vecteur3D position() const; // retourne la position du pendule-ressort
  double vitesse() const{return sqrt(pow(Q.get_coord(1),2)+pow(Q.get_coord(2),2));} // vitesse
  //accesseurs
  double get_L() const{return P.norme();}
  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override;

private:
  //attributs
  double m; // masse
  double L; // longueur
  double k; // constante de raideur
};

/*class PendulesRelies : public Oscillateur{
public:
  //constructeur - destructeur
  explicit PendulesRelies(const std::initializer_list<double>& liP={0,0},
                          const std::initializer_list<double>& liQ={0,0},
                          const Vecteur3D& a=Vecteur3D(1,0,0),
                          const Vecteur3D& O=Vecteur3D(0,0,0),
                          double masse1=1.0, double longueur1=1.0, double raideur=1.0,
                          double masse2=1.0, double longueur2=1.0, double distance=1.0,
                          double dist_accroche1, double dist_accroche2,
                          SupportADessin* support=nullptr);
  virtual ~PendulesRelies(){}

  //autres fonctions
  virtual Vecteur f(const double& t) const override;

private:
  double m1;
  double m2;
  double k;
  double d;
  double L1;
  double L2;
  double a1;
  double a2;
};*/
