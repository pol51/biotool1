#include "cell.h"

#include <QtXml/QDomDocument>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

#include <math.h>

#include "dataCtrl.h"

Cell* Cell::_selected = nullptr;
Cell* Cell::_edited = nullptr;

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  // draw background
  if (isFull() && _interval > averageCenroidRadius && _showCellBackground)
  {
    qreal Angle2RefAngle(averageAngle - _angle);
    while (Angle2RefAngle < 0.) Angle2RefAngle += 360.;
    QColor BGColor(_colorInterpolator.getColorAt(Angle2RefAngle));
    BGColor.setAlpha(_bgAlpha);
    glColor4f(static_cast<GLfloat>(BGColor.redF()),
              static_cast<GLfloat>(BGColor.greenF()),
              static_cast<GLfloat>(BGColor.blueF()),
              static_cast<GLfloat>(BGColor.alphaF()));
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
  qreal sinsum(0.), cossum(0.);

  foreach (const VCil &VCilItem, _vcils)
  {
    const qreal angle = VCilItem.getAngle() * M_PI / 180.;
    sinsum += sin(angle);
    cossum += cos(angle);
  }

  _vcilsBeatingAngle = atan2(sinsum, cossum) * 180. / M_PI;
}

void Cell::computeVCilCircularStandardDeviation()
{
  qreal sinsum(0.), cossum(0.);

  foreach (const VCil &VCilItem, _vcils)
  {
    const qreal angle = VCilItem.getAngle() * M_PI / 180.;
    sinsum += sin(angle);
    cossum += cos(angle);
  }

  _vcilsCircularStandardDeviation = sqrt(-2 * log(sqrt(sinsum * sinsum +  cossum  * cossum) / _vcils.count())) * 180. / M_PI;
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
