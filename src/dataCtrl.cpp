#include "dataCtrl.h"

#include <QtOpenGL/QGLContext>
#include <QtXml/QDomDocument>
#include <QtCore/QFile>

#include <cmath>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent), saved(true), cntMode(eModeView),
  averageAngle(0.), averageCenroidRadius(0.)
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

  switch (cntMode)
  {
    case eModeEdit:
    case eModeView:
      foreach (Cell CellItem, cells)
        CellItem.draw(averageAngle, averageCenroidRadius);
      cell.draw();
      break;
    case eModeDefineCentroid:
      glColor3f(centroidsRefColor.redF(), centroidsRefColor.greenF(), centroidsRefColor.blueF());
      foreach (CellPolygon CellItem, centroidsRef)
        CellItem.draw();
  }

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
  switch (cntMode)
  {
    case eModeEdit:
    case eModeView:
      if (cell.addOneForm(points))
      {
        cells.push_back(cell);
        cell.clear();
        refresh();
      }
      break;
    case eModeDefineCentroid:
      centroidsRef.push_back(points);
      cell.clear();
      refresh();
      break;
  }
  points.clear();
  saved = false;
}

void DataCtrl::removeLastForm()
{
  switch (cntMode)
  {
    case eModeEdit:
    case eModeView:
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
      if (cell.clearOneForm() && !cells.isEmpty())
        refresh();
      break;
    case eModeDefineCentroid:
      if (!centroidsRef.isEmpty())
      {
        centroidsRef.pop_back();
        refresh();
      }
      break;
  }

  saved = false;
}

void DataCtrl::clear()
{
  points.clear();
  cell.clear();
  cells.clear();
  centroidsRef.clear();
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

  QDomElement CentroidsRed = Doc.createElement("centroid_references");
  Root.appendChild(CentroidsRed);
  foreach(CellPolygon _cellPoly, centroidsRef) _cellPoly.save(Doc, CentroidsRed, 0);

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
    qreal interval  = _cell.getInterval();
    if (angle > 180.) angle -= 360.;
    CSV.append(QString("%1;%2\n").arg(QString::number(strength).arg(interval > averageCenroidRadius?QString::number(angle):"")));
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

  clear();

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
    if (Element.tagName() == "centroid_references")
    {
      QDomElement CellPolyElement = Element.firstChildElement("polygon");
      while (!CellPolyElement.isNull())
      {
        CellPolygon LoadedCellPoly;
        LoadedCellPoly.load(CellPolyElement);
        LoadedCellPoly.computeData();
        centroidsRef.push_back(LoadedCellPoly);
        CellPolyElement = CellPolyElement.nextSiblingElement("polygon");
      }
    }
    Element = Element.nextSiblingElement();
  }

  refresh();
  saved = true;
}

void DataCtrl::refresh()
{
  averageAngle = 0.f;
  averageCenroidRadius = 0.f;
  const int cellsCount = cells.count();
  if (!cellsCount)
  {
    emit angleChanged(0.f);
    emit countChanged(0, 0);
    return;
  }

  qreal sinsum(0.f), cossum(0.f);

  int ignored = 0;

  const int centroidsRefCount = centroidsRef.count();
  if (centroidsRefCount)
  {
    foreach (CellPolygon _poly, centroidsRef)
      averageCenroidRadius += _poly.getRadius();
    averageCenroidRadius /= centroidsRefCount;
  }

  foreach(Cell _cell, cells)
    if (_cell.getInterval() > averageCenroidRadius)
    {
      const qreal angle = _cell.getAngle() * M_PI / 180.f;
      sinsum += sin(angle);
      cossum += cos(angle);
    }
    else
      ++ignored;

  averageAngle = atan2(sinsum, cossum) * 180.f / M_PI;
  emit angleChanged(averageAngle);
  emit countChanged(ignored, cellsCount);
}

