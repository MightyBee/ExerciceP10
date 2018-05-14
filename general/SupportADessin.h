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

  virtual ~SupportADessin() {}

  virtual void dessine(Systeme const&) = 0;
  virtual void dessine(Pendule const&) = 0;
  virtual void dessine(Ressort const&) = 0;
  virtual void dessine(Torsion const&) = 0;
  virtual void dessine(Chariot const&) = 0;
  virtual void dessine(PenduleDouble const&) = 0;
  virtual void dessine(PenduleRessort const&) = 0;

  virtual void phase(Oscillateur const&, Integrateur const&,  double tFinal=10, double dt=0.01 ) = 0;
};
