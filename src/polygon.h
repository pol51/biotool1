#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <QtGui/QPolygonF>

class Polygon : public QPolygonF
{
  public:
    enum EType
    {
      eEdition = 0,
      eFinalized
    };

  public:
    Polygon() : QPolygonF(), area(0.), type(eEdition) {}
    Polygon(const Polygon &other) : QPolygonF() { *this = other; }
    virtual ~Polygon() {}

    void computeData();
    virtual void clear();

    const QPointF &getCentroid() const { return centroid; }
    const qreal &getArea() const { return area; }

    void draw() const;

  protected:
    QPointF centroid;
    qreal area;
    EType type;
};

#endif