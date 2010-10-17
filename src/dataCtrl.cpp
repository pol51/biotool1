#include "dataCtrl.h"

#include <QDebug>

#include <QtOpenGL/QGLContext>

#include <cmath>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent)
{
}

DataCtrl::~DataCtrl()
{
}

void DataCtrl::addPoint(const QPointF &point)
{
  points.append(point);
}

void DataCtrl::draw()
{
  glLineWidth(3.);
  glPointSize(5.);

  for (int i = forms.count(); --i >= 0; )
  {
    glColor3f(i&1?1.:0.1, i&1?0.1:1., 0.1);
    QVector<QPointF> &Points(forms[i]);
    const int PointsCount = Points.count();
    if (PointsCount > 1)
    {
      glBegin(GL_LINE_LOOP);
        for (int j = PointsCount; --j >= 0; )
          glVertex3f(Points[j].x(), Points[j].y(), 0.);
      glEnd();
    }
    else
      glBegin(GL_POINTS);
        glVertex3f(Points[0].x(), Points[0].y(), 0.);
      glEnd();
  }

  glColor3f(.75, .75, .75);
  for (int i = points.count(); --i >= 0; )
  {
    glBegin(GL_POINTS);
      glVertex3f(points[i].x(), points[i].y(), 0.);
    glEnd();
  }
}

void DataCtrl::removeLastPoint()
{
  if (points.count()) points.pop_back();
}

void DataCtrl::finalizeForm()
{
  if (!points.count()) return;
  forms.push_back(points);

  if (points.count() >= 2)
  {
    QPointF Centroid;
    qreal signedArea = 0.0;
    qreal x0 = 0.0; // Current vertex X
    qreal y0 = 0.0; // Current vertex Y
    qreal x1 = 0.0; // Next vertex X
    qreal y1 = 0.0; // Next vertex Y
    qreal a = 0.0;  // Partial signed area

    points.push_back(points[0]);

    // For all vertices except last
    int i=0;
    for (i=0; i < points.count()-1; ++i)
    {
      x0 = points[i].x();
      y0 = points[i].y();
      x1 = points[i+1].x();
      y1 = points[i+1].y();
      a = x0*y1 - x1*y0;
      signedArea += a;
      Centroid.setX(Centroid.x() + (x0 + x1)*a);
      Centroid.setY(Centroid.y() + (y0 + y1)*a);
    }

    signedArea *= 0.5;
    Centroid.setX(Centroid.x() / (6*signedArea));
    Centroid.setY(Centroid.y() / (6*signedArea));

    QVector<QPointF> Centroid_; Centroid_.push_back(Centroid);
    forms.push_back(Centroid_);
  }

  points.clear();
}

void DataCtrl::removeLastForm()
{
  if (forms.count()) forms.pop_back();
}
