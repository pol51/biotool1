#ifndef __IMAGEVIEW_H__
#define __IMAGEVIEW_H__

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFunctions>

#include <QtCore/QBasicTimer>

#include "dataCtrl.h"
#include "geometryEngine.h"

#define Z_MAX 30.0
#define Z_MIN 0.0001

class ImageView : public QGLWidget, protected QGLFunctions
{
  Q_OBJECT

  public:
    ImageView(QWidget *parent = NULL);
    ~ImageView();

    void changeMode(DataCtrl::EMode mode) { dataCtrl->setCurrentMode(mode); onMoveDecal = false; }

    DataCtrl& data() { return *dataCtrl; }
    const DataCtrl& data() const { return *dataCtrl; }

  public slots:
    void doZoomOut()    { distance *= 1.1; if (distance > Z_MAX) distance = Z_MAX; updateRatio(); }
    void doZoomIn()     { distance *= 0.9; if (distance < Z_MIN) distance = Z_MIN; updateRatio(); }
    void doResetView()  { distance = 2.5; xDecal = yDecal = 0.; resizeGL(width(), height()); }

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent */*event*/) { update(); }

  protected slots:
    void doChangeImage(const QImage &image);
    void doCloseImage();

  protected:
    void initializeGL();
    void initShaders();
    void paintGL();
    void resizeGL(int w, int h);
    void updateRatio();

  protected:
    bool onMoveDecal = false;
    qreal distance = 2.5;
    DataCtrl *dataCtrl = NULL;
    GLuint imageTexId = 0;
    qreal xDecal = 0.0, yDecal = 0.0;
    qreal xRatio = 1.f, yRatio = 1.f;
    QBasicTimer refreshTimer;
    QVector2D lastMousePos;

    QMatrix4x4 projection;
    QGLShaderProgram program;
    GeometryEngine geometries;
};

#endif
