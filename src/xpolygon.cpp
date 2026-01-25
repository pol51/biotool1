#include "xpolygon.h"
#include <mapbox/earcut.hpp>

#include <QtXml/QDomDocument>

#ifdef __APPLE__
# include <OpenGL/gl.h>
#else
# include <GL/gl.h>
#endif

#include <cmath>
#include <array>
#include <vector>

void XPolygon::computeData()
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

void XPolygon::clear()
{
  QPolygonF::clear();
  centroid.setX(0.f);
  centroid.setY(0.f);
  area = 0.f;
  radius = 0.f;
  type = eEdition;
}

void XPolygon::draw() const
{
  const int Count = count();

  if (!Count) return;

  switch (type)
  {
    case eEdition:
      glColor3f(.75f, .75f, .75f);
      for (int i = Count; --i >= 0; )
      {
        glBegin(GL_POINTS);
          glVertex3f(at(i).x(), at(i).y(), 0.f);
        glEnd();
      }
      break;
    case eFinalized:
      if (Count > 1)
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
      break;
    default:
      break;
  }
}



void XPolygon::drawBackground() const
{
  const int Count(count());

  if (Count < 3) return;
  
  // Convert QPolygonF to earcut format
  using Point = std::array<double, 2>;
  std::vector<std::vector<Point>> polygon;
  
  std::vector<Point> points;
  points.reserve(Count);
  for (int i = 0; i < Count; ++i)
  {
    points.push_back({at(i).x(), at(i).y()});
  }
  polygon.push_back(points);
  
  // Triangulate using earcut
  auto indices = mapbox::earcut<uint32_t>(polygon);
  
  if (indices.empty()) return;
  
  // Render triangles with OpenGL
  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i < indices.size(); i += 3) {
    if (i + 2 < indices.size()) {
      const Point& p0 = points[indices[i]];
      const Point& p1 = points[indices[i + 1]];
      const Point& p2 = points[indices[i + 2]];
      
      glVertex3d(p0[0], p0[1], 0.0);
      glVertex3d(p1[0], p1[1], 0.0);
      glVertex3d(p2[0], p2[1], 0.0);
    }
  }
  glEnd();
}

void XPolygon::save(QDomDocument &doc, QDomElement &parentNode, const int level) const
{
  QDomElement PolyNode = doc.createElement("polygon");
  parentNode.appendChild(PolyNode);
  PolyNode.setAttribute("level", level);
  for (auto Point: *this)
  {
    QDomElement PointNode = doc.createElement("point");
    PolyNode.appendChild(PointNode);
    PointNode.setAttribute("x", Point.x());
    PointNode.setAttribute("y", Point.y());
  }
}

void XPolygon::load(QDomElement &node)
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
