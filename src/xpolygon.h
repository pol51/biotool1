#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <QtGui/QPolygonF>

#define M_PI  3.14159265358979323846

class QDomElement;
class QDomDocument;

class XPolygon : public QPolygonF
{
  public:
    enum EType
    {
      eEdition = 0,
      eFinalized
    };

  public:
    XPolygon() : QPolygonF(), area(0.f), radius(0.f), type(eEdition) {}
    XPolygon(const XPolygon &other) : QPolygonF() { *this = other; }

    void computeData();
    virtual void clear();

    const QPointF &getCentroid()  const { return centroid; }
    const qreal   &getArea()      const { return area; }
    const qreal   &getRadius()    const { return radius; }

    void draw() const;

    void save(QDomDocument &doc, QDomElement &parentNode, const int level) const;
    void load(QDomElement &node);

  protected:
    QPointF centroid;
    qreal area;
    qreal radius;
    EType type;
};

#endif
