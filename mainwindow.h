#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBuffer>
#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QPixmap>
#include <QSettings>
#include <QTime>
#include <QTimer>

#ifdef __APPLE__
/* Defined before OpenGL and GLUT includes to avoid deprecation messages */
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

extern "C" {
#include "viewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  int nodeAmount, edgeAmount, width, height, delay;
  double trans_x, trans_y, trans_z, rot_x, rot_y, rot_z, scale;
  double *borderlands, *borderlands_2, **nodes_static, **nodes_dynamic,
      *nodes_Array;
  List *surfaces;
  bool loadParametrs;
  void setDefaultSlider();
  void updateView();
  void allDelete();
  void loadSettings();
  void saveStipple(int val);
  void savePointType(int val);
  void saveOrtho(int val);

  QTimer *timer;
  QTime time;
  QString fileNameRec;

 private slots:
  void LoadFile();
  void AdjustCoordinates();
  void on_pushButton_clicked();
  void on_pushButton_3_clicked();
  void on_pushButton_2_clicked();
  void on_spinBox_valueChanged(int arg1);
  void on_radioButton_nodes_no_clicked();
  void on_spinBox_nodes_valueChanged(int arg1);
  void on_pushButton_Photo_clicked();
  void on_pushButton_GifRun_clicked();
  void slotTimerAlarm();
  void on_radioButton_projection_central_3_clicked();
  void on_radioButton_projection_parallel_3_clicked();
  void on_radioButton_nodes_square_clicked();
  void on_radioButton_nodes_circle_clicked();
  void on_radioButton_poligons_no_clicked();
  void on_radioButton_poligons_stipple_clicked();
  void on_radioButton_poligons_line_clicked();
};
#endif  // MAINWINDOW_H
