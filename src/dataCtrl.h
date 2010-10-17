#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QVector>

class DataCtrl : public QObject
{
  Q_OBJECT

  public:
    DataCtrl(QObject *parent = NULL);
    virtual ~DataCtrl();

    void addPoint(const QPointF &point);

    const QVector<QPointF> &getPoints() const { return points; }

    void draw();

  public slots:
    void removeLastPoint();
    void finalizeForm();
    void removeLastForm();

  protected:
    QVector<QPointF> points;
    QVector<QVector<QPointF> > forms;
};

#endif
