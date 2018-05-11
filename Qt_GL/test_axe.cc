#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
  IntegrateurRungeKutta rk;
  GLWidget w(rk);

  Vecteur3D axe(1,0,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(1,1,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(0,1,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(-1,2,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(-1,0,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(-3,-1,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(0,-1,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  axe=Vecteur3D(1,-1,0);
  w.add(Pendule({0},{1},axe,Vecteur3D(axe.x(),axe.y(),3),1,1));
  w.add(Ressort({1}, {0}, axe, Vecteur3D(axe.x(),axe.y(),0), 2, 1));
  w.add(Chariot({0,0},{4,0},axe,Vecteur3D(axe.x(),axe.y(),-3),100,10,0,1,1,0));

  w.show();
  return a.exec();
}
