#include <iostream>
#include <memory>
#include "Integrateur.h"
using namespace std;


/*##############################################################################
###                                                                          ###
###              METHODES DE LA CLASSE IntegrateurEulerCromer                ###
###                                                                          ###
##############################################################################*/


unique_ptr<IntegrateurEulerCromer> IntegrateurEulerCromer::clone() const{
  return unique_ptr<IntegrateurEulerCromer>(new IntegrateurEulerCromer(*this));
}


// spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration d'Euler-Cromer
void IntegrateurEulerCromer::evolue(Oscillateur& osc, double dt, double t){
  osc.set_Q(osc.get_Q()+dt*osc.f(t));
  osc.set_P(osc.get_P()+dt*osc.get_Q());
}


/*##############################################################################
###                                                                          ###
###                METHODES DE LA CLASSE IntegrateurNewmark                  ###
###                                                                          ###
##############################################################################*/



unique_ptr<IntegrateurNewmark> IntegrateurNewmark::clone() const{
  return unique_ptr<IntegrateurNewmark>(new IntegrateurNewmark(*this));
}


// spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration de Newmark
void IntegrateurNewmark::evolue(Oscillateur& osc, double dt, double t){
  Vecteur P(osc.get_P());
  Vecteur Q(osc.get_Q());
  Vecteur s(osc.f(t)), q(0), r(0);
  double e(pow(10,-8));
  do{
    q=osc.get_P();
    r=osc.f(t+dt);
    osc.set_Q(Q+0.5*dt*(r+s));
    osc.set_P(P+dt*Q+dt*dt/3.0*(0.5*r+s));
  }while((osc.get_P()-q).norme()>=e);
}


/*##############################################################################
###                                                                          ###
###               METHODES DE LA CLASSE IntegrateurRungeKutta                ###
###                                                                          ###
##############################################################################*/

unique_ptr<IntegrateurRungeKutta> IntegrateurRungeKutta::clone() const{
  return unique_ptr<IntegrateurRungeKutta>(new IntegrateurRungeKutta(*this));
}


void IntegrateurRungeKutta::evolue(Oscillateur& osc, double dt, double t){
  Vecteur P(osc.get_P());
  Vecteur Q(osc.get_Q());
  Vecteur k1(Q);
  Vecteur kk1(osc.f(t));

  osc.set_P(P+0.5*dt*k1);
  osc.set_Q(Q+0.5*dt*kk1);
  Vecteur k2(Q+0.5*dt*kk1);
  Vecteur kk2(osc.f(t+0.5*dt));

  osc.set_P(P+0.5*dt*k2);
  osc.set_Q(Q+0.5*dt*kk2);
  Vecteur k3(Q+0.5*dt*kk2);
  Vecteur kk3(osc.f(t+0.5*dt));

  osc.set_P(P+dt*k3);
  osc.set_Q(Q+dt*kk3);
  Vecteur k4(Q+dt*kk3);
  Vecteur kk4(osc.f(t+dt));

  osc.set_P(P+dt/6.0*(k1+2*k2+2*k3+k4));
  osc.set_Q(Q+dt/6.0*(kk1+2*kk2+2*kk3+kk4));
}
