#pragma once
#include <memory>
#include <vector>
#include "Dessinable.h"
#include "Oscillateur.h"
#include "Integrateur.h"


class Systeme : public Dessinable {
  public:
    Systeme(SupportADessin* sup, Integrateur const& integrat, double t=0);
    virtual ~Systeme(){}
    void add(Oscillateur const& osc);          //TODO methode remove ?
    void initialize();
    void evolue(double dt=0.1);
    const std::vector<std::unique_ptr<Oscillateur>>& get_col() const {return collection;}
    virtual void dessine() const override;
    void phase() const;
    void phase(unsigned int n) const;
    std::ostream& affiche(std::ostream& sortie) const; // permet permet d'afficher le systeme sur un flot de sortie
  private:
    std::unique_ptr<Integrateur> integrateur;
    double temps;
    std::vector<std::unique_ptr<Oscillateur>> collection;
};

std::ostream& operator<<(std::ostream& sortie, const Systeme& sys); // permet l'affichage standard : sortie << systeme;
