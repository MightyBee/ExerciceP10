#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
//  try{



    IntegrateurEulerCromer euler;
    IntegrateurNewmark newmark;
    IntegrateurRungeKutta rk;

    //Pendule p1({0},{1},{-1,0,0},{0,1,0},2,2);
    //Pendule p2({M_PI-0.00001},{0},{1,1,0},{1,0,0},1,1,0.05);
    //Ressort r({1}, {0}, {-1,2,-1}, {0,0,0}, 2, 1);
    //Chariot c({0,0},{4,0},{1,-2,0},{-1,-1,0},100,10,0,1,1,0);
    GLWidget w(rk);
    //w.initializeSysteme();
    w.add(PenduleDouble({3.14,3.14}));
    w.add(PenduleDouble({3.14,3.14},{0,0},{1,0,0},{0,-1,0},1,1,10));
    //w.add(p1);
    //w.add(p2);
    //w.add(r);
    //w.add(c);
    w.show();
//  } catch(Erreur err){
//    err.affiche("main_qt_gl.cc");
//  }
  return a.exec();
}

// TODO, question : que mettre dans le bloc try ? tout ? ou moins ?
