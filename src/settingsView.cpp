#include "settingsView.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>

#include <cell.h>
#include <dataCtrl.h>
#include <settings.h>

SettingsView::SettingsView(QWidget *parent) :
  QDialog(parent)
{
  QGridLayout *mainLayout = new QGridLayout(this);

  averageArrow = new QCheckBox(tr("Dispaly the average arrow on every cell."), this);
  averageArrow->setChecked(Cell::_averageArrow);
  averageArrowVCil = new QCheckBox(tr("Dispaly the average arrow on every VCil."), this);
  averageArrowVCil->setChecked(VCil::_averageArrow);
  cellBackground = new QCheckBox(tr("Dispaly the background on every cell."), this);
  cellBackground->setChecked(Cell::_cellBackground);

  backgroundAlpha = new QSlider(Qt::Horizontal, this);
  backgroundAlpha->setMaximum(0xff);
  backgroundAlpha->setValue(Cell::_bgAlpha);

  maximalCSDTxt = new QSpinBox(this);
  maximalCSDTxt->setMaximum(INT_MAX);
  maximalCSDTxt->setValue(DataCtrl::maximalCSD);

  QLabel *MaxCSDLbl(new QLabel(tr("CSD maximum threshold:"), this));
  QLabel *BackgroundAlphaLbl(new QLabel(tr("Cell background transparency:"), this));

  mainLayout->addWidget(MaxCSDLbl,          0, 0, 1, 1);
  mainLayout->addWidget(maximalCSDTxt,      0, 1, 1, 1);
  mainLayout->addWidget(averageArrow,       1, 0, 1, 2);
  mainLayout->addWidget(averageArrowVCil,   2, 0, 1, 2);
  mainLayout->addWidget(cellBackground,     3, 0, 1, 2);
  mainLayout->addWidget(BackgroundAlphaLbl, 4, 0, 1, 1);
  mainLayout->addWidget(backgroundAlpha,    4, 1, 1, 1);
  mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 4,0,1,2);

  connect(maximalCSDTxt,    static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this, &SettingsView::maximalCSD);
  connect(averageArrow,     &QCheckBox::toggled,    this, &SettingsView::onAverageArrow);
  connect(averageArrowVCil, &QCheckBox::toggled,    this, &SettingsView::onAverageArrowVCil);
  connect(cellBackground,   &QCheckBox::toggled,    this, &SettingsView::onCellBackground);
  connect(backgroundAlpha,  &QSlider::valueChanged, this, &SettingsView::onBGAlpha);
}

void SettingsView::onAverageArrow(bool display)
{
  Cell::_averageArrow = display;
  Settings::Save();
}

void SettingsView::onAverageArrowVCil(bool display)
{
  VCil::_averageArrow = display;
  Settings::Save();
}

void SettingsView::onCellBackground(bool display)
{
  Cell::_cellBackground = display;
  Settings::Save();
}

void SettingsView::onBGAlpha(int value)
{
  Cell::_bgAlpha = value;
  Settings::Save();
}
