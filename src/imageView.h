#ifndef __IMAGEVIEW_H__
#define __IMAGEVIEW_H__

#include <QtOpenGL/QGLWidget>

#include <QtCore/QTimer>

class ImageView : public QGLWidget
{
  Q_OBJECT

  public:
    ImageView(QWidget *parent = NULL);
    ~ImageView();

  public slots:
    void doZoomIn()   { if (zoom > 1) --zoom; }
    void doZoomOut()  { if (zoom < 9) ++zoom; }
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *);

  protected slots:
    void doChangeImage(const QImage &image);

  protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeGL(int w, int h);

  protected:
    QTimer refreshTimer;
    GLuint imageTexId;
    int zoom;
    GLfloat xDecal, yDecal;
    bool onMoveDecal;
    QPoint lastMousePos;
    GLfloat ratioWidthPerHeght;
};

#endif
