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
  QGridLayout *MainLayout = new QGridLayout(this);

  QLabel *MaxCSDLbl(new QLabel(tr("CSD maximum threshold:"), this));
  QSpinBox *MaximalCSDTxt = new QSpinBox(this);
  MaximalCSDTxt->setMaximum(INT_MAX);
  MaximalCSDTxt->setValue(DataCtrl::maximalCSD);

  QCheckBox *ShowAverageArrow = new QCheckBox(tr("Dispaly the average arrow on every cell."), this);
  ShowAverageArrow->setChecked(Cell::_showAverageArrow);

  QCheckBox *ShowArrow = new QCheckBox(tr("Dispaly the arrow on every cell."), this);
  ShowArrow->setChecked(Cell::_showArrow);

  QCheckBox *ShowAverageArrowVCil = new QCheckBox(tr("Dispaly the average arrow on every VCil."), this);
  ShowAverageArrowVCil->setChecked(VCil::_showAverageArrow);

  QCheckBox *ShowCellBackground = new QCheckBox(tr("Dispaly the background on every cell."), this);
  ShowCellBackground->setChecked(Cell::_showCellBackground);

  QLabel *BackgroundAlphaLbl(new QLabel(tr("Cell background transparency:"), this));
  QSlider *BackgroundAlpha = new QSlider(Qt::Horizontal, this);
  BackgroundAlpha->setMaximum(0xff);
  BackgroundAlpha->setValue(Cell::_bgAlpha);

  QLabel *ArrowScaleLbl(new QLabel(tr("Call arrows scale:"), this));
  QSlider *ArrowScale = new QSlider(Qt::Horizontal, this);
  ArrowScale->setMaximum(0x400);
  ArrowScale->setValue(qRound(Cell::_arrowScale * 0x100));

  MainLayout->addWidget(MaxCSDLbl,            0, 0, 1, 1);
  MainLayout->addWidget(MaximalCSDTxt,        0, 1, 1, 1);
  MainLayout->addWidget(ShowAverageArrow,     1, 0, 1, 2);
  MainLayout->addWidget(ShowArrow,            2, 0, 1, 2);
  MainLayout->addWidget(ShowAverageArrowVCil, 3, 0, 1, 2);
  MainLayout->addWidget(ShowCellBackground,   4, 0, 1, 2);
  MainLayout->addWidget(BackgroundAlphaLbl,   5, 0, 1, 1);
  MainLayout->addWidget(BackgroundAlpha,      5, 1, 1, 1);
  MainLayout->addWidget(ArrowScaleLbl,        6, 0, 1, 1);
  MainLayout->addWidget(ArrowScale,           6, 1, 1, 1);
  MainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding), 7, 0, 1, 2);

  connect(MaximalCSDTxt,        static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),  this, &SettingsView::maximalCSD);
  connect(ShowAverageArrow,     &QCheckBox::toggled,    [](bool display) { Cell::_showAverageArrow = display; Settings::Save(); });
  connect(ShowArrow,            &QCheckBox::toggled,    [](bool display) { Cell::_showArrow = display; Settings::Save(); });
  connect(ShowAverageArrowVCil, &QCheckBox::toggled,    [](bool display) { VCil::_showAverageArrow = display; Settings::Save(); });
  connect(ShowCellBackground,   &QCheckBox::toggled,    [](bool display) { Cell::_showCellBackground = display; Settings::Save(); });
  connect(BackgroundAlpha,      &QSlider::valueChanged, [](int value) { Cell::_bgAlpha = static_cast<quint8>(value); Settings::Save(); });
  connect(ArrowScale,           &QSlider::valueChanged, [](int value) { Cell::_arrowScale = static_cast<qreal>(value) / 0x100; Settings::Save(); });
}
