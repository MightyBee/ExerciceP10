#pragma once
#include <initializer_list>
#include <memory>
#include "Vecteur.h"
#include "Dessinable.h"

class Oscillateur : public Dessinable {
  public:
    //constructeurs
    explicit Oscillateur(const std::initializer_list<double>& liP, //construit un Oscillateur avec son origine et son axe
                         const std::initializer_list<double>& liQ,
                         const Vecteur3D& a=Vecteur3D(1,0,0),
                         const Vecteur3D& O=Vecteur3D(0,0,0),
                         SupportADessin* support=nullptr);
    virtual ~Oscillateur(){}
    virtual std::unique_ptr<Oscillateur> copie() const = 0;
    //accesseurs
    Vecteur get_P() const; // retourne le vecteur des paramètres de l'oscillateur
    Vecteur get_Q() const; // retourne le vecteur des "vitesses" de l'oscillateur
    Vecteur3D get_a() const;
    Vecteur3D get_O() const;
    virtual double get_anglePrecession(bool degre=false) const;
    virtual double get_angleNutation(bool degre=false) const = 0;
    virtual double get_angleRotPro(bool degre=false) const = 0;

    //manipulateurs
    void set_P(const Vecteur& p); // permet de modifier l'intégralité des paramètres
    void set_P(unsigned int n, double newValeur); // permet de modifier un des paramètre
    void set_Q(const Vecteur& q); // permet de modifier l'intégralité des "vitesses"
    void set_Q(unsigned int n, double newValeur); // permet de modifier une des "vitesses"

    //autres opérations
    virtual Vecteur f(const double& t=0) const = 0; // fonction P''=f(t,P,P') : détermine le mouvement de l'oscillateur
    virtual std::ostream& affiche(std::ostream& sortie) const; // permet permet d'afficher le vecteur par composants sur un flot de sortie

  protected:
    //attributs
    Vecteur P; // vecteur contenant les paramètres de l'oscillateur
    Vecteur Q; // vecteur contenenat les dérivées des paramètres
    Vecteur3D a; //vecteur3D représentant la direction principale (axe)
    Vecteur3D O; //vecteur3D représentant la position de référence (origine)
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
                   SupportADessin* support=nullptr);
  virtual ~Pendule(){}
  std::unique_ptr<Pendule> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres opérations
  virtual Vecteur f(const double& t) const override;
  Vecteur3D position() const;
  double get_L() const{return L;}
  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet permet d'afficher le vecteur par composants sur un flot de sortie

private:
  double L;
  double m;
  double frott;
};

class Ressort :public Oscillateur{
public:
  //constructeur
  explicit Ressort(const std::initializer_list<double>& liP={0},
                   const std::initializer_list<double>& liQ={0},
                   const Vecteur3D& a=Vecteur3D(1,0,0),
                   const Vecteur3D& O=Vecteur3D(0,0,0),
                   double raideur=1, double masse=1, double frottement=0,
                   SupportADessin* support=nullptr);
  virtual ~Ressort(){}
  std::unique_ptr<Ressort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autre fonctions
  virtual Vecteur f(const double& t) const override;
  Vecteur3D position() const;
  double get_x() const{return P.get_coord(1);}
  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet permet d'afficher le vecteur par composants sur un flot de sortie

private:
  double k;
  double m;
  double frott;
};

class Chariot :public Oscillateur{
public:
  explicit Chariot(const std::initializer_list<double>& liP={0,0},
                   const std::initializer_list<double>& liQ={0,0},
                   const Vecteur3D& a=Vecteur3D(1,0,0),
                   const Vecteur3D& O=Vecteur3D(0,0,0),
                   double raideur=1, double masseRessort=1, double frottRessort=0,
                   double longueur=1, double massePendule=1, double frottPendule=0,
                   SupportADessin* support=nullptr);
  virtual ~Chariot(){}
  std::unique_ptr<Chariot> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;

  virtual Vecteur f(const double& t) const override;
  Vecteur3D posC()const; // position du chariot
  Vecteur3D posP()const; //position du pendule
  double get_L() const{return L;}
  double get_x() const{return P.get_coord(1);}
  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override; // permet permet d'afficher le vecteur par composants sur un flot de sortie


private:
  //chariot (ressort)
  double frott1;
  double m1;
  double k;
  //pendule
  double frott2;
  double m2;
  double L;

};

//simon


class Torsion : public Oscillateur{
public:
  //constructeur - destructeur
  explicit Torsion(const std::initializer_list<double>& liP={0},
          const std::initializer_list<double>& liQ={0},
          const Vecteur3D& a=Vecteur3D(1,0,0),
          const Vecteur3D& O=Vecteur3D(0,0,0),
          double frottement = 0.0,
          double inertie = 1.0,
          double C_torsion = 1.0,
          SupportADessin* support=nullptr);
  virtual ~Torsion(){}
  std::unique_ptr<Torsion> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override;

  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override;

private:
  double I;
  double C;
  double frott;
};


class PenduleDouble : public Oscillateur{
public:
  //constructeur - destructeur
  explicit PenduleDouble(const std::initializer_list<double>& liP={0,0},
                         const std::initializer_list<double>& liQ={0,0},
                         const Vecteur3D& a=Vecteur3D(1,0,0),
                         const Vecteur3D& O=Vecteur3D(0,0,0),
                         double masse1 = 1.0, double longueur1 = 1.0,
                         double masse2 = 1.0, double longueur2 = 1.0,
                         SupportADessin* support=nullptr);
  virtual ~PenduleDouble(){}
  std::unique_ptr<PenduleDouble> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override;
  Vecteur3D pos1() const;
  Vecteur3D pos2() const;
  double get_L1() const{return L1;}
  double get_L2() const{return L2;}

  virtual double get_angleNutation(bool degre=false) const override;
  double get_angleNutation2(bool degre=false) const;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override;

private:
  double m1;
  double m2;
  double L1;
  double L2;
};


class PenduleRessort : public Oscillateur{
public:
  //constructeur - destructeur
  explicit PenduleRessort(const std::initializer_list<double>& liP={0,0},
                          const std::initializer_list<double>& liQ={0,0},
                          const Vecteur3D& a=Vecteur3D(1,0,0),
                          const Vecteur3D& O=Vecteur3D(0,0,0),
                          double masse=1.0, double longueur=1.0, double raideur=1.0,
                          SupportADessin* support=nullptr);
  virtual ~PenduleRessort(){}
  std::unique_ptr<PenduleRessort> clone() const;
  virtual std::unique_ptr<Oscillateur> copie() const override;
  virtual void dessine() const override;
  //autres fonctions
  virtual Vecteur f(const double& t) const override;
  virtual Vecteur3D position() const;
  double get_L() const{return P.norme();}

  virtual double get_angleNutation(bool degre=false) const override;
  virtual double get_angleRotPro(bool degre=false) const override;
  virtual std::ostream& affiche(std::ostream& sortie) const override;

private:
  double m;
  double L;
  double k;
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
