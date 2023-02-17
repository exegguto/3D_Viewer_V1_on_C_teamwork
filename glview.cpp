#include "glview.hpp"

glView::glView(QWidget *parent) : QOpenGLWidget(parent) {
  nodeAmount = pointType = stipple = pointSize = 0;
}

glView::~glView() {}

void glView::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}

void glView::paintGL() {
  glClearColor(uColorFons.redF(), uColorFons.greenF(), uColorFons.blueF(),
               uColorFons.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if (nodeAmount > 0) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (ortho == 0) {
      gluPerspective(150, width / height, 0.1, 100.0);
      glTranslated(0, 0, -5);
    } else {
      double newAstro = (borderlands[0] - borderlands[1]) * height * 2 /
                        (borderlands[2] - borderlands[3]) / width;
      glOrtho(borderlands[1] * 2, borderlands[0] * 2, newAstro * borderlands[3],
              newAstro * borderlands[2], -borderlands[2] * newAstro * 10,
              borderlands[2] * newAstro * 10);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (stipple == 1) {
      glEnable(GL_LINE_STIPPLE);
      glLineStipple(1, 0x3333);
    } else {
      if (stipple == 2 || stipple == 0) glDisable(GL_LINE_STIPPLE);
    }
    glLineWidth(lineWidth);

    if (stipple != 0)
      for (Node *poligon_temp = poligons->head; poligon_temp;
           poligon_temp = poligon_temp->next) {
        glBegin(GL_LINE_LOOP);
        glColor3d(uColorPoligons.redF(), uColorPoligons.greenF(),
                  uColorPoligons.blueF());
        for (int i = 0; i < poligon_temp->nodeCount; i++) {
          int num = poligon_temp->nodes[i] - 1;
          if (num < nodeAmount) {
            glVertex3d(nodes_dynamic[num][0], nodes_dynamic[num][1],
                       nodes_dynamic[num][2]);
          }
        }
        glEnd();
      }

    if (pointType > 0) {
      if (pointType == 2) glEnable(GL_POINT_SMOOTH);
      glVertexPointer(3, GL_DOUBLE, 0, nodes_Array);
      glEnableClientState(GL_VERTEX_ARRAY);
      glPointSize(pointSize);
      glColor3d(uColorNodes.redF(), uColorNodes.greenF(), uColorNodes.blueF());
      glDrawArrays(GL_POINTS, 0, nodeAmount);
      glDisableClientState(GL_VERTEX_ARRAY);
      if (pointType == 2) glDisable(GL_POINT_SMOOTH);
    }
  }
}

void glView::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  update();
}
