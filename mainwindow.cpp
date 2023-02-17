#include "mainwindow.h"

#include "gif.h"
#include "ui_mainwindow.h"

GifWriter writer;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  loadParametrs = false;
  surfaces = new List;
  surfaces->head = NULL;
  surfaces->tail = NULL;
  nodes_Array = NULL;
  nodes_static = NULL;
  nodes_dynamic = NULL;
  nodeAmount = 0;
  edgeAmount = 0;
  width = 640;
  height = 480;
  delay = 10;
  borderlands = new double[6];
  borderlands_2 = new double[6];

  ui->setupUi(this);
  loadSettings();

  ui->slider_trans_x->setMinimum(-100);
  ui->slider_trans_x->setMaximum(100);

  ui->slider_trans_y->setMinimum(-100);
  ui->slider_trans_y->setMaximum(100);

  ui->slider_trans_z->setMinimum(-100);
  ui->slider_trans_z->setMaximum(100);

  ui->slider_rot_x->setMinimum(-200);
  ui->slider_rot_x->setMaximum(200);

  ui->slider_rot_y->setMinimum(-200);
  ui->slider_rot_y->setMaximum(200);

  ui->slider_rot_z->setMinimum(-200);
  ui->slider_rot_z->setMaximum(200);

  ui->slider_scale->setMinimum(1);
  ui->slider_scale->setMaximum(1000);

  ui->slider_trans_x->hide();
  ui->slider_trans_y->hide();
  ui->slider_trans_z->hide();
  ui->slider_rot_x->hide();
  ui->slider_rot_y->hide();
  ui->slider_rot_z->hide();
  ui->slider_scale->hide();
  ui->label_trans->hide();
  ui->label_rot->hide();
  ui->line_3->hide();
  ui->label_scale->hide();

  setDefaultSlider();

  this->setWindowTitle("3DViewer_v1.0");
  setFixedSize(geometry().width(), geometry().height());

  connect(ui->pushButtonFile, &QPushButton::released, this,
          &MainWindow::LoadFile);
  connect(ui->slider_trans_x, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_trans_y, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_trans_z, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_rot_x, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_rot_y, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_rot_z, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
  connect(ui->slider_scale, SIGNAL(valueChanged(int)), this,
          SLOT(AdjustCoordinates()));
}

MainWindow::~MainWindow() { allDelete(); }

void MainWindow::updateView() {
  this->ui->openGLWidget->nodes_dynamic = nodes_dynamic;
  this->ui->openGLWidget->poligons = surfaces;
  this->ui->openGLWidget->nodeAmount = nodeAmount;
  this->ui->openGLWidget->borderlands = borderlands;
  this->ui->openGLWidget->width = width;
  this->ui->openGLWidget->height = height;
  this->ui->openGLWidget->nodes_Array = nodes_Array;
  ui->openGLWidget->update();
}

void MainWindow::allDelete() {
  for (int i = 0; i < nodeAmount; i++) {
    delete[] nodes_static[i];
    delete[] nodes_dynamic[i];
  }
  if (nodes_static) {
    delete[] nodes_static;
    delete[] nodes_dynamic;
    delete[] nodes_Array;
  }
  delete[] borderlands;
  delete[] borderlands_2;
  destroyList(surfaces);
}

