#pragma once

class Systeme;
class Oscillateur;
class Pendule;
class Ressort;
class Torsion;
class Chariot;
class PenduleDouble;
class PenduleRessort;

class Integrateur;

class SupportADessin{
 public:
  //destructeur
  virtual ~SupportADessin() {}
  //méthodes pour dessiner les différents oscillateurs
  virtual void dessine(Systeme const&) = 0;
  virtual void dessine(Pendule const&) = 0;
  virtual void dessine(Ressort const&) = 0;
  virtual void dessine(Torsion const&) = 0;
  virtual void dessine(Chariot const&) = 0;
  virtual void dessine(PenduleDouble const&) = 0;
  virtual void dessine(PenduleRessort const&) = 0;
};
