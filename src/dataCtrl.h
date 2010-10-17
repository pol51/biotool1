#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtCore/QObject>

#include "polygon.h"

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
    QVector<Polygon> forms;
};

#endif
