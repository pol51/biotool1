#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtCore/QObject>

#include "cell.h"

class DataCtrl : public QObject
{
  Q_OBJECT

  public:
    DataCtrl(QObject *parent = NULL);
    virtual ~DataCtrl();

    void addPoint(const QPointF &point);

    void draw();

  public slots:
    void removeLastPoint();
    void finalizeForm();
    void removeLastForm();

  protected:
    Polygon points;
    Cell cell;
    QVector<Cell> cells;
};

#endif
