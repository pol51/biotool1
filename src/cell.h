#ifndef __CELL_H__
#define __CELL_H__

#include <QtGui/QColor>

#include "polygon.h"

class QDomElement;
class QDomDocument;

class Cell
{
  public:
  Cell() : angle(0.) {}
    ~Cell() {}

    void clear();
    bool isEmpty() const ;
    bool isFull() const;
    bool clearOneForm(); // return true is empty (all forms removeds)
    bool addOneForm(const Polygon &form); // return true if full (all form sets)
    void computeVector();
    void draw() const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    const qreal &getAngle() const { return angle; }

  protected:
    Polygon insideForm;
    Polygon outsideForm;

    static QColor insideColor;
    static QColor outsideColor;

    qreal angle;
};

#endif
