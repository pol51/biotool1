#include "settings.h"

#include "cell.h"
#include "cellItem.h"
#include "dataCtrl.h"
#include "vCil.h"

#include <QtCore/QSettings>

bool   Cell::_averageArrow            (true);
bool   Cell::_cellBackground          (false);
QColor Cell::_insideColor             (QColor(0xff, 0x1f, 0x1f));
QColor Cell::_outsideColor            (QColor(0x1f, 0xff, 0x1f));
QColor Cell::_insideSelectedColor     (QColor(0x7f, 0x00, 0xff));
QColor Cell::_outsideSelectedColor    (QColor(0x00, 0x7f, 0xff));
QColor Cell::_vectorColor             (QColor(0x1f, 0x1f, 0xff));
QColor Cell::_averageVectorColor      (QColor(0x7f, 0x7f, 0xff));
quint8  Cell::_bgAlpha                (0x7f);
bool   CellItem::_averageArrow        (false);
QColor CellItem::_insideColor         (QColor(0xdf, 0x9f, 0x9f));
QColor CellItem::_outsideColor        (QColor(0x9f, 0xdf, 0x9f));
QColor CellItem::_vectorColor         (QColor(0xdf, 0xdf, 0xff));
QColor CellItem::_averageVectorColor  (QColor(0x9f, 0x9f, 0xff));
qreal  CellItem::_arrowLength         (.1);
qreal  CellItem::_arrowHeadLength     (.025);
qreal  CellItem::_arrowHeadHalfWidth  (.0075);
qreal  CellItem::_arrowScale          (0.75);
bool   VCil::_averageArrow            (false);

QColor DataCtrl::centroidsRefColor  (QColor(0x7f, 0x7f, 0xff));
QVector<const DataCtrl::CSVDataType*> DataCtrl::csvSelection;
int DataCtrl::maximalCSD(0);

void Settings::Load()
{
  QSettings settings;

  Cell::_averageArrow             = settings.value("averageArrow",        Cell::_averageArrow).value<bool>();
  Cell::_cellBackground           = settings.value("cellBackground",      Cell::_cellBackground).value<bool>();
  Cell::_insideColor              = settings.value("insideColor",         Cell::_insideColor).value<QColor>();
  Cell::_outsideColor             = settings.value("outsideColor",        Cell::_outsideColor).value<QColor>();
  Cell::_vectorColor              = settings.value("vectorColor",         Cell::_vectorColor).value<QColor>();
  Cell::_averageVectorColor       = settings.value("averageVectorColor",  Cell::_averageVectorColor).value<QColor>();
  Cell::_bgAlpha                  = settings.value("bgAlpha",             Cell::_bgAlpha).value<quint8>();
  CellItem::_arrowLength          = settings.value("arrowLength",         CellItem::_arrowLength).value<qreal>();
  CellItem::_arrowHeadLength      = settings.value("arrowHeadLength",     CellItem::_arrowHeadLength).value<qreal>();
  CellItem::_arrowHeadHalfWidth   = settings.value("arrowHeadHalfWidth",  CellItem::_arrowHeadHalfWidth).value<qreal>();
  CellItem::_arrowScale           = settings.value("arrowScale",          CellItem::_arrowScale).value<qreal>();
  VCil::_averageArrow             = settings.value("averageArrowVCil",    VCil::_averageArrow).value<bool>();

  DataCtrl::centroidsRefColor     = settings.value("centroidsRefColor",   DataCtrl::centroidsRefColor).value<QColor>();
  DataCtrl::maximalCSD            = settings.value("maximalCSD",          DataCtrl::maximalCSD).value<int>();
  DataCtrl::setSelectedDataTypesNames(settings.value("csvSelectedDataType", QStringList()).value<QStringList>());
}

void Settings::Save()
{
  QSettings settings;

  settings.setValue("averageArrow",         Cell::_averageArrow);
  settings.setValue("cellBackground",       Cell::_cellBackground);
  settings.setValue("insideColor",          Cell::_insideColor);
  settings.setValue("outsideColor",         Cell::_outsideColor);
  settings.setValue("vectorColor",          Cell::_vectorColor);
  settings.setValue("averageVectorColor",   Cell::_averageVectorColor);
  settings.setValue("bgAlpha",              Cell::_bgAlpha);
  settings.setValue("arrowLength",          CellItem::_arrowLength);
  settings.setValue("arrowHeadLength",      CellItem::_arrowHeadLength);
  settings.setValue("arrowHeadHalfWidth",   CellItem::_arrowHeadHalfWidth);
  settings.setValue("arrowScale",           CellItem::_arrowScale);
  settings.setValue("averageArrowVCil",     VCil::_averageArrow);

  settings.setValue("centroidsRefColor",    DataCtrl::centroidsRefColor);
  settings.setValue("maximalCSD",           DataCtrl::maximalCSD);

  QStringList SelectedDataNames;
  DataCtrl::getSelectedDataTypesNames(SelectedDataNames);
  settings.setValue("csvSelectedDataType",  SelectedDataNames);
}
