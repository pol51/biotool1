#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

#include <QtCore/QFile>

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
  QDomDocument Doc("document");
  QDomElement Root = Doc.createElement("document");
  Doc.appendChild(Root);
  QDomElement Cells = Doc.createElement("cells");
  Root.appendChild(Cells);

  foreach(Cell _cell, cells) _cell.save(Doc, Cells);

  QString FileName(filename);
  if (!FileName.endsWith(".xml"))
    FileName.append(".xml");
  QFile File(FileName);
  if (File.open(QFile::WriteOnly))
  {
    File.write(Doc.toString(2).toUtf8());
    File.close();
  }

  saved = true;
}
