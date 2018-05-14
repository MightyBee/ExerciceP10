#include <iostream>
#include <string>
#include "TextViewer.h"
using namespace std;

TextViewer::TextViewer(ostream& sortie) : sortie(sortie) {}

void TextViewer::dessine(Systeme const& sys){
  sortie << sys << endl;
}

void TextViewer::dessine(Pendule const& p){
  sortie << p << endl;
}

void TextViewer::dessine(Ressort const& r){
  sortie << r << endl;
}

void TextViewer::dessine(Torsion const& t){
  sortie << t << endl;
}

void TextViewer::dessine(Chariot const& c){
  sortie << c << endl;
}

void TextViewer::dessine(PenduleDouble const& pd){
  sortie << pd << endl;
}

void TextViewer::dessine(PenduleRessort const& pr){
  sortie << pr << endl;
}

void TextViewer::phase(Oscillateur const& o, Integrateur const& integrat, double tFinal, double dt){
  unique_ptr<Oscillateur> osc(o.copie());
  sortie << "##### ESPACE DES PHASES ####" << endl;
  for(double t(0.0); t<=tFinal;t+=dt){
    sortie << osc->PQ() << endl;
    integrat.evolue(*osc,dt,t);
  }
  sortie << endl;
}
