#ifndef __CELL_H__
#define __CELL_H__

#include "cellItem.h"

class Cell: public CellItem
{
  public:
    Cell() : CellItem() {}
    virtual ~Cell() { if (this == _selected) _selected = NULL; if (this == _edited) _edited = NULL; }

    void draw(const qreal &averageAngle = 400.f, const qreal &averageCenroidRadius = 0.f) const;
    void save(QDomDocument &doc, QDomElement &parentNode) const;
    bool load(QDomElement &node);

    void setSelected() { _selected = this; }
    static Cell* selected() { return _selected; }
    void setEdited() { _edited = this; }
    static Cell* edited() { return _edited; }
    static void stopEdition() { _edited = NULL; }

  protected:
    friend class Settings;
    friend class SettingsView;

    static bool averageArrow;

    static Cell* _selected;
    static Cell* _edited;

    static QColor insideColor;
    static QColor outsideColor;
    static QColor insideSelectedColor;
    static QColor outsideSelectedColor;
    static QColor vectorColor;
    static QColor averageVectorColor;
};

#endif
