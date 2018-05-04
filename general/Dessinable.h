#pragma once
#include "SupportADessin.h"

class Dessinable {
  public:
    Dessinable(SupportADessin* sup);
    virtual ~Dessinable() {}
    virtual void dessine() const = 0;
    void set_sup(SupportADessin* sup);
  protected:
    SupportADessin* support;
};
