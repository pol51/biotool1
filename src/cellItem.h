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
    CellItem() : _angle(0.f), _strength(0.f), _interval(0.f), _areaRatio(0.f) {}
    virtual ~CellItem() { }

    void clear();
    bool isEmpty() const;
    bool isFull() const;
    bool clearOneForm(); // return true when empty (all forms removeds)
    bool addOneForm(const Polygon &form); // return true if full (all form sets)
    void computeAreaRatio();
    void computeVector();
    virtual void draw(QGLShaderProgram *program, const qreal &averageAngle = 400.f, const qreal &averageCenroidRadius = 0.f) const;
    virtual void save(QDomDocument &doc, QDomElement &parentNode, const QString &name) const;
    virtual bool load(QDomElement &node);

    const qreal &getAngle()     const { return _angle; }
    const qreal &getStrength()  const { return _strength; }
    const qreal &getInterval()  const { return _interval; }
    const qreal &getAreaRatio() const { return _areaRatio; }

    static void drawArrow();

  protected:
    virtual const QColor &inColor()             const { return _insideColor; }
    virtual const QColor &outColor()            const { return _outsideColor; }
    virtual const QColor &vectorColor()         const { return _vectorColor; }
    virtual const QColor &averageVectorColor()  const { return _averageVectorColor; }
    virtual bool averageArrow()                 const { return _averageArrow; }

  protected:
    friend class Settings;
    friend class SettingsView;

    static bool _averageArrow;

    Polygon _insideForm;
    Polygon _outsideForm;

    static QColor _insideColor;
    static QColor _outsideColor;
    static QColor _vectorColor;
    static QColor _averageVectorColor;

    static qreal _arrowLength;
    static qreal _arrowHeadLength;
    static qreal _arrowHeadHalfWidth;
    static qreal _arrowScale;

    qreal _angle;
    qreal _strength;
    qreal _interval;
    qreal _areaRatio;
};

#endif
