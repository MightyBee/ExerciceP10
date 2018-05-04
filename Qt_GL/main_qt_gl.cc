#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
//  try{
    IntegrateurEulerCromer euler;
    Pendule p1({0},{1},{-1,-1,0},{0,1,0},2,2);
    Pendule p2({0},{1},{1,1,0},{1,0,0},1,0.5);
    Ressort r({1}, {0}, {1,1,-5}, {0,0,0}, 5, 1);
    GLWidget w(euler);
    w.add(p1);
    w.add(p2);
    w.add(r);
    w.show();
//  } catch(Erreur err){
//    err.affiche("main_qt_gl.cc");
//  }
  return a.exec();
}

// TODO, question : que mettre dans le bloc try ? tout ? ou moins ?