void MainWindow::LoadFile() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("Open Object"), "../obj/", tr("Object Files (*.obj)"));

  if (!fileName.isEmpty()) {
    List *nodes1 = new List;
    List *nodes2 = new List;
    nodes1->head = NULL;
    nodes1->tail = NULL;
    nodes2->head = NULL;
    nodes2->tail = NULL;

    if (nodes_static) {
      allDelete();
      surfaces = new List;
      surfaces->head = NULL;
      surfaces->tail = NULL;
      borderlands = new double[6];
      borderlands_2 = new double[6];

      nodeAmount = 0;
      edgeAmount = 0;
      this->ui->openGLWidget->nodeAmount = nodeAmount;
      setDefaultSlider();
    }

    QFileInfo fileInfo(fileName);
    QString fileName_OnlyFile(fileInfo.fileName());

    ui->editFileName->setText(fileName);
    ui->label_Filename->setText(fileName_OnlyFile);

    QByteArray tempArray = fileName.toLocal8Bit();
    char *filePath = tempArray.data();

    if (!loadFromFile(filePath, nodes1, nodes2, surfaces, &nodeAmount,
                      &edgeAmount, borderlands, borderlands_2)) {
      ui->label_Nodes->setText(QString::number(nodeAmount));
      ui->label_Edges->setText(QString::number(edgeAmount));

      nodes_static = new double *[nodeAmount];
      nodes_dynamic = new double *[nodeAmount];
      nodes_Array = new double[nodeAmount * 3];
      int j = 0, i = 0;
      for (Node *tmp = nodes1->head, *tmp2 = nodes2->head; tmp;
           tmp = tmp->next, tmp2 = tmp2->next, j++) {
        nodes_static[j] = tmp->coords;
        nodes_dynamic[j] = tmp2->coords;
        nodes_Array[i++] = tmp2->coords[0];
        nodes_Array[i++] = tmp2->coords[1];
        nodes_Array[i++] = tmp2->coords[2];
      }

      ui->slider_trans_x->show();
      ui->slider_trans_y->show();
      ui->slider_trans_z->show();
      ui->slider_rot_x->show();
      ui->slider_rot_y->show();
      ui->slider_rot_z->show();
      ui->slider_scale->show();
      ui->label_trans->show();
      ui->label_rot->show();
      ui->line_3->show();
      ui->label_scale->show();

      updateView();
    }
    destroyList(nodes1);
    destroyList(nodes2);
  }
}

void MainWindow::setDefaultSlider() {
  ui->slider_trans_x->blockSignals(true);
  ui->slider_trans_y->blockSignals(true);
  ui->slider_trans_z->blockSignals(true);
  ui->slider_rot_x->blockSignals(true);
  ui->slider_rot_y->blockSignals(true);
  ui->slider_rot_z->blockSignals(true);
  ui->slider_scale->blockSignals(true);

  ui->slider_trans_x->setValue(0);
  ui->slider_trans_y->setValue(0);
  ui->slider_trans_z->setValue(0);
  ui->slider_rot_x->setValue(0);
  ui->slider_rot_y->setValue(0);
  ui->slider_rot_z->setValue(0);
  ui->slider_scale->setValue(100);

  ui->slider_trans_x->blockSignals(false);
  ui->slider_trans_y->blockSignals(false);
  ui->slider_trans_z->blockSignals(false);
  ui->slider_rot_x->blockSignals(false);
  ui->slider_rot_y->blockSignals(false);
  ui->slider_rot_z->blockSignals(false);
  ui->slider_scale->blockSignals(false);
}

