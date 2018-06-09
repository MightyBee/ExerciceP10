#include <QApplication>
#include "glwidget.h"
#include "Integrateur.h"

int main(int argc, char* argv[]){
  QApplication a(argc, argv);
  //try{
    IntegrateurRungeKutta rk;
    GLWidget w(rk);
    double m(1), L(1);
    double critique(sqrt(L*L*L*m*9.81*4));

    int nb(0); // on construit un cube de "côté" nb : il y a nb^3 fois le même système, juste décalé dans l'espace
    // note : sur nos ordis la limite se situe plus ou moins a 8 pour que le simulation reste a peu pres fluide
    //w.add(Pendule({1},{0},{1,0,0},{-4,4,-3}));
    for(int k(0)  ;k<=nb;k++){
    for(int j(-nb);j<=0 ;j++){
    for(int i(0)  ;i<=nb;i++){
      w.add(PenduleDouble( {3.14,3.14},    {0,0}, {3,1,0},  { 0.0+4.0*k, 1.8+5.0*j, 0.0+3.0*i}));
      w.add(PenduleDouble( {1.5,1.5},      {0,0}, {1,0,0},  { 0.0+4.0*k, 1.0+5.0*j, 0.0+3.0*i},1,1,10,2));
      w.add(PenduleRessort({1,-1},         {0,0}, {1,-1,0}, { 1.0+4.0*k,-1.0+5.0*j, 0.0+3.0*i},0.5,1,10));
      w.add(Chariot(       {1,0},          {0,0}, {1,-2,0}, {-1.5+4.0*k,-1.5+5.0*j, 0.0+3.0*i},10,1,0,1,10));
      w.add(Pendule(       {3.14},         {0},   {-1,0,0}, { 0.0+4.0*k, 0.5+5.0*j, 0.0+3.0*i},2,2));
      w.add(Pendule(       {M_PI-1.00001}, {0},   {1,0,0},  { 0.0, 0.0-1.0*i, 0.0},L,m,(0.3*i+0.1)*critique,true));
      w.add(Ressort(       {-1},           {0},   {0,0,1},  { 0.0+4.0*k, 0.0+5.0*j, 0.0+3.0*i},2, 1));
      w.add(Torsion(       {1},            {5.2}, {1,0,0},  { 0.0+4.0*k, 0.0+5.0*j, 0.0+3.0*i},0.0,1.0,1.0));
      w.add(Chariot(       {0,0},          {4,0}, {1,-2,0}, {-1.0+4.0*k,-1.0+5.0*j, 0.0+3.0*i},100,10,0,1,1,0));
      w.add(PenduleDouble({3.14+0.000000001*i,3.14},{0,0},{1,0,0},{0,i,0}));
    }}}
    //w.add(RessortForce({-1},{0},{1,0,0},{5.0,5.0,-3.0},2,1,0,4,3));


    w.show();

    return a.exec();
  /*} catch(Erreur err){
    err.affiche("main_qt_gl.cc");
  }*/

}

// TODO, question : que mettre dans le bloc try ? tout ? ou moins ?
