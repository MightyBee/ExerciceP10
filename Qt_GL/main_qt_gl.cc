#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
  //try{
    IntegrateurEulerCromer euler;
    IntegrateurNewmark newmark;
    IntegrateurRungeKutta rk;

    GLWidget w(rk);
    //w.initializeSysteme();

    w.add(PenduleDouble({3.14,3.14},{0,0},{3,1,0},{0,1.8,0}));
    w.add(PenduleDouble({1.5,1.5},{0,0},{1,0,0},{0,1,0},1,1,10,2));
    w.add(PenduleRessort({1,-1},{0,0},{1,-1,0},{1,-1,0},0.5,1,10));
    w.add(Chariot({1,0},{0,0},{1,-2,0},{-1.5,-1.5,0},10,1,0,1,1));
    w.add(Pendule({3.14},{0},{-1,0,0},{0,0.5,0},2,2));
    w.add(Pendule({M_PI-0.00001},{0},{1,0,0},{1.3,0,0},1,1,0.05));
    w.add(Ressort({-1}, {0}, {0,0,1}, {0,0,1}, 2, 1));
    w.add(Chariot({0,0},{4,0},{1,-2,0},{-1,-1,0},100,10,0,1,1,0));

    w.show();
    return a.exec();

  /*} catch(Erreur err){
    err.affiche("main_qt_gl.cc");
  }*/

}

// TODO, question : que mettre dans le bloc try ? tout ? ou moins ?
