#pragma once

class Pendule;
class Ressort;
class Chariot;
class Systeme;

class SupportADessin
{
 public:

  virtual ~SupportADessin() {}

  virtual void dessine(Pendule const&) = 0;
  virtual void dessine(Ressort const&) = 0;
  virtual void dessine(Systeme const&) = 0;
  virtual void dessine(Chariot const&) = 0;

};
