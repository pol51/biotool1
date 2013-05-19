#include "imageView.h"

#include <QtGui/QMouseEvent>

ImageView::ImageView(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent),
  dataCtrl(new DataCtrl(this))
{
  refreshTimer.start(20, this);
}

ImageView::~ImageView()
{
  if (imageTexId) deleteTexture(imageTexId);
  delete dataCtrl;
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    const DataCtrl::EMode CurrentMode = dataCtrl->currentMode();
    onMoveDecal = (event->type() == QEvent::MouseButtonPress && CurrentMode == DataCtrl::eModeView);
    lastMousePos = QVector2D(event->pos());

    if ((1<<CurrentMode) & ((1<<DataCtrl::eModeEdit) | (1<<DataCtrl::eModeDefineCentroid)))
    {
      QPointF Point((event->x() - (width()/2)) * xRatio - xDecal, (event->y() - (height()/2)) * yRatio - yDecal);
      dataCtrl->addPoint(Point);
    }
  }

  setFocus();

  QGLWidget::mousePressEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    onMoveDecal = false;

  QGLWidget::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
  if (onMoveDecal && event->type())
  {
    QVector2D Diff = QVector2D(event->localPos()) - lastMousePos;
    xDecal += Diff.x() * xRatio;
    yDecal += Diff.y() * yRatio;
  }
  lastMousePos = QVector2D(event->pos());

  if (!onMoveDecal && !(event->buttons() & Qt::LeftButton))
    QGLWidget::mouseMoveEvent(event);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
  if (event->orientation() == Qt::Vertical)
  {
    if (event->delta() > 0) doZoomOut();
    else doZoomIn();
  }
  else
    QGLWidget::wheelEvent(event);
}

void ImageView::keyPressEvent(QKeyEvent *event)
{
  switch (event->key())
  {
    case Qt::Key_Backspace:
      if (event->modifiers() & Qt::ControlModifier)
        dataCtrl->removeLastForm();
      else
        dataCtrl->removeLastPoint();
      break;
    case Qt::Key_Space:
      dataCtrl->finalizeForm();
      break;
    case Qt::Key_Plus:
      doZoomIn();
      break;
    case Qt::Key_Minus:
      doZoomOut();
      break;
    default:
      break;
  }

  QGLWidget::keyPressEvent(event);
}

void ImageView::doChangeImage(const QImage &image)
{
  if (imageTexId) deleteTexture(imageTexId);
  imageTexId = bindTexture(image);
  glBindTexture(GL_TEXTURE_2D, imageTexId);

  geometries.init((image.width() / GLfloat(image.height())));
}

void ImageView::doCloseImage()
{
  if (imageTexId) deleteTexture(imageTexId);
  imageTexId = 0;
}

void ImageView::initializeGL()
{
  initializeGLFunctions();
  qglClearColor(Qt::black);
  initShaders();

  glEnable(GL_CULL_FACE);

  geometries.init(1.f);
}

void ImageView::initShaders()
{
  setlocale(LC_NUMERIC, "C");

  if (!program.addShaderFromSourceFile(QGLShader::Vertex,   ":/vshader.glsl"))  close();
  if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))  close();

  if (!program.link()) close();
  if (!program.bind()) close();

  setlocale(LC_ALL, "");
}

void ImageView::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  QMatrix4x4 Translation;
  Translation.translate(xDecal, yDecal, -distance);

  program.setUniformValue("mvp_matrix", projection * Translation);
  program.setUniformValue("texture", 0);
  program.setUniformValue("color", QVector3D(1., 1., 1.));
  program.setUniformValue("useTexture", true);

  geometries.draw(&program);

  program.setUniformValue("useTexture", false);
  dataCtrl->draw(&program);
}

void ImageView::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  const qreal Aspect = qreal(w) / qreal(h ? h : 1);
  const qreal ZNear = Z_MIN * 0.9, ZFar = Z_MAX * 1.1, Fov = 45;

  projection.setToIdentity();
  projection.perspective(Fov, Aspect, ZNear, ZFar);
  projection.scale(1,-1, 1);

  updateRatio();
}

void ImageView::updateRatio()
{
  const qreal w(width()), h(height());
  const qreal Aspect = w / (h ? h : 1);
  xRatio = (distance / ((projection.column(0).x()) / 2.))/w;
  yRatio = (distance / ((projection.column(0).x()) / 2.))/h / Aspect;
}
