#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>

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
    forms.at(i).draw();

  points.draw();
}

void DataCtrl::removeLastPoint()
{
  if (points.count()) points.pop_back();
}

void DataCtrl::finalizeForm()
{
  if (!points.count()) return;
  points.computeData();
  forms.push_back(points);
  points.clear();
}

void DataCtrl::removeLastForm()
{
  if (forms.count()) forms.pop_back();
}
