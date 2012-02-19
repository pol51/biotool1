#ifndef __CELL_H__
#define __CELL_H__

#include <QtGui/QColor>

#include "cellPolygon.h"

class QDomElement;
class QDomDocument;

class Settings;
class SettingsView;

class Cell
{
  public:
    Cell() : angle(0.f), strength(0.f), interval(0.f), areaRatio(0.f) {}
    ~Cell() {}

    void clear();
    bool isEmpty() const ;
    bool isFull() const;
    bool clearOneForm(); // return true is empty (all forms removeds)
    bool addOneForm(const CellPolygon &form); // return true if full (all form sets)
    void computeAreaRatio();
    void computeVector();
    void draw(const qreal &averageAngle = 400.f, const qreal &averageCenroidRadius = 0.f) const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    const qreal &getAngle() const { return angle; }
    const qreal &getStrength() const { return strength; }
    const qreal &getInterval() const { return interval; }
    const qreal &getAreaRatio() const { return areaRatio; }

    static void drawArrow();

  protected:
    friend class Settings;
    friend class SettingsView;

    CellPolygon insideForm;
    CellPolygon outsideForm;

    static bool averageArrow;

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
    qreal interval;
    qreal areaRatio;
};

#endif
