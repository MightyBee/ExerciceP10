#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "glwidget.h"


// ======================================================================
void GLWidget::add(Oscillateur const& osc){
  s.add(osc);
}

void GLWidget::initializeGL()
{
  vue.init();
  timerId = startTimer(20);
}

// ======================================================================
void GLWidget::resizeGL(int width, int height)
{
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
void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  s.dessine();
}


// ======================================================================
void GLWidget::keyPressEvent(QKeyEvent* event)
{
  constexpr double petit_angle(5.0); // en degrés
  constexpr double petit_pas(1.0);

  switch (event->key()) {

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

  case Qt::Key_Space:
    pause();
    break;

  case Qt::Key_V:
    vue.changeDessin();
    break;

  case Qt::Key_P:
    pause();
    s.phase();
    pause();
    break;

  case Qt::Key_1:
    pause();
    s.phase(1);
    pause();
    break;

  case Qt::Key_2:
    pause();
    s.phase(2);
    pause();
    break;

  case Qt::Key_3:
    pause();
    s.phase(3);
    pause();
    break;

  case Qt::Key_4:
    pause();
    s.phase(4);
    pause();
    break;

  case Qt::Key_5:
    pause();
    s.phase(5);
    pause();
    break;

  };

  updateGL(); // redessine
}

// ======================================================================
void GLWidget::timerEvent(QTimerEvent* event)
{
  Q_UNUSED(event);

  double dt = chronometre.restart() / 1000.0;

  s.evolue(dt);
  updateGL();
}

// ======================================================================
void GLWidget::pause()
{
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

// ======================================================================
void PhaseWidget::initializeGL(){
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
void PhaseWidget::resizeGL(int width, int height)
{
  glViewport(10, 10, width-20, height-20); // on laisse une marge de 10px de chaque coté
}

// ======================================================================
void PhaseWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 matrice;
  prog.setUniformValue("vue_modele", matrice);              // On met la matrice identité dans vue_modele


  /* Change de matrice de projection adpatée aux zoom du graph */
  matrice.setToIdentity();
  double xmin(-5.0);
  double xmax(+5.0);
  double ymin(-5.0);
  double ymax(+5.0);
  matrice.ortho(xmin, xmax, ymin, ymax, -10.0, 10.0);
  prog.setUniformValue("projection", matrice);

  /* Dessine les axes */
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);                                        // la primitive LINES dessine une ligne par paire de points (n/2 lignes)
  prog.setAttributeValue(SommetId, xmin, 0.0, -1.0);        // le -1.0 dans la composante z met les axes en arrière plan
  prog.setAttributeValue(SommetId, xmax, 0.0, -1.0);
  prog.setAttributeValue(SommetId, 0.0, ymin, -1.0);
  prog.setAttributeValue(SommetId, 0.0, ymax, -1.0);
  glEnd();

  /* Dessine les phases */
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);                                   // la primitive LINE_STRIP ne referme par le tracé (n-1 lignes)
  std::unique_ptr<Oscillateur> o(osc->copie());
  for(double t(0.0); t<=tFinal;t+=dt){
    prog.setAttributeValue(SommetId, o->get_P().get_coord(1), o->get_Q().get_coord(1), 0.0);
    integrat->evolue(*o,dt,t);
  }
  glEnd();
}
