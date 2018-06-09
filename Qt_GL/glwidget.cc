#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include <cmath>
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "glwidget.h"


/*##############################################################################
###                                                                          ###
###                      METHODES DE LA CLASSE PWidget                       ###
###                                                                          ###
##############################################################################*/


// ======================================================================
void PWidget::initializeGL(){
  prog.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/vertex_shader.glsl");
  prog.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment_shader.glsl");
  prog.bindAttributeLocation("sommet",  SommetId);
  prog.bindAttributeLocation("couleur", CouleurId);
  prog.link();
  prog.bind();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
}

// ======================================================================
void PWidget::resizeGL(int width, int height){
  glViewport(10, 10, width-20, height-20); // on laisse une marge de 10px de chaque coté
}

// ======================================================================
void PWidget::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 matrice;
  prog.setUniformValue("vue_modele", matrice); // On met la matrice identité dans vue_modele

  double xm(minX), xM(maxX), ym(minY), yM(maxY); //   * pour ne pas déformer le dessin, on
  if((maxX-minX)/(maxY-minY)<ratio()){ //             * ajoute une correction au max et min
    xM+=0.5*((maxY-minY)*ratio()-(maxX-minX)); //     * des y ou des x selon le besoin
    xm-=0.5*((maxY-minY)*ratio()-(maxX-minX));
  } else if ((maxX-minX)/(maxY-minY)>ratio()) {
    yM+=0.5*((maxX-minX)/ratio()-(maxY-minY));
    ym-=0.5*((maxX-minX)/ratio()-(maxY-minY));
  }

  /* Change de matrice de projection adpatée aux zoom du graph */
  matrice.setToIdentity();
  matrice.ortho(xm, xM, ym, yM, -10.0, 10.0);
  prog.setUniformValue("projection", matrice);

  /* Dessine les axes */
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0);
  glBegin(GL_LINES); // la primitive LINES dessine une ligne par paire de points (n/2 lignes)
  prog.setAttributeValue(SommetId, xm, 0.0, -1.0); // le -1.0 dans la composante z met les axes en arrière plan
  prog.setAttributeValue(SommetId, xM, 0.0, -1.0);
  // dessine la graduation
  for(int i(ceil(xm));i<xM;++i){
    prog.setAttributeValue(SommetId,   i,-0.1, -1.0);
    prog.setAttributeValue(SommetId,   i, 0.1, -1.0);
  }
  for(int i(ceil(ym));i<yM;++i){
    prog.setAttributeValue(SommetId,-0.1,   i, -1.0);
    prog.setAttributeValue(SommetId, 0.1,   i, -1.0);
  }
  prog.setAttributeValue(SommetId, 0.0, ym, -1.0);
  prog.setAttributeValue(SommetId, 0.0, yM, -1.0);
  glEnd();

  /* Dessine les phases */
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0); // vert
  glBegin(GL_LINE_STRIP); // la primitive LINE_STRIP ne referme par le tracé (n-1 lignes)
  for(auto const& point : phases){ // on parcourt la tableau contenant tous les points
    prog.setAttributeValue(SommetId, point[0], point[1], 0.0);
  }
  glEnd();

  /* Dessine le dernier point / le point courant */
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.1);
  glPointSize(5);
  glBegin(GL_POINTS);
  prog.setAttributeValue(SommetId, phases.back()[0], phases.back()[1], 0.0);
  glEnd();
}

// ======================================================================
// permet d'ajouter un point dans l'espace des phases
void PWidget::add(std::array<double,3> const& pqt){
  if(phases.empty()){ // si le tableau des phases est vide, on initialise les min et max
    maxX=pqt[0]+0.001;
    minX=pqt[0]-0.001;
    maxY=pqt[1]+0.001;
    minY=pqt[1]-0.001;
  } else { // sinon, on met a jour les min et max
    if(phases.size()>=1000000) phases.erase(phases.begin()); // on met une limite a la taille du tableau
    if(pqt[0]>maxX) maxX=pqt[0];
    if(pqt[0]<minX) minX=pqt[0];
    if(pqt[1]>maxY) maxY=pqt[1];
    if(pqt[1]<minY) minY=pqt[1];
  }
  phases.push_back(pqt); // et on ajoute le point au tableau des phases
  updateGL();
}