void MainWindow::AdjustCoordinates() {
  trans_x = (double)ui->slider_trans_x->value() / 10;
  trans_y = (double)ui->slider_trans_y->value() / 10;
  trans_z = (double)ui->slider_trans_z->value() / 10;
  rot_x = (double)ui->slider_rot_x->value();
  rot_y = (double)ui->slider_rot_y->value();
  rot_z = (double)ui->slider_rot_z->value();
  scale = (double)ui->slider_scale->value() / 100;

  adjustTranslate(nodes_static, nodes_dynamic, nodeAmount, trans_x, trans_y,
                  trans_z, borderlands_2, borderlands);
  adjustRotate(nodes_dynamic, nodeAmount, rot_x, rot_y, rot_z);
  adjustScale(nodes_dynamic, nodeAmount, scale);

  for (int i = 0, j = 0; i < nodeAmount; i++) {
    nodes_Array[j++] = nodes_dynamic[i][0];
    nodes_Array[j++] = nodes_dynamic[i][1];
    nodes_Array[j++] = nodes_dynamic[i][2];
  }

  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_Photo_clicked() {
  QString selectedFilter = "JPG (*jpeg *jpg)";
  QString fileName = QFileDialog::getSaveFileName(
      0, "Сохранить как...", QDir::currentPath(),
      "JPG (*.jpeg, *.jpg);; BMP (*.bmp)", &selectedFilter);

  QMessageBox msgBox;
  if (fileName.isNull()) {
    msgBox.setText("Файл не сохранен!");
  } else {
    QPixmap pixmap(ui->openGLWidget->size());
    ui->openGLWidget->render(&pixmap);
    if (!fileName.contains(".jpg") && !fileName.contains(".bmp")) {
      if (selectedFilter.contains("JPG"))
        fileName += ".jpg";
      else if (selectedFilter.contains("BMP"))
        fileName += ".bmp";
    }

    pixmap.save(fileName, nullptr);
    msgBox.setText("Готово");
  }
  msgBox.exec();
}

void MainWindow::on_pushButton_GifRun_clicked() {
  fileNameRec = QFileDialog::getSaveFileName(
      0, "Сохранить как...", QDir::currentPath(), "GIF (*.gif)");

  if (fileNameRec.isNull()) {
    QMessageBox msgBox;
    msgBox.setText("Файл не сохранен!");
    msgBox.exec();
  } else {
    if (!fileNameRec.contains(".gif")) {
      fileNameRec += ".gif";
    }
    if (GifBegin(&writer, fileNameRec.toLatin1().data(), width, height,
                 delay)) {
      ui->pushButton_GifRun->setText("Gif пишется");
      time = QTime::currentTime();
      timer = new QTimer(this);
      connect(timer, SIGNAL(timeout()), this, SLOT(slotTimerAlarm()));
      timer->start(100);
    }
  }
}

void MainWindow::slotTimerAlarm() {
  QImage img = ui->openGLWidget->grabFramebuffer().scaled(
      width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  GifWriteFrame(&writer,
                img.convertToFormat(QImage::Format_Indexed8)
                    .convertToFormat(QImage::Format_RGBA8888)
                    .bits(),
                img.width(), img.height(), delay);

  QTime end = QTime::currentTime();
  if (time.secsTo(end) > 5) {
    GifEnd(&writer);
    QMessageBox msgBox;
    msgBox.setText("Готово");
    ui->pushButton_GifRun->setText("Начать запись Gif");
    msgBox.exec();
    timer->stop();
  }
}

void MainWindow::on_pushButton_clicked() {
  if (!loadParametrs) return;
  ui->openGLWidget->uColorNodes =
      QColorDialog::getColor(ui->openGLWidget->uColorNodes, this);
  ui->openGLWidget->update();

  QSettings settings("settings.conf", "settings");
  QColor uColorNodes = ui->openGLWidget->uColorNodes;

  settings.beginGroup("Nodes");
  settings.setValue("red", uColorNodes.redF());
  settings.setValue("green", uColorNodes.greenF());
  settings.setValue("blue", uColorNodes.blueF());
  settings.setValue("alpha", uColorNodes.alphaF());
  settings.endGroup();
}

void MainWindow::on_pushButton_3_clicked() {
  if (!loadParametrs) return;
  ui->openGLWidget->uColorFons =
      QColorDialog::getColor(ui->openGLWidget->uColorFons, this);
  ui->openGLWidget->update();

  QSettings settings("settings.conf", "settings");
  QColor uColorFons = ui->openGLWidget->uColorFons;

  settings.beginGroup("Background");
  settings.setValue("red", uColorFons.redF());
  settings.setValue("green", uColorFons.greenF());
  settings.setValue("blue", uColorFons.blueF());
  settings.setValue("alpha", uColorFons.alphaF());
  settings.endGroup();
}

void MainWindow::on_pushButton_2_clicked() {
  if (!loadParametrs) return;
  ui->openGLWidget->uColorPoligons =
      QColorDialog::getColor(ui->openGLWidget->uColorPoligons, this);
  ui->openGLWidget->update();

  QSettings settings("settings.conf", "settings");
  QColor uColorPoligons = ui->openGLWidget->uColorPoligons;

  settings.beginGroup("Poligons");
  settings.setValue("red", uColorPoligons.redF());
  settings.setValue("green", uColorPoligons.greenF());
  settings.setValue("blue", uColorPoligons.blueF());
  settings.setValue("alpha", uColorPoligons.alphaF());
  settings.endGroup();
}

void MainWindow::on_spinBox_valueChanged(int arg1) {
  if (!loadParametrs) return;
  ui->openGLWidget->lineWidth = arg1;
  QSettings settings("settings.conf", "settings");
  settings.beginGroup("Poligons");
  settings.setValue("LineWidth", arg1);
  settings.endGroup();

  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_nodes_valueChanged(int arg1) {
  if (!loadParametrs) return;
  ui->openGLWidget->pointSize = arg1;
  QSettings settings("settings.conf", "settings");
  settings.beginGroup("Nodes");
  settings.setValue("PointSize", ui->openGLWidget->pointSize);
  settings.endGroup();

  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_projection_central_3_clicked() { saveOrtho(1); }

void MainWindow::on_radioButton_projection_parallel_3_clicked() {
  saveOrtho(0);
}

void MainWindow::saveOrtho(int val) {
  if (!loadParametrs) return;
  ui->openGLWidget->ortho = val;
  QSettings settings("settings.conf", "settings");
  settings.beginGroup("Background");
  settings.setValue("ortho", val);
  settings.endGroup();
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_nodes_no_clicked() { savePointType(0); }

void MainWindow::on_radioButton_nodes_square_clicked() { savePointType(1); }

void MainWindow::on_radioButton_nodes_circle_clicked() { savePointType(2); }

void MainWindow::savePointType(int val) {
  if (!loadParametrs) return;
  ui->openGLWidget->pointType = val;
  QSettings settings("settings.conf", "settings");
  settings.beginGroup("Nodes");
  settings.setValue("pointType", val);
  settings.endGroup();
  ui->openGLWidget->update();
}

void MainWindow::on_radioButton_poligons_no_clicked() { saveStipple(0); }

void MainWindow::on_radioButton_poligons_stipple_clicked() { saveStipple(1); }

void MainWindow::on_radioButton_poligons_line_clicked() { saveStipple(2); }

void MainWindow::saveStipple(int val) {
  if (!loadParametrs) return;
  ui->openGLWidget->stipple = val;
  QSettings settings("settings.conf", "settings");
  settings.beginGroup("Poligons");
  settings.setValue("Stipple", val);
  settings.endGroup();
  ui->openGLWidget->update();
}

void MainWindow::loadSettings() {
  QSettings settings("settings.conf", "settings");
  QColor *uColorFons = &(ui->openGLWidget->uColorFons);
  QColor *uColorPoligons = &(ui->openGLWidget->uColorPoligons);
  QColor *uColorNodes = &(ui->openGLWidget->uColorNodes);

  settings.beginGroup("Background");
  uColorFons->setRedF(settings.value("red", 0).toDouble());
  uColorFons->setGreenF(settings.value("green", 0).toDouble());
  uColorFons->setBlueF(settings.value("blue", 0).toDouble());
  uColorFons->setAlphaF(settings.value("alpha", 1).toDouble());
  ui->openGLWidget->ortho = settings.value("ortho", 1).toInt();
  settings.endGroup();

  if (ui->openGLWidget->ortho == 1)
    ui->radioButton_projection_central_3->setChecked(true);
  else
    ui->radioButton_projection_parallel_3->setChecked(true);

  settings.beginGroup("Poligons");
  uColorPoligons->setRedF(settings.value("red", 0).toDouble());
  uColorPoligons->setGreenF(settings.value("green", 1).toDouble());
  uColorPoligons->setBlueF(settings.value("blue", 0).toDouble());
  uColorPoligons->setAlphaF(settings.value("alpha", 1).toDouble());
  ui->openGLWidget->stipple = settings.value("Stipple", 2).toInt();
  ui->openGLWidget->lineWidth = settings.value("LineWidth", 1).toInt();
  settings.endGroup();

  ui->spinBox->setValue(ui->openGLWidget->lineWidth);

  if (ui->openGLWidget->stipple == 0)
    ui->radioButton_poligons_no->setChecked(true);
  else if (ui->openGLWidget->stipple == 1)
    ui->radioButton_poligons_stipple->setChecked(true);
  else if (ui->openGLWidget->stipple == 2)
    ui->radioButton_poligons_line->setChecked(true);

  settings.beginGroup("Nodes");
  uColorNodes->setRedF(settings.value("red", 1).toDouble());
  uColorNodes->setGreenF(settings.value("green", 0).toDouble());
  uColorNodes->setBlueF(settings.value("blue", 0).toDouble());
  uColorNodes->setAlphaF(settings.value("alpha", 1).toDouble());
  ui->openGLWidget->pointSize = settings.value("PointSize", 1).toInt();
  ui->openGLWidget->pointType = settings.value("pointType", 0).toInt();
  settings.endGroup();

  ui->spinBox_nodes->setValue(ui->openGLWidget->pointSize);

  if (ui->openGLWidget->pointType == 0)
    ui->radioButton_nodes_no->setChecked(true);
  else if (ui->openGLWidget->pointType == 1)
    ui->radioButton_nodes_square->setChecked(true);
  else if (ui->openGLWidget->pointType == 2)
    ui->radioButton_nodes_circle->setChecked(true);

  loadParametrs = true;
  ui->openGLWidget->update();
}
