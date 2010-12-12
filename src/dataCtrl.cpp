#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>
#include <QtXml/QDomDocument>
#include <QtCore/QFile>

#include <cmath>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent), saved(true), averageAngle(0.)
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
    cells.at(i).draw(averageAngle, minimalStrength);

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
    refresh();
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
      refresh();
      saved = false;
    }
    return;
  }
  if (cell.clearOneForm() && points.isEmpty() && !cells.isEmpty())
  {
    cell = cells.last();
    cells.pop_back();
    refresh();
  }
  saved = false;
}

void DataCtrl::clear()
{
  points.clear();
  cell.clear();
  cells.clear();
  refresh();
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

void DataCtrl::exportCsv(const QString &filename)
{
  QByteArray CSV;

  foreach(Cell _cell, cells)
  {
    qreal angle     = _cell.getAngle() - averageAngle;
    qreal strength  = _cell.getStrength();
    if (angle > 180.) angle -= 360.;
    CSV.append(QString("%1;%2\n").arg(strength > minimalStrength?QString::number(strength):"").arg(angle));
  }

  QString FileName(filename);
  if (!FileName.endsWith(".csv"))
    FileName.append(".csv");
  QFile File(FileName);
  if (File.open(QFile::WriteOnly))
  {
    File.write(CSV);
    File.close();
  }
}

void DataCtrl::setMinimalStrength(const qreal &minimalStrength)
{
  this->minimalStrength = minimalStrength;
  emit refresh();
}

void DataCtrl::load(const QString &filename)
{
  QDomDocument Doc("document");
  QFile File(filename);
  if (!File.open(QIODevice::ReadOnly))
    return;

  if (!Doc.setContent(&File))
  {
    File.close();
    return;
  }
  File.close();

  QDomElement DocElem = Doc.documentElement();

  QDomElement Element = DocElem.firstChildElement();
  while (!Element.isNull())
  {
    if (Element.tagName() == "cells")
    {
      QDomElement CellElement = Element.firstChildElement("cell");
      while (!CellElement.isNull())
      {
        Cell LoadedCell;
        if (LoadedCell.load(CellElement))
          cells.push_back(LoadedCell);
        CellElement = CellElement.nextSiblingElement("cell");
      }
    }
    Element = Element.nextSiblingElement();
  }

  refresh();
  saved = true;
}

void DataCtrl::refresh()
{
  averageAngle = 0;
  const int cellsCount = cells.count();
  if (!cellsCount)
  {
    emit angleChanged(0.);
    emit countChanged(0, 0);
    return;
  }

  qreal sinsum(0.), cossum(0.);

  int ignored = 0;

  foreach(Cell _cell, cells)
    if (_cell.getStrength() > minimalStrength)
    {
      const qreal angle = _cell.getAngle() * M_PI / 180.;
      sinsum += sin(angle);
      cossum += cos(angle);
    }
    else
      ++ignored;

  averageAngle = atan2(sinsum, cossum) * 180. / M_PI;
  emit angleChanged(averageAngle);
  emit countChanged(ignored, cellsCount);
}

