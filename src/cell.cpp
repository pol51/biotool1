#include "cell.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

const Cell* Cell::_selected = NULL;

void Cell::clear()
{
  insideForm.clear();
  outsideForm.clear();
}

bool Cell::isEmpty() const
{
  return insideForm.isEmpty() && outsideForm.isEmpty();
}

bool Cell::isFull() const
{
  return !(insideForm.isEmpty() || outsideForm.isEmpty());
}

bool Cell::clearOneForm()
{
  if (!insideForm.isEmpty())
  {
    insideForm.clear();
    return false;
  }
  outsideForm.clear();
  return true;
}

bool Cell::addOneForm(const CellPolygon &form)
{
  if (outsideForm.isEmpty())
  {
    outsideForm = form;
    return false;
  }
  insideForm = form;
  computeAreaRatio();
  computeVector();
  return true;
}

void Cell::computeAreaRatio()
{
  if (outsideForm.count() <= 1 ) return;

  areaRatio = insideForm.getArea() / outsideForm.getArea();
}

void Cell::computeVector()
{
  if (outsideForm.count() <= 1 ) return;

  // compute angle
  QLineF line(outsideForm.getCentroid(), insideForm.getCentroid());
  angle = line.angle();

  // compute lengths
  interval = line.length();
  qreal centroidToOutsideLength(0.);

  line.setLength(10000);
  QPointF intersection;
  QLineF line2;
  QLineF lineToOutside;
  lineToOutside.setP1(outsideForm.getCentroid());
  outsideForm.push_back(outsideForm.first());
  for (int i = outsideForm.count(); --i > 0; )
  {
    line2.setPoints(outsideForm[i], outsideForm[i-1]);
    if (line.intersect(line2, &intersection) == QLineF::BoundedIntersection)
    {
      lineToOutside.setP2(intersection);
      if (lineToOutside.length() > centroidToOutsideLength)
        centroidToOutsideLength = lineToOutside.length();
    }
  }
  outsideForm.pop_back();

  strength = interval / centroidToOutsideLength;
}

void Cell::draw(const qreal &averageAngle, const qreal &averageCenroidRadius) const
{
  QColor & inColor(_selected==this ?  insideSelectedColor :  insideColor);
  QColor &outColor(_selected==this ? outsideSelectedColor : outsideColor);

  // draw outside form
  glColor3f(outColor.redF(), outColor.greenF(), outColor.blueF());
  outsideForm.draw();

  // draw inside form
  glColor3f(inColor.redF(), inColor.greenF(), inColor.blueF());
  insideForm.draw();

  // draw vector
  if (isFull() && outsideForm.count() > 1 && interval > averageCenroidRadius)
  {
    glPushMatrix();
    glColor3f(vectorColor.redF(), vectorColor.greenF(), vectorColor.blueF());
    glTranslatef(outsideForm.getCentroid().x(), outsideForm.getCentroid().y(), 0.);
    glRotatef(angle, 0., 0., -1.);
    //glScalef(strength * 2.5, strength * 2.5, 1.);
    glScalef(arrowScale, arrowScale, 1.);
    drawArrow();

    if (averageArrow && averageAngle <= 360.)
    {
      glColor3f(averageVectorColor.redF(), averageVectorColor.greenF(), averageVectorColor.blueF());
      glRotatef(averageAngle - angle, 0., 0., -1.);
      drawArrow();
    }

    glPopMatrix();
  }
}

void Cell::save(QDomDocument &doc, QDomElement &parentNode) const
{
  QDomElement CellNode = doc.createElement("cell");
  parentNode.appendChild(CellNode);

  insideForm.save(doc, CellNode, 0);
  outsideForm.save(doc, CellNode, 1);
}

bool Cell::load(QDomElement &node)
{
  // load data
  QDomElement FormElement = node.firstChildElement("polygon");
  while (!FormElement.isNull())
  {
    bool Ok = false;
    int Level = FormElement.attribute("level", "-1").toInt(&Ok, 10);
    if (Ok && Level >= 0)
    {
      if (Level == 0) insideForm.load(FormElement);
      if (Level == 1) outsideForm.load(FormElement);
    }

    FormElement = FormElement.nextSiblingElement("polygon");
  }

  // (re-)compute other data if loaded correctly
  if (isFull())
  {
    insideForm.computeData();
    outsideForm.computeData();
    computeVector();
    computeAreaRatio();
    return true;
  }

  return false;
}

void Cell::drawArrow()
{
  qreal arrowHeadBase(arrowLength - arrowHeadLength);

  glBegin(GL_LINES);
    glVertex3f(arrowLength, 0., 0.);
    glVertex3f(0., 0., 0.);
    glVertex3f(arrowLength, 0., 0.);
    glVertex3f(arrowHeadBase, arrowHeadHalfWidth, 0.);
    glVertex3f(arrowLength, 0., 0.);
    glVertex3f(arrowHeadBase, -arrowHeadHalfWidth, 0.);
  glEnd();
}
