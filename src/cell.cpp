#include "cell.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

#include <math.h>

#include "dataCtrl.h"

Cell* Cell::_selected = NULL;
Cell* Cell::_edited = NULL;

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  // draw background
  if (isFull() && _interval > averageCenroidRadius && _cellBackground)
  {
    qreal Angle2RefAngle(averageAngle - _angle);
    while (Angle2RefAngle < 0.f) Angle2RefAngle += 360.f;
    QColor BGColor(_colorInterpolator.getColorAt(Angle2RefAngle));
    BGColor.setAlpha(_bgAlpha);
    glColor4f(BGColor.redF(), BGColor.greenF(), BGColor.blueF(), BGColor.alphaF());
    _outsideForm.drawBackground();
  }

  CellItem::draw(averageAngle, averageCenroidRadius);

  if (_edited == this || _vcilsCircularStandardDeviation < DataCtrl::maxCSD())
    foreach (const VCil &VCilItem, _vcils)
      VCilItem.draw(_vcilsBeatingAngle);
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

    computeVCilBeatingAngle();
    computeVCilCircularStandardDeviation();

    return true;
  }
  return false;
}

void Cell::computeVCilBeatingAngle()
{
  qreal sinsum(0.f), cossum(0.f);

  foreach (const VCil &VCilItem, _vcils)
  {
    const qreal angle = VCilItem.getAngle() * M_PI / 180.f;
    sinsum += sin(angle);
    cossum += cos(angle);
  }

  _vcilsBeatingAngle = atan2(sinsum, cossum) * 180.f / M_PI;
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
  computeVCilBeatingAngle();
  computeVCilCircularStandardDeviation();
}

void Cell::removeLastForm(CellItem &editedCell)
{
  if (!_vcils.isEmpty())
  {
    editedCell = _vcils.last();
    _vcils.pop_back();
    editedCell.clearOneForm();
  }
}
