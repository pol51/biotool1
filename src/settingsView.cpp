#include "settingsView.h"

#include <QtGui/QGridLayout>
#include <QtGui/QSlider>
#include <QtGui/QCheckBox>

#include <cell.h>
#include <settings.h>

SettingsView::SettingsView(QWidget *parent) :
  QDialog(parent)
{
  QGridLayout *mainLayout = new QGridLayout(this);

  averageArrow = new QCheckBox(tr("Flèche moyenne sur chaque cellule."), this);
  averageArrow->setChecked(Cell::_averageArrow);

  /*minimalStrengthSlider = new QSlider(Qt::Horizontal, this);
  minimalStrengthSlider->setMinimum(0);
  minimalStrengthSlider->setMaximum(100);

  mainLayout->addWidget(minimalStrengthSlider, 0, 0);*/

  mainLayout->addWidget(averageArrow, 0, 0);

  //connect(minimalStrengthSlider, SIGNAL(valueChanged(int)), this, SLOT(onMinimalStrengthSliderChanged(int)));

  connect(averageArrow, SIGNAL(toggled(bool)), this, SLOT(onAverageArrow(bool)));
}

/*void SettingsView::onMinimalStrengthSliderChanged(int value)
{
  emit minimalStrength(value / 100.);
}*/

void SettingsView::onAverageArrow(bool display)
{
  Cell::_averageArrow = display;
  Settings::Save();
}
