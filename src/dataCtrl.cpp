#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>
#include <QtXml/QDomDocument>
#include <QtCore/QFile>

#include <cmath>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent), saved(true), averageAngle(0.)
{
  connect(this, SIGNAL(countChanged(int)), this, SLOT(onCountChanged()));
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
    cells.at(i).draw(averageAngle);

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
    emit countChanged(cells.count());
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
      emit countChanged(cells.count());
      saved = false;
    }
    return;
  }
  if (cell.clearOneForm() && points.isEmpty() && !cells.isEmpty())
  {
    cell = cells.last();
    cells.pop_back();
    emit countChanged(cells.count());
  }
  saved = false;
}

void DataCtrl::clear()
{
  points.clear();
  cell.clear();
  cells.clear();
  emit countChanged(0);
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
  foreach(Cell _cell, cells) CSV.append(QString("%1;%2\n").arg(_cell.getStrength()).arg(_cell.getAngle()));

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

  emit countChanged(cells.count());
  saved = true;
}

void DataCtrl::onCountChanged()
{
  averageAngle = 0;
  if (!cells.count()) return;
  qreal sinsum(0.), cossum(0.);
  foreach(Cell _cell, cells)
  {
    const qreal angle = _cell.getAngle() * M_PI / 180.;
    sinsum += sin(angle);
    cossum += cos(angle);
  }
  averageAngle = atan2(sinsum, cossum) * 180. / M_PI;
  emit angleChanged(averageAngle);
}

