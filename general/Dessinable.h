#pragma once
#include "SupportADessin.h"

class Dessinable {
  public:
    //constructeur
    Dessinable(SupportADessin* sup);
    //destructeur
    virtual ~Dessinable() {}
    //autres méthodes
    virtual void dessine() const = 0;
    void set_sup(SupportADessin* sup);
  protected:
    //attribut
    SupportADessin* support;
};
