#include <QKeyEvent>
#include <QTimerEvent>
#include <QMatrix4x4>
#include "vertex_shader.h" // Identifiants Qt de nos différents attributs
#include "glwidget.h"


// ======================================================================
void GLWidget::add(Oscillateur const& osc){
  s.add(osc);
  phases.push_back(nullptr);
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
void GLWidget::closeEvent (QCloseEvent *event){
  for(size_t i(1);i<=phases.size();++i) phase(i, false, true);
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

  case Qt::Key_F1:
    s.changeIntegrateur(1);
    break;

  case Qt::Key_F2:
    s.changeIntegrateur(2);
    break;

  case Qt::Key_F3:
    s.changeIntegrateur(3);
    break;

  case Qt::Key_V:
    vue.changeDessin();
    break;

  case Qt::Key_1:
    phase(1);
    break;

  case Qt::Key_2:
    phase(2);
    break;

  case Qt::Key_3:
    phase(3);
    break;

  case Qt::Key_4:
    phase(4);
    break;

  case Qt::Key_5:
    phase(5);
    break;

  case Qt::Key_6:
    phase(6);
    break;

  case Qt::Key_7:
    phase(7);
    break;

  case Qt::Key_8:
    phase(8);
    break;

  case Qt::Key_9:
    phase(9);
    break;

  case Qt::Key_0:
    for(size_t i(1);i<=phases.size();++i) phase(i, false, true);
    break;

  case Qt::Key_O:
    for(size_t i(1);i<=phases.size();++i) phase(i, true);
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
  for(size_t i(0);i<phases.size();i++){
    if(phases[i]!=nullptr) phases[i]->add({s.get_col()[i]->PQ()[0],s.get_col()[i]->PQ()[1],0});
  }
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

void GLWidget::phase(int i, bool openAll, bool closeAll){
  --i;
  if(i>=0 and i<phases.size()){
    if(phases[i]!=nullptr and not phases[i]->isVisible()) phases[i].reset(nullptr);
    if(phases[i]!=nullptr and not openAll){
      phases[i]->hide();
      phases[i].reset(nullptr);
    } else if(phases[i]==nullptr and not closeAll){
      phases[i].reset(new PWidget());
      phases[i]->show();
    }
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
    prog.setAttributeValue(SommetId, o->get_P()[0], o->get_Q()[0], 0.0);
    integrat->evolue(*o,dt,t);
  }
  glEnd();
}









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
void PWidget::resizeGL(int width, int height)
{
  glViewport(10, 10, width-20, height-20); // on laisse une marge de 10px de chaque coté
}

// ======================================================================
void PWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 matrice;
  prog.setUniformValue("vue_modele", matrice);              // On met la matrice identité dans vue_modele


  /* Change de matrice de projection adpatée aux zoom du graph */
  matrice.setToIdentity();
  matrice.ortho(minX, maxX, minX/ratio(), maxX/ratio(), -10.0, 10.0);
  prog.setUniformValue("projection", matrice);

  /* Dessine les axes */
  prog.setAttributeValue(CouleurId, 0.0, 0.0, 1.0);
  glBegin(GL_LINES);                                        // la primitive LINES dessine une ligne par paire de points (n/2 lignes)
  prog.setAttributeValue(SommetId, minX, 0.0, -1.0);        // le -1.0 dans la composante z met les axes en arrière plan
  prog.setAttributeValue(SommetId, maxX, 0.0, -1.0);
  for(int i(1);i<maxX or -i>minX;++i){
    prog.setAttributeValue(SommetId,   i,-0.1, -1.0);
    prog.setAttributeValue(SommetId,   i, 0.1, -1.0);
    prog.setAttributeValue(SommetId,  -i,-0.1, -1.0);
    prog.setAttributeValue(SommetId,  -i, 0.1, -1.0);
    prog.setAttributeValue(SommetId,-0.1,   i, -1.0);
    prog.setAttributeValue(SommetId, 0.1,   i, -1.0);
    prog.setAttributeValue(SommetId,-0.1,  -i, -1.0);
    prog.setAttributeValue(SommetId, 0.1,  -i, -1.0);
  }
  prog.setAttributeValue(SommetId, 0.0, minX/ratio(), -1.0);
  prog.setAttributeValue(SommetId, 0.0, maxX/ratio(), -1.0);
  glEnd();

  /* Dessine les phases */
  prog.setAttributeValue(CouleurId, 0.0, 1.0, 0.0);
  glBegin(GL_LINE_STRIP);                                   // la primitive LINE_STRIP ne referme par le tracé (n-1 lignes)
  for(auto const& point : phases){
    prog.setAttributeValue(SommetId, point[0], point[1], 0.0);
  }
  glEnd();
  prog.setAttributeValue(CouleurId, 1.0, 0.0, 0.1);
  glPointSize(5);
  glBegin(GL_POINTS);
  prog.setAttributeValue(SommetId, phases.back()[0], phases.back()[1], 0.0);
  glEnd();
}


void PWidget::add(std::array<double,3> const& pqt){
  if(phases.size()>=1000000) phases.erase(phases.begin());
  if(pqt[0]>maxX) maxX=pqt[0];
  if(pqt[0]<minX) minX=pqt[0];
  if(pqt[1]>maxX/ratio()) maxX=ratio()*pqt[1];
  if(pqt[1]<minX/ratio()) minX=ratio()*pqt[1];
  phases.push_back(pqt);
  updateGL();
}
