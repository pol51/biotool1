#include "xpolygon.h"

#include <QOpenGLContext>
#include <QLineF>
#include <QDomDocument>

#ifdef __APPLE__
# include <OpenGL/glu.h>
#else
# include <GL/glu.h>
#endif

#include <math.h>

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
        qreal a =  0.f; // Partial signed area

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

class TesselatorCB {
  public:
    static void resetVertexIndex() { _vertexIndex = 0; }
    static void tessVertexCB(const GLvoid *data) { glVertex3dv((const GLdouble*)data); }
    static void tessCombineCB(const GLdouble newVertex[3], const GLdouble **, const GLfloat *, GLdouble **outData)
    {
      memcpy(&_vertices[_vertexIndex], newVertex, sizeof(GLdouble) * 3);
      *outData = _vertices[_vertexIndex++];
    }

  protected:
    static GLdouble _vertices[256][3];
    static int _vertexIndex;
};
GLdouble TesselatorCB::_vertices[256][3];
int TesselatorCB::_vertexIndex = 0;

void XPolygon::drawBackground() const
{
  const int Count(count());

  TesselatorCB::resetVertexIndex();

  GLuint Id = glGenLists(1);
  GLUtesselator *Tess = gluNewTess();

  GLdouble **Points;
  Points = (GLdouble**)malloc(sizeof(GLdouble*) * Count);
  for (int i = Count; --i >= 0; )
  {
    Points[i] = (GLdouble*)malloc(sizeof(GLdouble) * 3);
    Points[i][0] = at(i).x();
    Points[i][1] = at(i).y();
    Points[i][2] = 0.f;
  }

  gluTessCallback(Tess, GLU_TESS_BEGIN,   (void (*)())glBegin);
  gluTessCallback(Tess, GLU_TESS_END,     (void (*)())glEnd);
  gluTessCallback(Tess, GLU_TESS_VERTEX,  (void (*)())TesselatorCB::tessVertexCB);
  gluTessCallback(Tess, GLU_TESS_COMBINE, (void (*)())TesselatorCB::tessCombineCB);

  gluTessProperty(Tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
  glNewList(Id, GL_COMPILE);
  gluTessBeginPolygon(Tess, 0);
    gluTessBeginContour(Tess);
      for (int i = Count; --i >= 0; )
        gluTessVertex(Tess, Points[i], Points[i]);
    gluTessEndContour(Tess);
  gluTessEndPolygon(Tess);
  glEndList();

  gluDeleteTess(Tess);

  glCallList(Id);

  for (int i = Count; --i >= 0; )
    free(Points[i]);
  free(Points);
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
