#pragma once
#include "SupportADessin.h"
#include "Oscillateur.h"
#include "Systeme.h"

class TextViewer : public SupportADessin {
  public:
    TextViewer(std::ostream& sortie);
    virtual void dessine(Pendule const&) override;
    virtual void dessine(Ressort const&) override;
    virtual void dessine(Systeme const&) override;
  private:
    std::ostream& sortie;
};