/*##############################################################################
###                                                                          ###
###                      METHODES DE LA CLASSE GLWidget                      ###
###                                                                          ###
##############################################################################*/


// ======================================================================
// permet d'ajouter un oscillateur au système
void GLWidget::add(Oscillateur const& osc){
  s.add(osc);
  phases.push_back(nullptr); // on oublie pas d'ajouter une "case" à la collection gérant les fenêtres des phases
}

// ======================================================================
void GLWidget::initializeGL(){
  vue.init();
  timerId = startTimer(20);
}

// ======================================================================
void GLWidget::resizeGL(int width, int height){
  /* On commance par dire sur quelle partie de la
   * fenêtre OpenGL doit dessiner.
   * Ici on lui demande de dessiner sur toute la fenêtre.
   */
  glViewport(0, 0, width, height);

  /* Puis on modifie la matrice de projection du shader.
   * Pour ce faire on crée une matrice identité (constructeur
   * par défaut), on la multiplie par la droite par une matrice
   * de perspective.
   * Plus de détail sur cette matrice
   *     http://www.songho.ca/opengl/gl_projectionmatrix.html
   * Puis on upload la matrice sur le shader à l'aide de la
   * méthode de la classe VueOpenGL
   */
  QMatrix4x4 matrice;
  matrice.perspective(70.0, qreal(width) / qreal(height ? height : 1.0), 1e-3, 1e5);
  vue.setProjection(matrice);
}

// ======================================================================
void GLWidget::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  s.dessine();
}

// ======================================================================
// gère l'ouverture de la fenêtre principale (simulation)
void GLWidget::showEvent(QShowEvent* event){
  QGLWidget::showEvent(event);
  for(size_t i(0);i<phases.size();i++){ // on vérifie si on doit afficher le portrait des phases de certains oscillateurs
    if(s.getPhaseBool(i)) phase(i);
  }
}

// ======================================================================
// gère la femeture de la fenêtre principale (simulation)
void GLWidget::closeEvent (QCloseEvent* event){
  QGLWidget::closeEvent(event);
  for(size_t i(0);i<phases.size();++i) phase(i, false, true); // on ferme toutes les fenêtre des portrait des phases
}

