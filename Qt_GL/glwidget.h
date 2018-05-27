#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>        // Classe pour faire une fenêtre OpenGL
#include <QOpenGLShaderProgram> // Classe qui wrap les fonctions OpenGL liées aux shaders
#include <QTime>            // Classe pour gérer le temps
#include <array>
#include <vector>
#include "vue_opengl.h"
#include "Systeme.h"
#include "Integrateur.h"

class PWidget : public QGLWidget
{
public:
  PWidget(QWidget* parent = nullptr) : QGLWidget(parent) , phases(0), maxX(0), minX(0) {}
  virtual ~PWidget() {}

  void add(std::array<double,3> const& pqt);

private:
  // Les 3 méthodes clés de la classe QOpenGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  double ratio() const {return width()/height();}
  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;

  std::vector<std::array<double,3>> phases;
  double maxX;
  double minX;

};

class GLWidget : public QGLWidget
/* La fenêtre hérite de QGLWidget ;
 * les événements (clavier, souris, temps) sont des méthodes virtuelles à redéfinir.
 */
{
public:
  GLWidget(Integrateur const& integrat, QWidget* parent = nullptr)
    : QGLWidget(parent) , s(&vue, integrat), phases(s.taille()) {}
  virtual ~GLWidget() {}
  void add(Oscillateur const& osc);
  void initializeSysteme(){s.initialize();}
private:
  // Les 3 méthodes clés de la classe QGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;
  virtual void closeEvent (QCloseEvent *event) override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;

  // Méthodes de gestion interne
  void pause();
  void phase(int i, bool openAll = false, bool closeAll = false);

  // Vue : ce qu'il faut donner au contenu pour qu'il puisse se dessiner sur la vue
  VueOpenGL vue;

  // Timer
  int timerId;
  // pour faire évoluer les objets avec le bon "dt"
  QTime chronometre;

  // objets à dessiner, faire évoluer
  Systeme s;
  std::vector<std::unique_ptr<PWidget>> phases;

};


class PhaseWidget : public QOpenGLWidget
{
public:
  PhaseWidget(Oscillateur const& o, Integrateur const& i, double t=10.0, double dt=0.01, QWidget* parent = nullptr)
    : QOpenGLWidget(parent), osc(o.copie()), integrat(i.copie()), tFinal(t), dt(dt) {}

  virtual ~PhaseWidget() {}

private:
  // Les 3 méthodes clés de la classe QOpenGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;



  // Un shader OpenGL encapsulé dans une classe Qt
  QOpenGLShaderProgram prog;
  std::unique_ptr<Oscillateur> osc;
  std::unique_ptr<Integrateur> integrat;
  double tFinal;
  double dt;
};



#endif // GLWIDGET_H
