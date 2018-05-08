#pragma once

class Systeme;
class Pendule;
class Ressort;
class Torsion;
class Chariot;
class PenduleDouble;

class SupportADessin
{
 public:

  virtual ~SupportADessin() {}

  virtual void dessine(Systeme const&) = 0;
  virtual void dessine(Pendule const&) = 0;
  virtual void dessine(Ressort const&) = 0;
  virtual void dessine(Torsion const&) = 0;
  virtual void dessine(Chariot const&) = 0;
  virtual void dessine(PenduleDouble const&) = 0;

};
