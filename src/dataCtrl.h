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

    void draw() const;

    bool isSaved() const { return saved; }

  public slots:
    void removeLastPoint();
    void finalizeForm();
    void removeLastForm();

    void clear();

    void save(const QString &filename);
    void load(const QString &filename);

  signals:
    void countChanged(int);

  protected:
    bool saved;
    Polygon points;
    Cell cell;
    QVector<Cell> cells;
};

#endif
