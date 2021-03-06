#include "glwidget.h"
#include "vue_opengl.h"
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "Systeme.h"
#include "Oscillateur.h"
#include "Vecteur.h"
#include <memory>


// ======================================================================
// Dessin du système
void VueOpenGL::dessine(Systeme const& sys){
  dessineAxes();
  if(fpvOsc>=0 and fpvOsc < sys.taille()){
    dessinePiece();
    fpvCam=true;
    sys.get_col()[fpvOsc]->dessine();
    fpvCam=false;
    for(auto const& osc : sys.get_col()){
      osc->dessine();
    }
  }else{
    dessineGrille();
    for(auto const& osc : sys.get_col()){
      osc->dessine();
    }
  }
}


// ======================================================================
// Dessin d'un pendule
void VueOpenGL::dessine(Pendule const& p){
  QMatrix4x4 matrice;
  Vecteur3D O(p.get_O()); //origine
  Vecteur3D pos(p.position()); //position

  if(not fpvCam){
    // Dessin de la corde
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
    prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
    prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
    glEnd();

    // Dessin du pendule (masse)
    matrice.translate(O.x(),O.y(),O.z());
    angleEuler(p.get_anglePrecession(true),p.get_angleNutation(true),0,matrice); // true car on veut en degrés et pas en radians
    matrice.translate(0.0,0.0,-p.get_L());
    matrice.scale(0.15);
    if(vitesse){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
      dessineSphere(matrice, p.vitesse()); // couleur en fonction de la vitesse
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
    }else{
      dessineCube(matrice);
    }
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.translate(0.0,0.0,p.get_L());
    matrice.rotate(-p.get_angleNutation(true),1,0,0);
    matrice.rotate(-p.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
  }
}


// ======================================================================
// Dessin d'un ressort
void VueOpenGL::dessine(Ressort const& r){
  QMatrix4x4 matrice;
  Vecteur3D O(r.get_O());
  Vecteur3D pos(r.position());

  if( not fpvCam){
    // Dessin du "ressort" (trait de l'origine à la position)
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
    prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
    prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
    glEnd();

    // Dessin de la masse
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
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.translate(0.0,-r.get_x(),0.0);
    matrice.rotate(-r.get_angleNutation(true),1,0,0);
    matrice.rotate(-r.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
  }
}


// ======================================================================
// Dessin d'un pendule de torsion
void VueOpenGL::dessine(Torsion const& t){
  QMatrix4x4 matrice;
  Vecteur3D O(t.get_O());

  if(not fpvCam){
    // Dessin de la masse
    matrice.translate(O.x(),O.y(),O.z());
    angleEuler(t.get_anglePrecession(true),t.get_angleNutation(true),t.get_angleRotPro(true),matrice);
    matrice.scale(0.1);
    if(vitesse){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
      dessineSphere(matrice,t.vitesse());
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
    }else{
      dessineCube(matrice); // on met trois cubes pour former une "tige" pour mieux voir la rotation
      matrice.translate(-1,0,0);
      dessineCube(matrice);
      matrice.translate(2,0,0);
      dessineCube(matrice);
    }
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.rotate(-t.get_angleRotPro(true),0,0,1);
    matrice.rotate(-t.get_angleNutation(true),1,0,0);
    matrice.rotate(-t.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
  }
}


// ======================================================================
// Dessin d'un chariot (pendule accroché à un chariot relié à un ressort)
void VueOpenGL::dessine(Chariot const& c){
  QMatrix4x4 matrice;
  Vecteur3D O(c.get_O());
  Vecteur3D posC(c.posC()); // position du chariot
  Vecteur3D posP(c.posP()); // position du pendule

  if(not fpvCam){
    // Dessin des cordes
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    glBegin(GL_LINE_STRIP);
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
    // de l'origine au pendule en passant par le chariot
    prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
    prog.setAttributeValue(SommetId, posC.x(), posC.y(), posC.z());
    prog.setAttributeValue(SommetId, posP.x(), posP.y(), posP.z());
    glEnd();

    // dessin de la masse
    matrice.translate(O.x(),O.y(),O.z());
    angleEuler(c.get_anglePrecession(true),0,0,matrice);
    matrice.translate(0,c.get_x(),0); // on est à la position du chariot maintenant
    QMatrix4x4 reference(matrice);
    matrice.scale(0.15);
    if(vitesse){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
      dessineSphere(matrice,c.vitC());
    }else{
      dessineCube(matrice);
    }
    matrice=reference; // on revient à juste avant le dessin du chariot
    angleEuler(0,c.get_angleNutation(true),0,matrice);
    matrice.translate(0.0,0.0,-c.get_L());
    matrice.scale(0.15);
    if(vitesse){
      dessineSphere(matrice,c.vitP());
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
    }else{
      dessineCube(matrice);
    }
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.translate(0.0,-c.get_L(),0.0);
    matrice.rotate(-c.get_angleNutation(true),1,0,0);
    matrice.translate(0.0,-c.get_x(),0.0);
    matrice.rotate(-c.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
  }
}


// ======================================================================
// Dessin d'un pendule double (pendule accroché à un autre pendule)
void VueOpenGL::dessine(PenduleDouble const& pd){
  QMatrix4x4 matrice;
  Vecteur3D O(pd.get_O());
  Vecteur3D pos1(pd.pos1()); // position du 1er pendule
  Vecteur3D pos2(pd.pos2()); // position du 2eme pendule

  if(not fpvCam){
    // Dessin de la corde
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    glBegin(GL_LINE_STRIP);
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
    prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
    prog.setAttributeValue(SommetId, pos1.x(), pos1.y(), pos1.z());
    prog.setAttributeValue(SommetId, pos2.x(), pos2.y(), pos2.z());
    glEnd();

    // dessin des deux masses
    matrice.translate(O.x(),O.y(),O.z());
    angleEuler(pd.get_anglePrecession(true),pd.get_angleNutation(true),0,matrice);
    matrice.translate(0.0,0.0,-pd.get_L1()); // on est a la position de la 1ère masse
    QMatrix4x4 ref(matrice);
    matrice.scale(0.15);
    if(vitesse){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // passe en mode "fil de fer"
      dessineSphere(matrice,pd.vit1());
    }else{
      dessineCube(matrice);
    }
    matrice=ref; // on revient à la position de la 1ère masse juste avant le dessin
    angleEuler(0,pd.get_angleNutation2(true),0,matrice);
    matrice.translate(0.0,0.0,-pd.get_L2());
    matrice.scale(0.15);
    if(vitesse){
      dessineSphere(matrice,pd.vit2());
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // repasse en mode "plein"
    }else{
      dessineCube(matrice);
    }
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.translate(0.0,0.0,pd.get_L2());
    matrice.rotate(-pd.get_angleNutation2(true),1,0,0);
    matrice.translate(0.0,0.0,pd.get_L1());
    matrice.rotate(-pd.get_angleNutation(true),1,0,0);
    matrice.rotate(-pd.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
  }
}


// ======================================================================
// Dessin d'un pendule-ressort (masse accrochée à un ressort qui pendule)
void VueOpenGL::dessine(PenduleRessort const& pr){
  QMatrix4x4 matrice;
  Vecteur3D O(pr.get_O());
  Vecteur3D pos(pr.position());

  if(not fpvCam){
    // Dessin de la corde
    prog.setUniformValue("vue_modele", matrice_vue * matrice);
    glBegin(GL_LINES);
    prog.setAttributeValue(CouleurId, 1.0, 1.0, 1.0); // blanc
    prog.setAttributeValue(SommetId, O.x(), O.y(), O.z());
    prog.setAttributeValue(SommetId, pos.x(), pos.y(), pos.z());
    glEnd();

    // Dessin de la masse
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
  }else{
    matrice.setToIdentity();
    matrice.rotate(-90,1,0,0);
    matrice.translate(0.0,0.0,pr.get_L());
    matrice.rotate(-pr.get_angleNutation(true),1,0,0);
    matrice.rotate(-pr.get_anglePrecession(true),0,0,1);
    matrice.translate(-O.x(),-O.y(),-O.z());
    matrice_vue=matrice;
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
  rotate(-60,1,0,0);
  translate(0.0, 0.0, -4.0);
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

// ======================================================================
void VueOpenGL::changeDessin(){
  if(vitesse) vitesse=false;
  else vitesse=true;
}

// ======================================================================
void VueOpenGL::changeVue(int taille){
  if(fpvOsc<taille-1){
    fpvOsc++;
  }else{
    fpvOsc=-1;
    initializePosition();
  }
}

// ======================================================================
void VueOpenGL::dessineQueue(const QueueOscillateur& queue, size_t i){
  if(fpvOsc!=i){
    prog.setUniformValue("vue_modele", matrice_vue);
    glBegin(GL_LINE_STRIP);
    prog.setAttributeValue(CouleurId, 0.8, 0.3, 0.0); // rouge
    for(const auto& point : queue.get_list()){
      prog.setAttributeValue(SommetId,point.x(),point.y(),point.z());
    }
    glEnd();
  }
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

// ======================================================================
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

// ======================================================================
void VueOpenGL::dessineGrille(double hauteur, double largeur, int n, double rouge, double vert, double bleu, QMatrix4x4 const& point_de_vue){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  glBegin(GL_LINES);
  prog.setAttributeValue(CouleurId, rouge, vert, bleu); // gris clair
  double h(largeur/n);
  for(int i(-n+1);i<=n-1;i++){
    prog.setAttributeValue(SommetId, i*h, -largeur, hauteur);
    prog.setAttributeValue(SommetId, i*h,  largeur, hauteur);
    prog.setAttributeValue(SommetId, -largeur, i*h, hauteur);
    prog.setAttributeValue(SommetId,  largeur, i*h, hauteur);
  }
  glEnd();
}

// ======================================================================
void VueOpenGL::dessinePiece(){
  QMatrix4x4 matrice;
  //dessineGrille(5,5,5,0,0,1,matrice);
  dessineGrille(-5,5,5,0,1,1,matrice);
  matrice.rotate(90,1,0,0);
  dessineGrille(-5,5,5,0,1,0,matrice);
  dessineGrille(5,5,5,1,1,0,matrice);
  matrice.setToIdentity();
  matrice.rotate(90,0,1,0);
  dessineGrille(-5,5,5,1,0,1,matrice);
  dessineGrille(5,5,5,1,0,0,matrice);
}

// ======================================================================
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue,
                               double rouge, double vert, double bleu){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, rouge, vert, bleu);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}

// ======================================================================
//dessine une sphere et la colorie en fonction de sa vitesse : blanc = lent --> rouge foncé = rapide
void VueOpenGL::dessineSphere (QMatrix4x4 const& point_de_vue, double vit){
  prog.setUniformValue("vue_modele", matrice_vue * point_de_vue);
  prog.setAttributeValue(CouleurId, 10.0/(vit+4)+0.5, 10.0/(vit+4)-1.5,10.0/(vit+4)-0.5);  // met la couleur
  sphere.draw(prog, SommetId);                           // dessine la sphère
}





// ======================================================================
// fonction externe, modifie la matrice par une rotation selon les angles d'Euler
void angleEuler(double a, double b, double c, QMatrix4x4& matrice){
  matrice.rotate(a,0,0,1); // précession
  matrice.rotate(b,1,0,0); // nutation
  matrice.rotate(c,0,0,1); // rotation propre
}
