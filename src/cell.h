#ifndef __CELL_H__
#define __CELL_H__

#include <QtGui/QColor>

#include "cellPolygon.h"

class QDomElement;
class QDomDocument;

class Settings;

class Cell
{
  public:
    Cell() : angle(0.), strength(0.) {}
    ~Cell() {}

    void clear();
    bool isEmpty() const ;
    bool isFull() const;
    bool clearOneForm(); // return true is empty (all forms removeds)
    bool addOneForm(const CellPolygon &form); // return true if full (all form sets)
    void computeVector();
    void draw(const qreal &averageAngle = 400., const qreal &minimalStrength = 0.) const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    const qreal &getAngle() const { return angle; }
    const qreal &getStrength() const { return strength; }

    static void drawArrow();

  protected:
    friend class Settings;

    CellPolygon insideForm;
    CellPolygon outsideForm;

    static QColor insideColor;
    static QColor outsideColor;
    static QColor vectorColor;
    static QColor averageVectorColor;

    static qreal arrowLength;
    static qreal arrowHeadLength;
    static qreal arrowHeadHalfWidth;
    static qreal arrowScale;

    qreal angle;
    qreal strength;
};

#endif
