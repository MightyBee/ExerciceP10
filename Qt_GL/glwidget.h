#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>        // Classe pour faire une fenêtre OpenGL
#include <QOpenGLShaderProgram> // Classe qui wrap les fonctions OpenGL liées aux shaders
#include <QTime>                // Classe pour gérer le temps
#include <array>
#include <vector>
#include <memory>
#include <cmath>
#include "vue_opengl.h"
#include "Systeme.h"
#include "Integrateur.h"


/*##############################################################################
###                                                                          ###
###                             CLASSE PWidget                               ###
###                                                                          ###
##############################################################################*/

// fenêtre pour afficher le portrait des phases
class PWidget : public QGLWidget{
public:
  // constructeur(s)
  PWidget(QWidget* parent = nullptr) : QGLWidget(parent) , phases(0), maxX(0), minX(0), maxY(0), minY(0) {}
  // destructeur
  virtual ~PWidget() {}
  // méthodes utilitaires publiques
  void add(std::array<double,3> const& pqt); // ajoute un point dans l'espace des phases

private:
  // Les 3 méthodes clés de la classe QOpenGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;
  // méthodes utilitaires privées
  double ratio() const {return (1.0*width())/(1.0*height());} // retourne le ratio entre la largeur et la hauteur de la fenêtre

  //############## attributs ##############//
  QOpenGLShaderProgram prog; // Un shader OpenGL encapsulé dans une classe Qt
  std::vector<std::array<double,3>> phases; // tableau contenant [P(t),P'(t),t] pour chaque t
  double maxX; // la valeur max sur l'axe des x
  double minX; // la valeur min sur l'axe des x
  double maxY; // la valeur max sur l'axe des y
  double minY; // la valeur min sur l'axe des y
};


/*##############################################################################
###                                                                          ###
###                            CLASSE GLWidget                               ###
###                                                                          ###
##############################################################################*/

/* Fenêtre principale pour l'affichage du système ;
 * la fenêtre hérite de QGLWidget ;
 * les événements (clavier, souris, temps) sont des méthodes virtuelles à redéfinir.
 */
class GLWidget : public QGLWidget{
public:
  // constructeur(s)
  GLWidget(Integrateur const& integrat, QWidget* parent = nullptr)
    : QGLWidget(parent) , s(&vue, integrat), showQueues(false), queues(s.taille()), phases(s.taille()) {}
  GLWidget(const GLWidget&) =delete;
  GLWidget& operator=(const GLWidget&) =delete;
  // destructeur
  virtual ~GLWidget() {}
  // méthodes utilitaires publiques
  void add(Oscillateur const& osc); // permet d'ajouter un oscillateur au système

private:
  // Les 3 méthodes clés de la classe QGLWidget à réimplémenter
  virtual void initializeGL()                  override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL()                       override;

  // Méthodes de gestion d'évènements
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void timerEvent(QTimerEvent* event)  override;
  virtual void showEvent(QShowEvent* event)    override; // lorsque la fenêtre s'ouvre
  virtual void closeEvent(QCloseEvent* event)  override; // lorsque la fenêtre se ferme

  // Méthodes de gestion interne
  void pause(); // met en pause la simulation si elle tourne, et la relance si elle est en pause
  void phase(unsigned int i, bool openAll = false, bool closeAll = false); // gestion ouverture/fermeture des fenêtres des phases et de l'attribut "phases"

  //############## attributs ##############//
  VueOpenGL vue; // Vue : ce qu'il faut donner au système pour qu'il puisse se dessiner sur la vue
  int timerId; // Timer
  QTime chronometre; // pour faire évoluer les objets avec le bon "dt"

  Systeme s; // système à dessiner, faire évoluer
  bool showQueues;
  std::vector<QueueOscillateur> queues;
  std::vector<std::unique_ptr<PWidget>> phases; // collection de pointeurs sur des fenêtre pour dessiner l'espace des phases des oscillateurs du sytsème
};

#endif // GLWIDGET_H
