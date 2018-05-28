#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
  //try{
    IntegrateurRungeKutta rk;
    GLWidget w(rk);
    int nb(8);
    w.add(Pendule({1},{0},{1,0,0},{-4,4,-3}));
    for(int k(0);k<=nb;k++){
    for(int j(-nb);j<=0;j++){
    for(int i(0);i<=nb;i++){
      w.add(PenduleDouble( {3.14,3.14},    {0,0}, {3,1,0},  { 0.0+4.0*k, 1.8+5.0*j, 0.0+3.0*i}));
      w.add(PenduleDouble( {1.5,1.5},      {0,0}, {1,0,0},  { 0.0+4.0*k, 1.0+5.0*j, 0.0+3.0*i},1,1,10,2));
      w.add(PenduleRessort({1,-1},         {0,0}, {1,-1,0}, { 1.0+4.0*k,-1.0+5.0*j, 0.0+3.0*i},0.5,1,10));
      w.add(Chariot(       {1,0},          {0,0}, {1,-2,0}, {-1.5+4.0*k,-1.5+5.0*j, 0.0+3.0*i},10,1,0,1,10));
      w.add(Pendule(       {3.14},         {0},   {-1,0,0}, { 0.0+4.0*k, 0.5+5.0*j, 0.0+3.0*i},2,2));
      w.add(Pendule(       {M_PI-0.00001}, {0},   {1,0,0},  { 1.3+4.0*k, 0.0+5.0*j, 0.0+3.0*i},1,1,0.05));
      w.add(Ressort(       {-1},           {0},   {0,0,1},  { 0.0+4.0*k, 0.0+5.0*j, 1.0+3.0*i},2, 1));
      w.add(Chariot(       {0,0},          {4,0}, {1,-2,0}, {-1.0+4.0*k,-1.0+5.0*j, 0.0+3.0*i},100,10,0,1,1,0));
    }}}
    w.show();

    return a.exec();
  /*} catch(Erreur err){
    err.affiche("main_qt_gl.cc");
  }*/

}

// TODO, question : que mettre dans le bloc try ? tout ? ou moins ?
