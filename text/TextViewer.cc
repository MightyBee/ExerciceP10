#include <iostream>
#include <string>
#include "TextViewer.h"
using namespace std;

TextViewer::TextViewer(ostream& sortie) : sortie(sortie) {}

void TextViewer::dessine(Pendule const& p){
  sortie << p << endl;
}

void TextViewer::dessine(Ressort const& r){
  sortie << r << endl;
}

void TextViewer::dessine(Systeme const& sys){
  sortie << sys << endl;

}
