#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QWidget>

extern "C" {
#include "viewer.h"
}

class glView : public QOpenGLWidget {
 public:
  glView(QWidget *parent = 0);
  ~glView();

  int lineWidth, pointSize, nodeAmount, pointType, stipple, ortho, width,
      height;
  double **nodes_dynamic, *nodes_Array, *borderlands;
  QColor uColorNodes, uColorFons, uColorPoligons;
  List *poligons;

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
};

#endif  // GLVIEW_HPP
