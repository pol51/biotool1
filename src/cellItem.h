#ifndef __CELLITEM_H__
#define __CELLITEM_H__

#include <QtGui/QColor>

#include "polygon.h"

class QDomElement;
class QDomDocument;

class Settings;
class SettingsView;

class CellItem
{
  public:
    CellItem() : angle(0.f), strength(0.f), interval(0.f), areaRatio(0.f) {}
    virtual ~CellItem() { }

    void clear();
    bool isEmpty() const;
    bool isFull() const;
    bool clearOneForm(); // return true when empty (all forms removeds)
    bool addOneForm(const Polygon &form); // return true if full (all form sets)
    void computeAreaRatio();
    void computeVector();
    virtual void draw(const qreal &averageAngle, const qreal &averageCenroidRadius, const QColor &inColor, const QColor &outColor, const QColor &vectorColor, const bool averageArrow, const QColor &averageVectorColor) const;
    virtual void save(QDomDocument &doc, QDomElement &parentNode, const QString &name) const;
    virtual bool load(QDomElement &node);

    const qreal &getAngle()     const { return angle; }
    const qreal &getStrength()  const { return strength; }
    const qreal &getInterval()  const { return interval; }
    const qreal &getAreaRatio() const { return areaRatio; }

    static void drawArrow();

  protected:
    friend class Settings;
    friend class SettingsView;

    Polygon insideForm;
    Polygon outsideForm;

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
