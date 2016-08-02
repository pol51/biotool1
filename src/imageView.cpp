#include "imageView.h"

#include <QtGui/QMouseEvent>

#include <QtCore/QDebug>

ImageView::ImageView(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent),
  dataCtrl(new DataCtrl(this))
{
  grabGesture(Qt::PinchGesture);
  connect(&refreshTimer, &QTimer::timeout, this, &ImageView::updateGL);
  refreshTimer.start(20);
}

ImageView::~ImageView()
{
  if (imageTexId) deleteTexture(imageTexId);
  delete dataCtrl;
}

void ImageView::setImageRealWidth(const qreal &width)
{
  dataCtrl->setImageRealWidth(width);
  dataCtrl->setImageRealHeigth(width / ratioWidthPerHeght);
}

void ImageView::setImageRealHeight(const qreal &height)
{
  dataCtrl->setImageRealHeigth(height);
  dataCtrl->setImageRealWidth(height * ratioWidthPerHeght);
}

bool ImageView::event(QEvent *event)
{
  if (event->type() == QEvent::Gesture)
    return gestureEvent((QGestureEvent*)event);
  return QGLWidget::event(event);
}

bool ImageView::gestureEvent(QGestureEvent *event)
{
    qDebug() << "gestureEvent():" << event;
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void ImageView::pinchTriggered(QPinchGesture *gesture)
{
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  if (changeFlags & QPinchGesture::ScaleFactorChanged) {
      qDebug() << "pinchTriggered(): zoom by" << gesture->lastScaleFactor();
      zoom = 1.f / gesture->lastScaleFactor();
      resizeGL(width(), height());
  }
  if (gesture->state() == Qt::GestureFinished) {
    qDebug() << "pinchTriggered(): done";
  }
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
  {
    const DataCtrl::EMode CurrentMode = dataCtrl->currentMode();
    onMoveDecal = (event->type() == QEvent::MouseButtonPress && CurrentMode == DataCtrl::eModeView);
    lastMousePos = event->pos();

    if ((1<<CurrentMode) & ((1<<DataCtrl::eModeEdit) | (1<<DataCtrl::eModeDefineCentroid)))
    {
      const float factor = qMin(width(), height());
      float x(zoom);
      float y(x);
      if (width() > height())
        x *= ((float)width() / (float)height());
      else
        y *= ((float)height() / (float)width());
      QPointF Point((event->x() / factor * (zoom*2.f) - xDecal) - x,
                    (event->y() / factor * (zoom*2.f) - yDecal) - y);

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
  float factor = qMin(width(), height());
  if (onMoveDecal && event->type())
  {
    xDecal += (event->x() - lastMousePos.x()) / factor * (zoom*2.f);
    yDecal += (event->y() - lastMousePos.y()) / factor * (zoom*2.f);
  }
  lastMousePos = event->pos();

  if (!onMoveDecal && !(event->buttons() & Qt::LeftButton))
    QGLWidget::mouseMoveEvent(event);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
  zoom += event->delta() / 1200.;
  resizeGL(width(), height());

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
    default:
      break;
  }

  QGLWidget::keyPressEvent(event);
}

void ImageView::doChangeImage(const QImage &image)
{
  if (imageTexId) deleteTexture(imageTexId);
  ratioWidthPerHeght = (image.width() / (qreal)image.height());

  imageTexId = bindTexture(image);
}

void ImageView::doCloseImage()
{
  if (imageTexId) deleteTexture(imageTexId);
  ratioWidthPerHeght = 1.;

  imageTexId = 0;
}

void ImageView::initializeGL()
{
  QGLWidget::initializeGL();

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ImageView::paintGL()
{
  QGLWidget::paintGL();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glColor3f(1., 1., 1.);
  glTranslatef(xDecal, yDecal, 0);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, imageTexId);

  glEnable(GL_BLEND);

  glBegin(GL_QUADS);
    glTexCoord2f( 0, 1); glVertex3f(-ratioWidthPerHeght, -1., 0.);
    glTexCoord2f( 0, 0); glVertex3f(-ratioWidthPerHeght,  1., 0.);
    glTexCoord2f( 1, 0); glVertex3f( ratioWidthPerHeght,  1., 0.);
    glTexCoord2f( 1, 1); glVertex3f( ratioWidthPerHeght, -1., 0.);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  dataCtrl->draw();

  glDisable(GL_BLEND);

  glPopMatrix();
}

void ImageView::resizeGL(int width, int height)
{
  QGLWidget::resizeGL(width, height);

  glViewport(0, 0, width, height);

  float x(zoom);
  float y(x);

  if (width > height)
    x *= ((float)width / (float)height);
  else
    y *= ((float)height / (float)width);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-x, +x, +y, -y, -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}
