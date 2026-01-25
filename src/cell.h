#pragma once

#include "cellItem.h"
#include "vCil.h"

class Cell: public CellItem
{
  public:
    Cell() : CellItem() {}
    Cell(const CellItem& cellItem) : CellItem(cellItem) {}
    virtual ~Cell() { if (this == _selected) _selected = nullptr; if (this == _edited) _edited = nullptr; }

    void draw(const qreal &averageAngle = 400., const qreal &averageCenroidRadius = 0.) const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    const qreal &getVCilBeatingAngle() const { return _vcilsBeatingAngle; }
    const qreal &getVCilCircularStandardDeviation() const { return _vcilsCircularStandardDeviation; }
    int getVCilCount() const { return _vcils.count(); }

    void computeVCilBeatingAngle();
    void computeVCilCircularStandardDeviation();

    void setSelected() { _selected = this; }
    static Cell* selected() { return _selected; }
    void setEdited() { _edited = this; }
    static Cell* edited() { return _edited; }
    static void stopEdition() { _edited = nullptr; }

    void addVCil(const VCil &vcil);
    void removeLastForm(CellItem &editedCell);

  protected:
    const QColor &inColor()                   { return (_selected==this) ? _insideSelectedColor  : _insideColor; }
    const QColor &outColor()                  { return (_selected==this) ? _outsideSelectedColor : _outsideColor; }
    static const QColor &vectorColor()        { return _vectorColor; }
    static const QColor &averageVectorColor() { return _averageVectorColor; }
    bool averageArrow() const override        { return _showAverageArrow; }
    bool showArrow() const override           { return _showArrow; }

  protected:
    friend class Settings;
    friend class SettingsView;

    static bool _showAverageArrow;
    static bool _showArrow;
    static bool _showCellBackground;

    static Cell* _selected;
    static Cell* _edited;

    static QColor _insideColor;
    static QColor _outsideColor;
    static QColor _insideSelectedColor;
    static QColor _outsideSelectedColor;
    static QColor _vectorColor;
    static QColor _averageVectorColor;

    static quint8 _bgAlpha;

    qreal _vcilsBeatingAngle = 0.;
    qreal _vcilsCircularStandardDeviation = 0.;
    QList<VCil> _vcils;
};
