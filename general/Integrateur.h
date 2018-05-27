#pragma once
#include <memory>
#include "Oscillateur.h"

// super-classe Integrateur, abstraite (pas de méthodes d'intégration numérique spécifiées)
class Integrateur{
  public:
    // destructeur
    virtual ~Integrateur(){}

    virtual std::unique_ptr<Integrateur> copie() const = 0;
    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt, double t) const = 0; // méthode qui fait évoluer l'intégrateur d'un pas de temps
};

// classe héritée d'Integrateur, (méthode d'intégration numérique : Euler-Cromer)
class IntegrateurEulerCromer : public Integrateur{
  public:
    //destructeur
    virtual ~IntegrateurEulerCromer(){}

    std::unique_ptr<IntegrateurEulerCromer> clone() const;

    virtual std::unique_ptr<Integrateur> copie() const override{return clone();}

    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt=0.1, double t=0) const override; // spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration d'Euler-Cromer
};


// classe héritée d'Integrateur, (méthode d'intégration numérique : Newmark)
class IntegrateurNewmark : public Integrateur{
  public:
    //destructeur
    virtual ~IntegrateurNewmark(){}

    std::unique_ptr<IntegrateurNewmark> clone() const;

    virtual std::unique_ptr<Integrateur> copie() const override{return clone();}

    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt=0.1, double t=0) const override; // spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration de Newmark
};



// classe héritée d'Integrateur, (méthode d'intégration numérique : Runge-Kutta)
class IntegrateurRungeKutta : public Integrateur{
  public:
    //destructeur
    virtual ~IntegrateurRungeKutta(){}

    std::unique_ptr<IntegrateurRungeKutta> clone() const;

    virtual std::unique_ptr<Integrateur> copie() const override{return clone();}

    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt=0.1, double t=0) const override; // spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration de Runge-Kutta

};
