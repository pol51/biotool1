#pragma once

#include <QtGui/QColor>
#include <QtCore/QPropertyAnimation>
#include <QtCore/QLineF>
#include <QtCore/QPointF>

#include "xpolygon.h"

class QDomElement;
class QDomDocument;

class Settings;
class SettingsView;

class CellItem
{
  class ColorInterpolator
  {
    public:
      ColorInterpolator()
      {
        _p.setEasingCurve(QEasingCurve::Linear);
        _p.setDuration(360.);
        _p.setKeyValueAt(.0,    QColor(Qt::green));
        _p.setKeyValueAt(1./6., QColor(Qt::yellow));
        _p.setKeyValueAt(2./6., QColor(Qt::red));
        _p.setKeyValueAt(.5,    QColor(Qt::magenta));
        _p.setKeyValueAt(4./6., QColor(Qt::blue));
        _p.setKeyValueAt(5./6., QColor(Qt::cyan));
        _p.setKeyValueAt(1.,    QColor(Qt::green));
      }

      const QColor getColorAt(const qreal &angle)
      {
        _p.setCurrentTime(static_cast<int>(angle));
        return _p.currentValue().value<QColor>();
      }

    protected:
      QPropertyAnimation _p;
  };

  public:
    virtual ~CellItem() {}

    void clear();
    bool isEmpty() const;
    bool isFull() const;
    bool clearOneForm(); // return true when empty (all forms removeds)
    bool addOneForm(const XPolygon &form); // return true if full (all form sets)
    void computeAreaRatio();
    void computeVector();
    void draw(const qreal &averageAngle = 400., const qreal &averageCenroidRadius = 0.) const;
    void save(QDomDocument &doc, QDomElement &parentNode, const QString &name) const;
    bool load(QDomElement &node);

    const qreal &getAngle()     const { return _angle; }
    const qreal &getStrength()  const { return _strength; }
    const qreal &getInterval()  const { return _interval; }
    const qreal &getAreaRatio() const { return _areaRatio; }
    const qreal &cellArea()     const { return _outsideForm.getArea(); }
    const qreal &patchArea()    const { return _insideForm.getArea(); }

    static void drawArrow();

  protected:
    static const QColor &inColor()            { return _insideColor; }
    static const QColor &outColor()           { return _outsideColor; }
    static const QColor &vectorColor()        { return _vectorColor; }
    static const QColor &averageVectorColor() { return _averageVectorColor; }
    virtual bool averageArrow() const         { return _showAverageArrow; }
    virtual bool showArrow() const            { return _showArrow; }

  protected:
    friend class Settings;
    friend class SettingsView;

    static bool _showAverageArrow;
    static bool _showArrow;

    XPolygon _insideForm;
    XPolygon _outsideForm;

    static QColor _insideColor;
    static QColor _outsideColor;
    static QColor _vectorColor;
    static QColor _averageVectorColor;

    static qreal _arrowLength;
    static qreal _arrowHeadLength;
    static qreal _arrowHeadHalfWidth;
    static qreal _arrowScale;

    qreal _angle = 0.;
    qreal _strength = 0.;
    qreal _interval = 0.;
    qreal _areaRatio = 0.;

    static ColorInterpolator _colorInterpolator;
};
