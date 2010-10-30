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

  for (int i = cells.count(); --i >= 0; )
    cells.at(i).draw();

  cell.draw();

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
  if (cell.addOneForm(points))
  {
    cells.push_back(cell);
    cell.clear();
  }
  points.clear();
}

void DataCtrl::removeLastForm()
{
  if (cell.clearOneForm() && !cells.isEmpty())
  {
    cell = cells.last();
    cells.pop_back();
  }
}
