#include "cellItem.h"

#include <QtXml/QDomDocument>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

CellItem::ColorInterpolator CellItem::_colorInterpolator;

void CellItem::clear()
{
  _insideForm.clear();
  _outsideForm.clear();
}

bool CellItem::isEmpty() const
{
  return _insideForm.isEmpty() && _outsideForm.isEmpty();
}

bool CellItem::isFull() const
{
  return !(_insideForm.isEmpty() || _outsideForm.isEmpty());
}

bool CellItem::clearOneForm()
{
  if (!_insideForm.isEmpty())
  {
    _insideForm.clear();
    return false;
  }
  _outsideForm.clear();
  return true;
}

bool CellItem::addOneForm(const XPolygon &form)
{
  if (_outsideForm.isEmpty())
  {
    _outsideForm = form;
    return false;
  }
  _insideForm = form;
  computeAreaRatio();
  computeVector();
  return true;
}

void CellItem::computeAreaRatio()
{
  if (_outsideForm.count() <= 1 ) return;

  _areaRatio = _insideForm.getArea() / _outsideForm.getArea();
}

void CellItem::computeVector()
{
  if (_outsideForm.count() < 1 ) return;

  // compute angle
  QLineF line(_outsideForm.getCentroid(), _insideForm.getCentroid());
  _angle = line.angle();

  // compute lengths
  _interval = line.length();
  qreal centroidToOutsideLength(0.);

  line.setLength(10000);
  QPointF intersection;
  QLineF line2;
  QLineF lineToOutside;
  lineToOutside.setP1(_outsideForm.getCentroid());
  _outsideForm.push_back(_outsideForm.first());
  for (int i = _outsideForm.count(); --i > 0; )
  {
    line2.setPoints(_outsideForm[i], _outsideForm[i-1]);
    if (line.intersects(line2, &intersection) == QLineF::BoundedIntersection)
    {
      lineToOutside.setP2(intersection);
      if (lineToOutside.length() > centroidToOutsideLength)
        centroidToOutsideLength = lineToOutside.length();
    }
  }
  _outsideForm.pop_back();

  // compute strength
  _strength = _interval / centroidToOutsideLength;
}

void CellItem::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  const QColor &InColor(inColor());
  const QColor &OutColor(outColor());
  const QColor &VectorColor(vectorColor());
  const QColor &AverageVectorColor(averageVectorColor());

  // draw outside form
  glColor3f(static_cast<GLfloat>(OutColor.redF()),
            static_cast<GLfloat>(OutColor.greenF()),
            static_cast<GLfloat>(OutColor.blueF()));
  _outsideForm.draw();

  // draw inside form
  glColor3f(static_cast<GLfloat>(InColor.redF()),
            static_cast<GLfloat>(InColor.greenF()),
            static_cast<GLfloat>(InColor.blueF()));
  _insideForm.draw();

  // draw vector
  if (isFull() && _interval > averageCenroidRadius)
  {
    glPushMatrix();

    glTranslatef(static_cast<GLfloat>(_outsideForm.getCentroid().x()),
                 static_cast<GLfloat>(_outsideForm.getCentroid().y()), 0.f);
    glRotatef(static_cast<GLfloat>(_angle), 0.f, 0.f, -1.f);
    glScalef(static_cast<GLfloat>(_arrowScale), static_cast<GLfloat>(_arrowScale), 1.f);

    if (showArrow())
    {
      glColor3f(static_cast<GLfloat>(VectorColor.redF()),
                static_cast<GLfloat>(VectorColor.greenF()),
                static_cast<GLfloat>(VectorColor.blueF()));
      drawArrow();
    }

    if (averageArrow() && averageAngle <= 360.)
    {
      glColor3f(static_cast<GLfloat>(AverageVectorColor.redF()),
                static_cast<GLfloat>(AverageVectorColor.greenF()),
                static_cast<GLfloat>(AverageVectorColor.blueF()));
      glRotatef(static_cast<GLfloat>(averageAngle - _angle), 0.f, 0.f, -1.f);
      drawArrow();
    }

    glPopMatrix();
  }
}

void CellItem::save(QDomDocument &doc, QDomElement &parentNode, const QString &name) const
{
  QDomElement CellNode = doc.createElement(name);
  parentNode.appendChild(CellNode);

  _insideForm.save(doc, CellNode, 0);
  _outsideForm.save(doc, CellNode, 1);
}

bool CellItem::load(QDomElement &node)
{
  // load data
  QDomElement FormElement = node.firstChildElement("polygon");
  while (!FormElement.isNull())
  {
    bool Ok = false;
    int Level = FormElement.attribute("level", "-1").toInt(&Ok, 10);
    if (Ok && Level >= 0)
    {
      if (Level == 0) _insideForm.load(FormElement);
      if (Level == 1) _outsideForm.load(FormElement);
    }

    FormElement = FormElement.nextSiblingElement("polygon");
  }

  // (re-)compute other data if loaded correctly
  if (isFull())
  {
    _insideForm.computeData();
    _outsideForm.computeData();
    computeVector();
    computeAreaRatio();
    return true;
  }

  return false;
}

void CellItem::drawArrow()
{
  qreal arrowHeadBase(_arrowLength - _arrowHeadLength);

  glBegin(GL_LINES);
    glVertex3f(static_cast<GLfloat>(_arrowLength), 0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(static_cast<GLfloat>(_arrowLength), 0.f, 0.f);
    glVertex3f(static_cast<GLfloat>(arrowHeadBase), static_cast<GLfloat>(_arrowHeadHalfWidth), 0.f);
    glVertex3f(static_cast<GLfloat>(_arrowLength), 0.f, 0.f);
    glVertex3f(static_cast<GLfloat>(arrowHeadBase), static_cast<GLfloat>(-_arrowHeadHalfWidth), 0.f);
  glEnd();
}
