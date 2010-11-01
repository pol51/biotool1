#include "polygon.h"

#include <QtOpenGL/QGLContext>

#include <QtXml/QDomDocument>

void Polygon::computeData()
{
  const int Count = count();

  switch (Count)
  {
    case 0: return;
    case 1:
      centroid = at(0);
      area = 0.;
      break;
    case 2:
      centroid.setX((at(0).x() + at(1).x()) * .5);
      centroid.setY((at(0).y() + at(1).y()) * .5);
      area = 0.;
      break;
    default:
      {
        centroid.setX(0.);
        centroid.setY(0.);
        area = 0.;

        qreal x0 = 0.0; // Current vertex X
        qreal y0 = 0.0; // Current vertex Y
        qreal x1 = 0.0; // Next vertex X
        qreal y1 = 0.0; // Next vertex Y
        qreal a = 0.0;  // Partial signed area

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

        area *= .5;
        centroid.setX(centroid.x() / (6 * area));
        centroid.setY(centroid.y() / (6 * area));
      }
      break;
  }
  type = eFinalized;
}

void Polygon::clear()
{
  QPolygonF::clear();
  centroid.setX(0.);
  centroid.setY(0.);
  area = 0.;
  type = eEdition;
}

void Polygon::draw() const
{
  const int Count = count();

  if (!Count) return;

  switch (type)
  {
    case eEdition:
      glColor3f(.75, .75, .75);
      for (int i = Count; --i >= 0; )
      {
        glBegin(GL_POINTS);
          glVertex3f(at(i).x(), at(i).y(), 0.);
        glEnd();
      }
      break;
    case eFinalized:
      if (Count > 1)
      {
        glBegin(GL_LINE_LOOP);
          for (int i = Count; --i >= 0; )
            glVertex3f(at(i).x(), at(i).y(), 0.);
        glEnd();
        glBegin(GL_POINTS);
          glVertex3f(centroid.x(), centroid.y(), 0.);
        glEnd();
      }
      else
      {
        glBegin(GL_POINTS);
          glVertex3f(at(0).x(), at(0).y(), 0.);
        glEnd();
      }
      break;
    default:
      break;
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
