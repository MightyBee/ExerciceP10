#pragma once
#include <string>

class Erreur{
  public:
    Erreur(std::string type="non spécifié", std::string fonction="non spécifiée", std::string dscrpt="Une erreur est survenue");
    void affiche(std::string program) const;
    void add_fct(std::string newFct);
    void set_fct(std::string newFct);
    void set_dscrpt(std::string dscrpt);
  private:
    std::string type;
	  std::string fct;
	  std::string description;
};
