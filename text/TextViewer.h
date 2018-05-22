#pragma once
#include "SupportADessin.h"
#include "Oscillateur.h"
#include "Systeme.h"

class TextViewer : public SupportADessin {
  public:
    TextViewer(std::ostream& sortie);
    virtual void dessine(Systeme const&) override;
    virtual void dessine(Pendule const&) override;
    virtual void dessine(Ressort const&) override;
    virtual void dessine(Torsion const&) override;
    virtual void dessine(Chariot const&) override;
    virtual void dessine(PenduleDouble const&) override;
    virtual void dessine(PenduleRessort const&) override;

    virtual void phase(Oscillateur const&, Integrateur const&, double tFinal=10, double dt=0.01 ) override;

  private:
    std::ostream& sortie;
};
