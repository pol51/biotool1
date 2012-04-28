#include "cell.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

Cell* Cell::_selected = NULL;
Cell* Cell::_edited = NULL;

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  QColor & inColor(_selected==this ?  insideSelectedColor :  insideColor);
  QColor &outColor(_selected==this ? outsideSelectedColor : outsideColor);

  CellItem::draw(averageAngle, averageCenroidRadius, inColor, outColor, vectorColor, averageArrow, averageVectorColor);

  //TODO: draw vcils
}

void Cell::save(QDomDocument &doc, QDomElement &parentNode) const
{
  CellItem::save(doc, parentNode, "cell");

  //TODO: save vcils
}

bool Cell::load(QDomElement &node)
{
  if (CellItem::load(node))
  {
    return true;

    //TODO: load vcils
  }
  return false;
}
