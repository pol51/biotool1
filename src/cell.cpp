#include "cell.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

Cell* Cell::_selected = NULL;
Cell* Cell::_edited = NULL;

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  CellItem::draw(averageAngle, averageCenroidRadius);

  foreach (const VCil &VCilItem, _vcils)
    VCilItem.draw(_vcilsAverageAngle);
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
    QDomElement VCilElement = node.firstChildElement("vcil");
    while (!VCilElement.isNull())
    {
      VCil LoadedVCil;
      if (LoadedVCil.load(VCilElement))
        _vcils.push_back(LoadedVCil);
      VCilElement = VCilElement.nextSiblingElement("vcil");
    }

    computeVCilAverageAngle();
    computeVCilCircularStandardDeviation();

    return true;
  }
  return false;
}

void Cell::computeVCilAverageAngle()
{
  qreal sinsum(0.f), cossum(0.f);

  foreach (const VCil &VCilItem, _vcils)
  {
    const qreal angle = VCilItem.getAngle() * M_PI / 180.f;
    sinsum += sin(angle);
    cossum += cos(angle);
  }

  _vcilsAverageAngle = atan2(sinsum, cossum) * 180.f / M_PI;
}

void Cell::computeVCilCircularStandardDeviation()
{
  qreal sinsum(0.f), cossum(0.f);

  foreach (const VCil &VCilItem, _vcils)
  {
    const qreal angle = VCilItem.getAngle() * M_PI / 180.f;
    sinsum += sin(angle);
    cossum += cos(angle);
  }

  _vcilsCircularStandardDeviation = sqrt(-2 * log(sqrt(sinsum * sinsum +  cossum  * cossum) / _vcils.count())) * 180.f / M_PI;
}

void Cell::addVCil(const VCil &vcil)
{
  _vcils.append(vcil);
  computeVCilAverageAngle();
  computeVCilCircularStandardDeviation();
}
