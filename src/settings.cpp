#include "settings.h"

#include "cell.h"

#include <QtCore/QSettings>

QColor Cell::insideColor        (QColor(0xff, 0x1f, 0x1f));
QColor Cell::outsideColor       (QColor(0x1f, 0xff, 0x1f));
QColor Cell::vectorColor        (QColor(0x1f, 0x1f, 0xff));
QColor Cell::averageVectorColor (QColor(0x7f, 0x7f, 0xff));
qreal Cell::arrowLength         (.1);
qreal Cell::arrowHeadLength     (.025);
qreal Cell::arrowHeadHalfWidth  (.0075);
qreal Cell::arrowScale          (0.75);

void Settings::Load()
{
  QSettings settings;
  Cell::insideColor         = settings.value("insideColor",         Cell::insideColor).value<QColor>();
  Cell::outsideColor        = settings.value("outsideColor",        Cell::outsideColor).value<QColor>();
  Cell::vectorColor         = settings.value("vectorColor",         Cell::vectorColor).value<QColor>();
  Cell::averageVectorColor  = settings.value("averageVectorColor",  Cell::averageVectorColor).value<QColor>();
  Cell::arrowLength         = settings.value("arrowLength",         Cell::arrowLength).value<qreal>();
  Cell::arrowHeadLength     = settings.value("arrowHeadLength",     Cell::arrowHeadLength).value<qreal>();
  Cell::arrowHeadHalfWidth  = settings.value("arrowHeadHalfWidth",  Cell::arrowHeadHalfWidth).value<qreal>();
  Cell::arrowScale          = settings.value("arrowScale",          Cell::arrowScale).value<qreal>();
}

void Settings::Save()
{
  QSettings settings;
  settings.setValue("insideColor",         Cell::insideColor);
  settings.setValue("outsideColor",        Cell::outsideColor);
  settings.setValue("vectorColor",         Cell::vectorColor);
  settings.setValue("averageVectorColor",  Cell::averageVectorColor);
  settings.setValue("arrowLength",         Cell::arrowLength);
  settings.setValue("arrowHeadLength",     Cell::arrowHeadLength);
  settings.setValue("arrowHeadHalfWidth",  Cell::arrowHeadHalfWidth);
  settings.setValue("arrowScale",          Cell::arrowScale);
}
