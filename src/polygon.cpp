#include "polygon.h"

#include <QtOpenGL/QGLContext>
#include <QtOpenGL/QGLShaderProgram>

#include <QtXml/QDomDocument>

#include <cmath>

void Polygon::computeData()
{
  const int Count = count();

  switch (Count)
  {
    case 0: return;
    case 1:
      centroid = at(0);
      area = 0.f;
      radius = 0.f;
      break;
    case 2:
      centroid.setX((at(0).x() + at(1).x()) * .5f);
      centroid.setY((at(0).y() + at(1).y()) * .5f);
      area = 0.f;
      radius = QLineF(at(0), at(1)).length() * .5f;
      break;
    default:
      {
        centroid.setX(0.f);
        centroid.setY(0.f);
        area = 0.f;

        qreal x0 = 0.f; // Current vertex X
        qreal y0 = 0.f; // Current vertex Y
        qreal x1 = 0.f; // Next vertex X
        qreal y1 = 0.f; // Next vertex Y
        qreal a =  0.f;  // Partial signed area

        push_back(at(0));

        // For all vertices except last
        for (int i = count(); --i > 0; )
        {
          x0 = at(i).x();
          y0 = at(i).y();
          x1 = at(i-1).x();
          y1 = at(i-1).y();
          a = x0*y1 - x1*y0;
          area += a;
          centroid.setX(centroid.x() + (x0 + x1) * a);
          centroid.setY(centroid.y() + (y0 + y1) * a);
        }

        pop_back();

        area *= .5f; // real area

        centroid.setX(centroid.x() / (6 * area));
        centroid.setY(centroid.y() / (6 * area));

        area = qAbs(area);
        radius = sqrt(area / M_PI);
      }
      break;
  }
  type = eFinalized;
}

void Polygon::clear()
{
  QPolygonF::clear();
  centroid.setX(0.f);
  centroid.setY(0.f);
  area = 0.f;
  radius = 0.f;
  type = eEdition;
}

void Polygon::draw(QGLShaderProgram *program, const QColor &color) const
{
  program->setUniformValue("color", QVector3D(color.redF(), color.greenF(), color.blueF()));

  const int Count = count();

  if (!Count) return;

  if (Count > 1 && type == eFinalized)
  {
    glBegin(GL_LINE_LOOP);
      for (int i = Count; --i >= 0; )
        glVertex3f(at(i).x(), at(i).y(), 0.f);
    glEnd();
    glBegin(GL_POINTS);
      glVertex3f(centroid.x(), centroid.y(), 0.f);
    glEnd();
  }
  else
  {
    glBegin(GL_POINTS);
      glVertex3f(at(0).x(), at(0).y(), 0.f);
    glEnd();
  }
}

void Polygon::save(QDomDocument &doc, QDomElement &parentNode, const int level) const
{
  QDomElement PolyNode = doc.createElement("polygon");
  parentNode.appendChild(PolyNode);
  PolyNode.setAttribute("level", level);
  foreach(QPointF Point, *this)
  {
    QDomElement PointNode = doc.createElement("point");
    PolyNode.appendChild(PointNode);
    PointNode.setAttribute("x", Point.x());
    PointNode.setAttribute("y", Point.y());
  }
}

void Polygon::load(QDomElement &node)
{
  QDomElement PointElement = node.firstChildElement("point");
  while (!PointElement.isNull())
  {
    bool OkX = false, OkY = false;
    double X = PointElement.attribute("x").toDouble(&OkX);
    double Y = PointElement.attribute("y").toDouble(&OkY);
    if (OkX && OkY) append(QPointF(X, Y));

    PointElement = PointElement.nextSiblingElement("point");
  }
}
