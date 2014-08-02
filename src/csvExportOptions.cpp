#include "csvExportOptions.h"

#include <ui_csvExportOptions.h>

#include "dataCtrl.h"
#include "settings.h"

CsvExportOptions::CsvExportOptions(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::CsvExportOptions)
{
  ui->setupUi(this);

  QStringList DataTypes;
  DataCtrl::getDataTypesNames(DataTypes);
  ui->lstDTAvailable->addItems(DataTypes);
  DataCtrl::getSelectedDataTypesNames(DataTypes);
  ui->lstDTSelected->addItems(DataTypes);

  connect(ui->lstDTAvailable, &QListWidget::doubleClicked, this, &CsvExportOptions::addDataType);
  connect(ui->lstDTSelected,  &QListWidget::doubleClicked, this, &CsvExportOptions::delDataType);
}

void CsvExportOptions::accept()
{
  const int Count(ui->lstDTSelected->count());
  QStringList DataTypes;
  for (int i = 0; i < Count; ++i)
    DataTypes.append(ui->lstDTSelected->item(i)->text());
  DataCtrl::setSelectedDataTypesNames(DataTypes);
  Settings::Save();
  QDialog::accept();
}

void CsvExportOptions::addDataType(const QModelIndex &index)
{
  ui->lstDTSelected->addItem(ui->lstDTAvailable->item(index.row())->text());
}

void CsvExportOptions::delDataType(const QModelIndex &index)
{
  ui->lstDTSelected->model()->removeRow(index.row());
}

