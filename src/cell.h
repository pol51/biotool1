#ifndef __CELL_H__
#define __CELL_H__

#include "cellItem.h"
#include "vCil.h"

class Cell: public CellItem
{
  public:
    Cell() : CellItem(), _vcilsBeatingAngle(.0f), _vcilsCircularStandardDeviation(0.f) {}
    Cell(const CellItem& cellItem) : CellItem(cellItem), _vcilsBeatingAngle(.0f), _vcilsCircularStandardDeviation(0.f) {}
    virtual ~Cell() { if (this == _selected) _selected = NULL; if (this == _edited) _edited = NULL; }

    void draw(QGLShaderProgram *program, const qreal &averageAngle = 400.f, const qreal &averageCenroidRadius = 0.f) const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    const qreal &getVCilBeatingAngle() const { return _vcilsBeatingAngle; }
    const qreal &getVCilCircularStandardDeviation() const { return _vcilsCircularStandardDeviation; }

    void computeVCilBeatingAngle();
    void computeVCilCircularStandardDeviation();

    void setSelected() { _selected = this; }
    static Cell* selected() { return _selected; }
    void setEdited() { _edited = this; }
    static Cell* edited() { return _edited; }
    static void stopEdition() { _edited = NULL; }

    void addVCil(const VCil &vcil);
    void removeLastForm(CellItem &editedCell);

  protected:
    virtual const QColor &inColor()             const { return (_selected==this) ? _insideSelectedColor  : _insideColor; }
    virtual const QColor &outColor()            const { return (_selected==this) ? _outsideSelectedColor : _outsideColor; }
    virtual const QColor &vectorColor()         const { return _vectorColor; }
    virtual const QColor &averageVectorColor()  const { return _averageVectorColor; }
    virtual bool averageArrow()                 const { return _averageArrow; }

  protected:
    friend class Settings;
    friend class SettingsView;

    static bool _averageArrow;

    static Cell* _selected;
    static Cell* _edited;

    static QColor _insideColor;
    static QColor _outsideColor;
    static QColor _insideSelectedColor;
    static QColor _outsideSelectedColor;
    static QColor _vectorColor;
    static QColor _averageVectorColor;

    qreal _vcilsBeatingAngle;
    qreal _vcilsCircularStandardDeviation;
    QVector<VCil> _vcils;
};

#endif
