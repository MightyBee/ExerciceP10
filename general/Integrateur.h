#pragma once
#include <memory>
#include "Oscillateur.h"

// super-classe Integrateur, abstraite (pas de méthodes d'intégration numérique spécifiées)
class Integrateur{
  public:

    virtual ~Integrateur(){}

    virtual std::unique_ptr<Integrateur> copie() const = 0;
    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt) = 0; // méthode qui fait évoluer l'intégrateur d'un pas de temps
};

// classe héritée d'Integrateur, (méthode d'intégration numérique : Euler-Cromer)
class IntegrateurEulerCromer : public Integrateur{
  public:
    virtual ~IntegrateurEulerCromer(){}

    std::unique_ptr<IntegrateurEulerCromer> clone() const;

    virtual std::unique_ptr<Integrateur> copie() const override;

    // autres méthodes
    virtual void evolue(Oscillateur& osc, double dt=0.1) override; // spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration d'Euler-Cromer
};

/*
class IntergrateurRungeKutta : public Integrateur{
  public:
    virtual void evolue() override;
  private:
    //uuu
};*/
