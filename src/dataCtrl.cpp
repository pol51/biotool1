#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent), saved(true)
{
}

DataCtrl::~DataCtrl()
{
}

void DataCtrl::addPoint(const QPointF &point)
{
  points.append(point);
  saved = false;
}

void DataCtrl::draw() const
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
  if (points.count())
  {
    points.pop_back();
    saved = false;
  }
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
  saved = false;
}

void DataCtrl::removeLastForm()
{
  if (cell.isEmpty())
  {
    if (!cells.isEmpty())
    {
      cell = cells.last();
      cells.pop_back();
      cell.clearOneForm();
      saved = false;
    }
    return;
  }
  if (cell.clearOneForm() && points.isEmpty() && !cells.isEmpty())
  {
    cell = cells.last();
    cells.pop_back();
  }
  saved = false;
}

void DataCtrl::clear()
{
  points.clear();
  cell.clear();
  cells.clear();
  saved = true;
}

void DataCtrl::load(const QString &filename)
{
  Q_UNUSED(filename);
  saved = true;
}

void DataCtrl::save(const QString &filename)
{
  Q_UNUSED(filename);
  saved = true;
}
