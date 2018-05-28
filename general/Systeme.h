#pragma once
#include <memory>
#include <vector>
#include "Dessinable.h"
#include "Oscillateur.h"
#include "Integrateur.h"


class Systeme : public Dessinable {
  public:
    //constructeurs
    Systeme(SupportADessin* sup, Integrateur const& integrat, double t=0);
    Systeme(const Systeme&) =delete;
    //destructeur
    virtual ~Systeme(){}
    //opérateur d'affectation
    Systeme& operator=(const Systeme&) =delete;
    //accesseurs
    const std::vector<std::unique_ptr<Oscillateur>>& get_col() const {return collection;} //retourne une référence constante sur la collection
    size_t taille() const{return collection.size();} // retourne la taille du système : le nb d'oscillateurs
    bool getPhaseBool(unsigned int i) const; // pour savoir on veut afficher le portrait des phases du (i+1)ème oscillateur
    //manipulateurs
    void add(Oscillateur const& osc); //rajoute un oscillateur au système
    void changeIntegrateur(int i); // permet de choisir parmis les trois intégrateurs implémentés
    void changePhase(unsigned int i); // afficher/ne plus afficher le portrait des phases
    //autres fonctions
    void evolue(double dt); // évolution du système par l'intégrateur
    std::ostream& affiche(std::ostream& sortie) const; // permet d'afficher le systeme sur un flot de sortie
    virtual void dessine() const override; // méthode appelée pour dessiner un système (dessin s'adaptant au support)
  private:
    //########### attributs ###########//
    std::unique_ptr<Integrateur> integrateur; //intégrateur
    double temps; // temps
    std::vector<std::unique_ptr<Oscillateur>> collection; // collection hétérogène d'oscillateurs
};

std::ostream& operator<<(std::ostream& sortie, const Systeme& sys); // permet l'affichage standard : sortie << systeme;
