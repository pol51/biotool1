#pragma once

#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPinchGesture>

#include <QtGui/QOpenGLTexture>

#include <QtCore/QTimer>

#include "dataCtrl.h"

class ImageView : public QOpenGLWidget
{
  Q_OBJECT

  public:
    ImageView(QWidget *parent = nullptr);
    ~ImageView() override;

    void changeMode(DataCtrl::EMode mode) { dataCtrl->setCurrentMode(mode); onMoveDecal = false; }
    void setImageRealWidth(const qreal &width);
    void setImageRealHeight(const qreal &height);

    DataCtrl& data() { return *dataCtrl; }
    const DataCtrl& data() const { return *dataCtrl; }

    const qreal &imageRealWidth()  const { return dataCtrl->imageRealWidth(); }
    const qreal &imageRealHeight() const { return dataCtrl->imageRealHeigth(); }

  public slots:
    void doChangeImage(const QImage &image);
    void doCloseImage();

    void doZoomIn()     { if (zoom > 1) --zoom; }
    void doZoomOut()    { if (zoom < 11) ++zoom; }
    void doResetView()  { zoom = 1.; xDecal = yDecal = 0.; resizeGL(width(), height()); }

    bool event(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

  private:
    bool gestureEvent(QGestureEvent *event);
    void pinchTriggered(QPinchGesture *gesture);

  protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

  protected:
    bool onMoveDecal = false;
    qreal zoom = 1.;
    DataCtrl *dataCtrl;
    GLuint imageTexId = 0;
    QOpenGLTexture *_image = nullptr;
    GLfloat xDecal = 0.f, yDecal = 0.f;
    GLfloat ratioWidthPerHeght = 1.f;
    QTimer refreshTimer;
    QPoint lastMousePos;
};
