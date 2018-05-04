#include <iostream>
#include <memory>
#include "Integrateur.h"
using namespace std;

/*##############################################################################
###                                                                          ###
###                    METHODES DE LA CLASSE Integrateur                     ###
###                                                                          ###
##############################################################################*/




/*##############################################################################
###                                                                          ###
###                           OPÉRATEURS EXTERNES                            ###
###                                                                          ###
##############################################################################*/




/*##############################################################################
###                                                                          ###
###              METHODES DE LA CLASSE IntegrateurEulerCromer                ###
###                                                                          ###
##############################################################################*/


unique_ptr<IntegrateurEulerCromer> IntegrateurEulerCromer::clone() const{
  return unique_ptr<IntegrateurEulerCromer>(new IntegrateurEulerCromer(*this));
}

unique_ptr<Integrateur> IntegrateurEulerCromer::copie() const{
  return clone();
}

// spécialisation de la méthode "evolue()" de la super-classe, avance d'un pas de temps avec la méthode d'intégration d'Euler-Cromer
void IntegrateurEulerCromer::evolue(Oscillateur& osc, double dt){
  osc.set_Q(osc.get_Q()+dt*osc.f());
  osc.set_P(osc.get_P()+dt*osc.get_Q());
}



/*##############################################################################
###                                                                          ###
###               METHODES DE LA CLASSE IntegrateurRungeKutta                ###
###                                                                          ###
##############################################################################*/

/*
void IntergrateurRungeKutta::evolue(){
  Vecteur P(osc.get_P());
  Vecteur Q(osc.get_Q());
  Vecteur k1(Q);
  Vecteur kk1(f(t_abs,P,Q));
  Vecteur k2(Q+pdt/2*kk1);
  Vecteur kk2(f(t_abs+pdt/2,P+pdt/2*k1,Q+pdt/2*kk1));
  Vecteur k3(Q+pdt/2*kk2);
  Vecteur kk3(f(t_abs+pdt/2,P+pdt/2*k2,Q+pdt/2*kk2));
  Vecteur k4(Q+pdt*kk3);
  Vecteur kk4(f(t_abs+pdt,P+pdt*k3,Q+pdt*kk3));
  osc.set_P(P+pdt/6*(k1+2*k2+2*k3+k4));
  osc.set_Q(Q+pdt/6*(kk1+2*kk2+2*kk3+kk4));
  t_abs+=pdt;
} */
