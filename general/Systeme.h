#pragma once
#include <memory>
#include <vector>
#include "Dessinable.h"
#include "Oscillateur.h"
#include "Integrateur.h"


class Systeme : public Dessinable {
  public:
    //constructeur
    Systeme(SupportADessin* sup, Integrateur const& integrat, double t=0);
    //destructeur
    virtual ~Systeme(){}
    //autres fonctions
    void add(Oscillateur const& osc); //rajoute un oscillateur
    void evolue(double dt=0.1); // évolution du système par l'intégrateur
    const std::vector<std::unique_ptr<Oscillateur>>& get_col() const {return collection;}
    size_t taille() const{return collection.size();}
    virtual void dessine() const override;
    void phase() const;
    void phase(unsigned int n) const;
    void changeIntegrateur(int i);
    std::ostream& affiche(std::ostream& sortie) const; // permet d'afficher le systeme sur un flot de sortie
  private:
    //attributs
    std::unique_ptr<Integrateur> integrateur; //intégrateur
    double temps; // temps
    std::vector<std::unique_ptr<Oscillateur>> collection; // collection hétérogène d'oscillateurs
};

std::ostream& operator<<(std::ostream& sortie, const Systeme& sys); // permet l'affichage standard : sortie << systeme;
