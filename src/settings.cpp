#include "settings.h"

#include "cell.h"
#include "cellItem.h"
#include "dataCtrl.h"
#include "vCil.h"

#include <QSettings>

bool   Cell::_showAverageArrow        (true);
bool   Cell::_showArrow               (true);
bool   Cell::_showCellBackground      (false);
QColor Cell::_insideColor             (QColor(0xff, 0x1f, 0x1f));
QColor Cell::_outsideColor            (QColor(0x1f, 0xff, 0x1f));
QColor Cell::_insideSelectedColor     (QColor(0x7f, 0x00, 0xff));
QColor Cell::_outsideSelectedColor    (QColor(0x00, 0x7f, 0xff));
QColor Cell::_vectorColor             (QColor(0x1f, 0x1f, 0xff));
QColor Cell::_averageVectorColor      (QColor(0x7f, 0x7f, 0xff));
quint8  Cell::_bgAlpha                (0x7f);
bool   CellItem::_showAverageArrow    (false);
bool   CellItem::_showArrow           (true);
QColor CellItem::_insideColor         (QColor(0xdf, 0x9f, 0x9f));
QColor CellItem::_outsideColor        (QColor(0x9f, 0xdf, 0x9f));
QColor CellItem::_vectorColor         (QColor(0xdf, 0xdf, 0xff));
QColor CellItem::_averageVectorColor  (QColor(0x9f, 0x9f, 0xff));
qreal  CellItem::_arrowLength         (.1);
qreal  CellItem::_arrowHeadLength     (.025);
qreal  CellItem::_arrowHeadHalfWidth  (.0075);
qreal  CellItem::_arrowScale          (1.);
bool   VCil::_showAverageArrow        (false);

QColor DataCtrl::centroidsRefColor  (QColor(0x7f, 0x7f, 0xff));
QList<const DataCtrl::CSVDataType*> DataCtrl::csvSelection;
bool DataCtrl::csvForExcel = true;
int DataCtrl::maximalCSD(0);

void Settings::Load()
{
  QSettings settings;

  if (settings.value("insideColor").value<QColor>().isValid())
  {
    Cell::_showAverageArrow         = settings.value("showAverageArrow",    Cell::_showAverageArrow).value<bool>();
    Cell::_showArrow                = settings.value("showArrow",           Cell::_showArrow).value<bool>();
    Cell::_showCellBackground       = settings.value("showCellBackground",  Cell::_showCellBackground).value<bool>();
    Cell::_insideColor              = settings.value("insideColor",         Cell::_insideColor).value<QColor>();
    Cell::_outsideColor             = settings.value("outsideColor",        Cell::_outsideColor).value<QColor>();
    Cell::_vectorColor              = settings.value("vectorColor",         Cell::_vectorColor).value<QColor>();
    Cell::_averageVectorColor       = settings.value("averageVectorColor",  Cell::_averageVectorColor).value<QColor>();
    Cell::_bgAlpha                  = settings.value("bgAlpha",             Cell::_bgAlpha).value<quint8>();
    Cell::_arrowScale               = settings.value("arrowScale",          Cell::_arrowScale).value<qreal>();
    CellItem::_arrowLength          = settings.value("arrowLength",         CellItem::_arrowLength).value<qreal>();
    CellItem::_arrowHeadLength      = settings.value("arrowHeadLength",     CellItem::_arrowHeadLength).value<qreal>();
    CellItem::_arrowHeadHalfWidth   = settings.value("arrowHeadHalfWidth",  CellItem::_arrowHeadHalfWidth).value<qreal>();
    CellItem::_arrowScale           = settings.value("arrowScale",          CellItem::_arrowScale).value<qreal>();
    VCil::_showAverageArrow         = settings.value("showAverageArrowVCil",VCil::_showAverageArrow).value<bool>();

    DataCtrl::centroidsRefColor     = settings.value("centroidsRefColor",   DataCtrl::centroidsRefColor).value<QColor>();
    DataCtrl::maximalCSD            = settings.value("maximalCSD",          DataCtrl::maximalCSD).value<int>();
    DataCtrl::csvForExcel           = settings.value("csvForExcel",         DataCtrl::csvForExcel).value<bool>();
    DataCtrl::setSelectedDataTypesNames(settings.value("csvSelectedDataType", QStringList()).value<QStringList>());
  }
}

void Settings::Save()
{
  QSettings settings;

  settings.setValue("showAverageArrow",     Cell::_showAverageArrow);
  settings.setValue("showArrow",            Cell::_showArrow);
  settings.setValue("showCellBackground",   Cell::_showCellBackground);
  settings.setValue("insideColor",          Cell::_insideColor);
  settings.setValue("outsideColor",         Cell::_outsideColor);
  settings.setValue("vectorColor",          Cell::_vectorColor);
  settings.setValue("averageVectorColor",   Cell::_averageVectorColor);
  settings.setValue("bgAlpha",              Cell::_bgAlpha);
  settings.setValue("arrowScale",           Cell::_arrowScale);
  settings.setValue("arrowLength",          CellItem::_arrowLength);
  settings.setValue("arrowHeadLength",      CellItem::_arrowHeadLength);
  settings.setValue("arrowHeadHalfWidth",   CellItem::_arrowHeadHalfWidth);
  settings.setValue("arrowScale",           CellItem::_arrowScale);
  settings.setValue("showAverageArrowVCil", VCil::_showAverageArrow);

  settings.setValue("centroidsRefColor",    DataCtrl::centroidsRefColor);
  settings.setValue("maximalCSD",           DataCtrl::maximalCSD);
  settings.setValue("csvForExcel",          DataCtrl::csvForExcel);

  QStringList SelectedDataNames;
  DataCtrl::getSelectedDataTypesNames(SelectedDataNames);
  settings.setValue("csvSelectedDataType",  SelectedDataNames);
}
