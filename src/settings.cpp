#include "settings.h"

#include "cell.h"
#include "cellItem.h"
#include "dataCtrl.h"

#include <QtCore/QSettings>

bool   Cell::averageArrow           (true);
QColor Cell::insideColor            (QColor(0xff, 0x1f, 0x1f));
QColor Cell::outsideColor           (QColor(0x1f, 0xff, 0x1f));
QColor Cell::insideSelectedColor    (QColor(0x7f, 0x00, 0xff));
QColor Cell::outsideSelectedColor   (QColor(0x00, 0x7f, 0xff));
QColor Cell::vectorColor            (QColor(0x1f, 0x1f, 0xff));
QColor Cell::averageVectorColor     (QColor(0x7f, 0x7f, 0xff));
qreal  CellItem::arrowLength        (.1);
qreal  CellItem::arrowHeadLength    (.025);
qreal  CellItem::arrowHeadHalfWidth (.0075);
qreal  CellItem::arrowScale         (0.75);

QColor DataCtrl::centroidsRefColor  (QColor(0x7f, 0x7f, 0xff));
QVector<const DataCtrl::CSVDataType*> DataCtrl::csvSelection;

void Settings::Load()
{
  QSettings settings;

  Cell::averageArrow            = settings.value("averageArrow",        Cell::averageArrow).value<bool>();
  Cell::insideColor             = settings.value("insideColor",         Cell::insideColor).value<QColor>();
  Cell::outsideColor            = settings.value("outsideColor",        Cell::outsideColor).value<QColor>();
  Cell::vectorColor             = settings.value("vectorColor",         Cell::vectorColor).value<QColor>();
  Cell::averageVectorColor      = settings.value("averageVectorColor",  Cell::averageVectorColor).value<QColor>();
  CellItem::arrowLength         = settings.value("arrowLength",         CellItem::arrowLength).value<qreal>();
  CellItem::arrowHeadLength     = settings.value("arrowHeadLength",     CellItem::arrowHeadLength).value<qreal>();
  CellItem::arrowHeadHalfWidth  = settings.value("arrowHeadHalfWidth",  CellItem::arrowHeadHalfWidth).value<qreal>();
  CellItem::arrowScale          = settings.value("arrowScale",          CellItem::arrowScale).value<qreal>();

  DataCtrl::centroidsRefColor = settings.value("centroidsRefColor",   DataCtrl::centroidsRefColor).value<QColor>();
  DataCtrl::setSelectedDataTypesNames(settings.value("csvSelectedDataType", QStringList()).value<QStringList>());
}

void Settings::Save()
{
  QSettings settings;

  settings.setValue("averageArrow",         Cell::averageArrow);
  settings.setValue("insideColor",          Cell::insideColor);
  settings.setValue("outsideColor",         Cell::outsideColor);
  settings.setValue("vectorColor",          Cell::vectorColor);
  settings.setValue("averageVectorColor",   Cell::averageVectorColor);
  settings.setValue("arrowLength",          CellItem::arrowLength);
  settings.setValue("arrowHeadLength",      CellItem::arrowHeadLength);
  settings.setValue("arrowHeadHalfWidth",   CellItem::arrowHeadHalfWidth);
  settings.setValue("arrowScale",           CellItem::arrowScale);

  settings.setValue("centroidsRefColor",    DataCtrl::centroidsRefColor);

  QStringList SelectedDataNames;
  DataCtrl::getSelectedDataTypesNames(SelectedDataNames);
  settings.setValue("csvSelectedDataType",  SelectedDataNames);
}
