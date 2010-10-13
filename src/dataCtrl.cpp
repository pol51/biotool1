#include "dataCtrl.h"

#include <QDebug>

DataCtrl::DataCtrl(QObject *parent):
  QObject(parent)
{
}

DataCtrl::~DataCtrl()
{
}

void DataCtrl::addPoint(const QPointF &point)
{
  qDebug() << point << "added";

  points.append(point);
}

void DataCtrl::removeLastPoint()
{
  if (points.count()) points.pop_back();
}