// ======================================================================
// gère les événements clavier
void GLWidget::keyPressEvent(QKeyEvent* event){
  constexpr double petit_angle(1.0); // en degrés
  constexpr double petit_pas(0.1);

  switch (event->key()) {

  // déplacement
  case Qt::Key_Left:
    vue.rotate(petit_angle, 0.0, -1.0, 0.0);
    break;
  case Qt::Key_Right:
    vue.rotate(petit_angle, 0.0, +1.0, 0.0);
    break;
  case Qt::Key_Up:
    vue.rotate(petit_angle, -1.0, 0.0, 0.0);
    break;
  case Qt::Key_Down:
    vue.rotate(petit_angle, +1.0, 0.0, 0.0);
    break;
  case Qt::Key_PageUp:
  case Qt::Key_W:
    vue.translate(0.0, 0.0,  petit_pas);
    break;
  case Qt::Key_PageDown:
  case Qt::Key_S:
    vue.translate(0.0, 0.0, -petit_pas);
    break;
  case Qt::Key_A:
    vue.translate( petit_pas, 0.0, 0.0);
    break;
  case Qt::Key_D:
    vue.translate(-petit_pas, 0.0, 0.0);
    break;
  case Qt::Key_R:
    vue.translate(0.0, -petit_pas, 0.0);
    break;
  case Qt::Key_F:
    vue.translate(0.0,  petit_pas, 0.0);
    break;
  case Qt::Key_Q:
    vue.rotate(petit_angle, 0.0, 0.0, -1.0);
    break;
  case Qt::Key_E:
    vue.rotate(petit_angle, 0.0, 0.0, +1.0);
    break;
  case Qt::Key_Home:
    vue.initializePosition();
    break;

  // pause ou reprise
  case Qt::Key_Space:
    pause();
    break;

  // changement d'intégrateur
  case Qt::Key_F1:
    s.changeIntegrateur(1);
    break;
  case Qt::Key_F2:
    s.changeIntegrateur(2);
    break;
  case Qt::Key_F3:
    s.changeIntegrateur(3);
    break;

  // change dessin : cube ou sphère colorée
  case Qt::Key_V:
    vue.changeDessin();
    break;

  // gestion portraits des phases
  case Qt::Key_1:
    phase(0);
    break;
  case Qt::Key_2:
    phase(1);
    break;
  case Qt::Key_3:
    phase(2);
    break;
  case Qt::Key_4:
    phase(3);
    break;
  case Qt::Key_5:
    phase(4);
    break;
  case Qt::Key_6:
    phase(5);
    break;
  case Qt::Key_7:
    phase(6);
    break;
  case Qt::Key_8:
    phase(7);
    break;
  case Qt::Key_9:
    phase(8);
    break;
  case Qt::Key_0: // ferme tous les portraits
    for(size_t i(0);i<phases.size();++i) phase(i, false, true);
    break;
  case Qt::Key_O: // ouvre tous les portraits
    for(size_t i(0);i<phases.size();++i) phase(i, true);
    break;

  };

  updateGL(); // redessine
}

// ======================================================================
void GLWidget::timerEvent(QTimerEvent* event){
  Q_UNUSED(event);
  double dt = chronometre.restart() / 1000.0;
  s.evolue(dt); // évolution du système
  for(size_t i(0);i<phases.size();i++){ // pour le dessin des phases, ajoute un point au portrait a chaque "evolution" du système
    if(phases[i]!=nullptr) phases[i]->add({s.get_col()[i]->PQ()[0],s.get_col()[i]->PQ()[1],0});
  }
  updateGL();
}

// ======================================================================
void GLWidget::pause(){
  if (timerId == 0) {
    // dans ce cas le timer ne tourne pas alors on le lance
    timerId = startTimer(20);
    chronometre.restart();
  } else {
    // le timer tourne alors on l'arrête
    killTimer(timerId);
    timerId = 0;
  }
}

// gère les fenêtres des portraits des phases : si elle est fermée (que ce soit par les touches
// clavier ou par la souris, en appuyant sur la croix), on la rouvre et si elle est ouverte,
// alors on la ferme
void GLWidget::phase(unsigned int i, bool openAll, bool closeAll){
  if(i<phases.size()){
    if(phases[i]!=nullptr and not phases[i]->isVisible()){ // signifie que la fenêtre a été fermé "manuellement" (avec la croix) auparavant
      // on simule alors une fermeture par les touches clavier à ce moment la, il aurait mieux de la faire avec une methode de gestion
      // d'evenement PWidget::closeEvent(QCloseEvent*), mais on ne sait pas comment atteindre la collection phases de GLWidget lors
      // de la fermeture d'une fenetre PWidget
      s.changePhase(i);
      phases[i].reset(nullptr);
    }

    if(phases[i]!=nullptr and not openAll){ // si la fenêtre est affichée (sinon elle aurait prise par le if précédent) et qu'on ne veut pas tout ouvrir
      phases[i]->hide();
      phases[i].reset(nullptr);
      s.changePhase(i);
    } else if(phases[i]==nullptr and not closeAll){ // pour ouvrir une nouvelle fenêtre
      phases[i].reset(new PWidget());
      phases[i]->show();
      s.changePhase(i);
    }
    // note : on appelle les méthode hide() et show() de PWidget via les pointeurs seulement lorsqu'on est sûr que ce ne sont pas des pointeurs nuls
  }
}
