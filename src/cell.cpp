#include "cell.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

Cell* Cell::_selected = NULL;
Cell* Cell::_edited = NULL;

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  CellItem::draw(averageAngle, averageCenroidRadius);

  foreach (const VCil &VCilItem, _vcils)
    VCilItem.draw();
}

void Cell::save(QDomDocument &doc, QDomElement &parentNode) const
{
  QDomElement CellNode = doc.createElement("cell");
  parentNode.appendChild(CellNode);

  _insideForm.save(doc, CellNode, 0);
  _outsideForm.save(doc, CellNode, 1);

  foreach (const VCil &VCilItem, _vcils)
    VCilItem.save(doc, CellNode, "vcil");
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

void Cell::addVCil(const VCil &vcil)
{
  _vcils.append(vcil);
}
