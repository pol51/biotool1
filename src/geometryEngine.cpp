#include "geometryEngine.h"

#include <QtGui/QVector2D>

struct VertexData
{
  QVector2D position;
  QVector2D texCoord;
};

GeometryEngine::~GeometryEngine()
{
  glDeleteBuffers(2, vboIds);
}

void GeometryEngine::init(const qreal ratio)
{
  initializeGLFunctions();

  glDeleteBuffers(2, vboIds);
  glGenBuffers(2, vboIds);

  VertexData vertices[] = {
    {QVector2D( ratio, -1.0), QVector2D(1.0, 1.0)},
    {QVector2D(-ratio, -1.0), QVector2D(0.0, 1.0)},
    {QVector2D( ratio,  1.0), QVector2D(1.0, 0.0)},
    {QVector2D(-ratio,  1.0), QVector2D(0.0, 0.0)},
  };

  GLushort indices[] = {0,  1,  2,  3};

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void GeometryEngine::draw(QGLShaderProgram *program)
{
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

  // vertices coordinates
  int vertexLocation = program->attributeLocation("a_position");
  program->enableAttributeArray(vertexLocation);
  glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)0);

  // textures coordinates
  int texcoordLocation = program->attributeLocation("a_texcoord");
  program->enableAttributeArray(texcoordLocation);
  glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (const void*)sizeof(QVector2D));

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, 0);
}
