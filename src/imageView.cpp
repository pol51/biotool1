#include "imageView.h"

#include <QtGui/QMouseEvent>
#include <QOpenGLContext>

ImageView::ImageView(QWidget *parent) :
  QOpenGLWidget(parent),
  dataCtrl(new DataCtrl(this))
{
  grabGesture(Qt::PinchGesture);
  connect(&refreshTimer, SIGNAL(timeout()), this, SLOT(update()));
  refreshTimer.start(20);
}

ImageView::~ImageView()
{
  if (_image) delete _image;
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
  return QOpenGLWidget::event(event);
}

bool ImageView::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    return true;
}

void ImageView::pinchTriggered(QPinchGesture *gesture)
{
  QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
  if (changeFlags & QPinchGesture::ScaleFactorChanged)
  {
      zoom = 1.f / gesture->lastScaleFactor();
      resizeGL(width(), height());
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

  QOpenGLWidget::mousePressEvent(event);
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
  if (event->button() == Qt::LeftButton)
    onMoveDecal = false;

  QOpenGLWidget::mousePressEvent(event);
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
    QOpenGLWidget::mouseMoveEvent(event);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
  if (event->source() == Qt::MouseEventNotSynthesized)
  {
    zoom += event->delta() / 1200.;
    resizeGL(width(), height());
  }

  QOpenGLWidget::wheelEvent(event);
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

  QOpenGLWidget::keyPressEvent(event);
}

void ImageView::doChangeImage(const QImage &image)
{
  if (_image) delete _image;
  ratioWidthPerHeght = (image.width() / (qreal)image.height());

  _image = new QOpenGLTexture(image);
}

void ImageView::doCloseImage()
{
  if (_image) delete _image;
  _image = nullptr;
  ratioWidthPerHeght = 1.;
}

void ImageView::initializeGL()
{
  QOpenGLWidget::initializeGL();

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ImageView::paintGL()
{
  int w = width();
  int h = height();
  glViewport(0, 0, w, h);

  float x(zoom);
  float y(x);

  if (w > h)
    x *= ((float)w / (float)h);
  else
    y *= ((float)h / (float)w);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-x, +x, +y, -y, -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);

  QOpenGLWidget::paintGL();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glColor3f(1., 1., 1.);
  glTranslatef(xDecal, yDecal, 0);

  if (_image)
  {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _image->textureId());
  }

  glEnable(GL_BLEND);

  glBegin(GL_QUADS);
    glTexCoord2f( 0, 1); glVertex3f(-ratioWidthPerHeght,  1., 0.);
    glTexCoord2f( 0, 0); glVertex3f(-ratioWidthPerHeght, -1., 0.);
    glTexCoord2f( 1, 0); glVertex3f( ratioWidthPerHeght, -1., 0.);
    glTexCoord2f( 1, 1); glVertex3f( ratioWidthPerHeght,  1., 0.);
  glEnd();

  glDisable(GL_TEXTURE_2D);

  dataCtrl->draw();

  glDisable(GL_BLEND);

  glPopMatrix();
}

void ImageView::resizeGL(int w, int h)
{
  QOpenGLWidget::resizeGL(w, h);

  glViewport(0, 0, w, h);

  if (zoom < .05f)
      zoom = .05f;

  float x(zoom);
  float y(x);

  if (w > h)
    x *= ((float)w / (float)h);
  else
    y *= ((float)h / (float)w);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-x, +x, +y, -y, -100.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}
