#include "glwidget.h"
#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "Systeme.h"
#include "Oscillateur.h"
#include "Vecteur.h"
#include <memory>


// ======================================================================
void VueOpenGL::dessine(Systeme const& sys){
  dessineAxes();
  dessineGrille();
  for(auto const& osc : sys.get_col()){
    osc->dessine();
  }
}

void VueOpenGL::phase(Oscillateur const& o, Integrateur const& integrat, double tFinal, double dt){
  PhaseWidget* ph(new PhaseWidget(o,integrat, tFinal, dt));
  ph->show();
  //TODO le delete du pointeur ?
}

void VueOpenGL::dessine(Pendule const& p){
  QMatrix4x4 matrice;
  Vecteur3D O(p.get_O());
  Vecteur3D pos(p.position());

  // Dessin de la corde
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
  glEnd();

  // dessin du pendule (masse)
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(p.get_anglePrecession(true),p.get_angleNutation(true),0,matrice);
  matrice.translate(0.0,0.0,-p.get_L());
  matrice.scale(0.15);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice, p.vitesse());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
  }

}

void VueOpenGL::dessine(Ressort const& r){
  QMatrix4x4 matrice;
  Vecteur3D O(r.get_O());
  Vecteur3D pos(r.position());

  // Dessin du ressort
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
  glEnd();

  // dessin de la masse
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(r.get_anglePrecession(true),r.get_angleNutation(true),0,matrice);
  matrice.translate(0,r.get_x(),0);
  matrice.scale(0.15);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice, r.vitesse());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
  }
}

void VueOpenGL::dessine(Torsion const& t){
  QMatrix4x4 matrice;
  Vecteur3D O(t.get_O());

  // Dessin de la corde
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z()+3);
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  glEnd();

  // dessin du pendule (masse)
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(t.get_anglePrecession(true),t.get_angleNutation(true),t.get_angleRotPro(true),matrice);
  matrice.scale(0.1);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice,t.vitesse());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
    matrice.translate(-1,0,0);
    dessineCube(matrice);
    matrice.translate(2,0,0);
    dessineCube(matrice);
  }
}

void VueOpenGL::dessine(Chariot const& c){
  QMatrix4x4 matrice;
  Vecteur3D O(c.get_O());
  Vecteur3D posC(c.posC());
  Vecteur3D posP(c.posP());

  // Dessin du ressort
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  prog.setAttributeValue(SommetId, posC.x(), posC.y(), posC.z());
  prog.setAttributeValue(SommetId, posC.x(), posC.y(), posC.z());
  prog.setAttributeValue(SommetId, posP.x(), posP.y(), posP.z());
  glEnd();

  // dessin de la masse
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(c.get_anglePrecession(true),0,0,matrice);
  matrice.translate(0,c.get_x(),0);
  QMatrix4x4 reference(matrice);
  matrice.scale(0.15);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice,c.vitC());
  }else{
    dessineCube(matrice);
  }
  matrice=reference;
  angleEuler(0,c.get_angleNutation(true),0,matrice);
  matrice.translate(0.0,0.0,-c.get_L());
  matrice.scale(0.15);
  if(vitesse){
    dessineSphere(matrice,c.vitP());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
  }
}

void VueOpenGL::dessine(PenduleDouble const& pd){
  QMatrix4x4 matrice;
  Vecteur3D O(pd.get_O());
  Vecteur3D pos1(pd.pos1());
  Vecteur3D pos2(pd.pos2());

  // Dessin de la corde
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  prog.setAttributeValue(SommetId, pos1.x(), pos1.y(), pos1.z());
  prog.setAttributeValue(SommetId, pos1.x(), pos1.y(), pos1.z());
  prog.setAttributeValue(SommetId, pos2.x(), pos2.y(), pos2.z());
  glEnd();

  // dessin du pendule (masse)
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(pd.get_anglePrecession(true),pd.get_angleNutation(true),0,matrice);
  matrice.translate(0.0,0.0,-pd.get_L1());
  QMatrix4x4 ref(matrice);
  matrice.scale(0.15);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice,pd.vit1());
  }else{
    dessineCube(matrice);
  }
  matrice=ref;
  angleEuler(0,pd.get_angleNutation2(true),0,matrice);
  matrice.translate(0.0,0.0,-pd.get_L2());
  matrice.scale(0.15);
  if(vitesse){
    dessineSphere(matrice,pd.vit2());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
  }
}

void VueOpenGL::dessine(PenduleRessort const& pr){
  QMatrix4x4 matrice;
  Vecteur3D O(pr.get_O());
  Vecteur3D pos(pr.position());

  // Dessin de la corde
  prog.setUniformValue("vue_modele", matrice_vue * matrice);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
  prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
  glEnd();

  // dessin du pendule (masse)
  matrice.translate(O.x(),O.y(),O.z());
  angleEuler(pr.get_anglePrecession(true),pr.get_angleNutation(true),0,matrice);
  dessineAxes(matrice, false); // pour mieux visualiser le plan dans lequel le pendule-ressort oscille
  matrice.translate(0.0,0.0,-pr.get_L());
  matrice.scale(0.15);
  if(vitesse){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
    dessineSphere(matrice,pr.vitesse());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
  }else{
    dessineCube(matrice);
  }
}

