#ifndef VUEOPENGL_H
#define VUEOPENGL_H

#include <QOpenGLShaderProgram> // Classe qui regroupe les fonctions OpenGL liées aux shaders
#include <QMatrix4x4>
#include "glsphere.h"
#include "SupportADessin.h"
#include "Systeme.h"
#include "Oscillateur.h"
#include "Vecteur.h"


class VueOpenGL : public SupportADessin {
 public:
  // méthode(s) de dessin (héritée(s) de SupportADessin)
  virtual void dessine(Systeme const&) override;
  virtual void dessine(Pendule const&) override;
  virtual void dessine(Ressort const&) override;
  virtual void dessine(Torsion const&) override;
  virtual void dessine(Chariot const&) override;
  virtual void dessine(PenduleDouble const&) override;
  virtual void dessine(PenduleRessort const&) override;

  virtual void phase(Oscillateur const&, Integrateur const&, double tFinal=10, double dt=0.01 ) override;


  // méthodes de (ré-)initialisation
  void init();
  void initializePosition();

  // méthode set
  void setProjection(QMatrix4x4 const& projection)
  { prog.setUniformValue("projection", projection); }

  // Méthodes set
  void translate(double x, double y, double z);
  void rotate(double angle, double dir_x, double dir_y, double dir_z);
  void changeDessin(); // change le bool vitesse

  // méthode utilitaire offerte pour simplifier
  void dessineAxes(QMatrix4x4 const& point_de_vue = QMatrix4x4(), bool en_couleur = true);
  void dessineGrille(double hauteur = -3, double largeur = 5, int n=5, QMatrix4x4 const& point_de_vue = QMatrix4x4());
  void dessineCube(QMatrix4x4 const& point_de_vue = QMatrix4x4() );
  void dessineSphere(QMatrix4x4 const& point_de_vue, double rouge, double vert, double bleu);
  void dessineSphere(QMatrix4x4 const& point_de_vue, double vit=0);

 private:
  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;
  GLSphere sphere;

  // Caméra
  QMatrix4x4 matrice_vue;

  // Quel type de dessin : cube ou sphere avec couleur en fct de la vitesse
  bool vitesse=false;
};

void angleEuler(double a, double b, double c, QMatrix4x4& matrice);

#endif