// ======================================================================
void VueOpenGL::init(){
  /* Initialise notre vue OpenGL.
   * Dans cet exemple, nous créons et activons notre shader.
   *
   * En raison du contenu des fichiers *.glsl, le shader de cet exemple
   * NE permet QUE de dessiner des primitives colorées
   * (pas de textures, brouillard, reflets de la lumière ou autres).
   *
   * Il est séparé en deux parties VERTEX et FRAGMENT.
   * Le VERTEX :
   * - récupère pour chaque sommet des primitives de couleur (dans
   *     l'attribut couleur) et de position (dans l'attribut sommet)
   * - multiplie l'attribut sommet par les matrices 'vue_modele' et
   *     'projection' et donne le résultat à OpenGL
   *   - passe la couleur au shader FRAGMENT.
   *
   * Le FRAGMENT :
   *   - applique la couleur qu'on lui donne
   */

  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");

  /* Identifie les deux attributs du shader de cet exemple
   * (voir vertex_shader.glsl).
   *
   * L'attribut identifié par 0 est particulier, il permet d'envoyer un
   * nouveau "point" à OpenGL
   *
   * C'est pourquoi il devra obligatoirement être spécifié et en dernier
   * (après la couleur dans cet exemple, voir plus bas).
   */

  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("couleur", CouleurId);

  // Compilation du shader OpenGL
  prog.link();

  // Activation du shader
  prog.bind();

  /* Activation du "Test de profondeur" et du "Back-face culling"
   * Le Test de profondeur permet de dessiner un objet à l'arrière-plan
   * partielement caché par d'autres objets.
   *
   * Le Back-face culling consiste à ne dessiner que les face avec ordre
   * de déclaration dans le sens trigonométrique.
   */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  sphere.initialize();  // initialise la sphère
  initializePosition();
}

// ======================================================================
void VueOpenGL::initializePosition(){
  // position initiale
  matrice_vue.setToIdentity();
  rotate(-38.66,0,0,1);
  rotate(-65,1,0,0);
  translate(0.0, 0.0, 3.0);
}

// ======================================================================
void VueOpenGL::translate(double x, double y, double z){
  /* Multiplie la matrice de vue par LA GAUCHE.
   * Cela fait en sorte que la dernière modification apportée
   * à la matrice soit appliquée en dernier (composition de fonctions).
   */
  QMatrix4x4 translation_supplementaire;
  translation_supplementaire.translate(x, y, z);
  matrice_vue = translation_supplementaire * matrice_vue;
}


// ======================================================================
void VueOpenGL::rotate(double angle, double dir_x, double dir_y, double dir_z){
  // Multiplie la matrice de vue par LA GAUCHE
  QMatrix4x4 rotation_supplementaire;
  rotation_supplementaire.rotate(angle, dir_x, dir_y, dir_z);
  matrice_vue = rotation_supplementaire * matrice_vue;
}

void VueOpenGL::changeDessin(){
  if(vitesse)vitesse=false;
  else vitesse=true;
}

// ======================================================================
void VueOpenGL::dessineCube (QMatrix4x4 const& point_de_vue){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_QUADS);
  // face coté X = +1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);

  // face coté X = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);

  // face coté Y = +1
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0); // bleu
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);

  // face coté Y = -1
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 1.0); // cyan
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);

  // face coté Z = +1
  prog.setAttributeValue(CouleurId, 1.0, 1.0, 0.0); // jaune
  prog.setAttributeValue(SommetId, -1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, +1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, +1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, +1.0);

  // face coté Z = -1
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 1.0); // magenta
  prog.setAttributeValue(SommetId, -1.0, -1.0, -1.0);
  prog.setAttributeValue(SommetId, -1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, +1.0, -1.0);
  prog.setAttributeValue(SommetId, +1.0, -1.0, -1.0);

  glEnd();
}

void VueOpenGL::dessineAxes (QMatrix4x4 const& point_de_vue, bool en_couleur){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);

  glBegin(GL_LINES);

  // axe X
  if (en_couleur) {
    prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.0); // rouge
  } else {
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
  }
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 1.0, 0.0, 0.0);

  // axe Y
  if (en_couleur) prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 1.0, 0.0);

  // axe Z
  if (en_couleur) prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0); // bleu
  prog.setAttributeValue(SommetId, 0.0, 0.0, 0.0);
  prog.setAttributeValue(SommetId, 0.0, 0.0, 1.0);

  glEnd();
}

void VueOpenGL::dessineGrille(double hauteur, double largeur, int n, QMatrix4x4 const& point_de_vue){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, 0.25, 0.25, 0.25);
  double h(largeur/n);
  for(int i(-n+1);i<=n-1;i++){
    prog.setAttributeValue(SommetId, i*h, -largeur, hauteur);
    prog.setAttributeValue(SommetId, i*h,  largeur, hauteur);
    prog.setAttributeValue(SommetId, -largeur, i*h, hauteur);
    prog.setAttributeValue(SommetId,  largeur, i*h, hauteur);
  }
  glEnd();
}

void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue,
                               double rouge, double vert, double bleu){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, rouge, vert, bleu);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}

//dessine une sphere et la colorie en fonction de sa vitesse : blanc = lent --> rouge foncé = rapide
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue, double vit){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, 10.0/(vit+4)+0.5, 10.0/(vit+4)-1.5,10.0/(vit+4)-0.5);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}

// fonction externe, modifie la matrice par une rotation selon les angles d'Euler
void angleEuler(double a, double b, double c, QMatrix4x4& matrice){
  matrice.rotate(a,0,0,1); // précession
  matrice.rotate(b,1,0,0); // nutation
  matrice.rotate(c,0,0,1); // rotation propre
}
