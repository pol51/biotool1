#include "settingsView.h"

#include <QtGui/QSlider>
#include <QtGui/QGridLayout>

SettingsView::SettingsView(QWidget *parent) :
  QDialog(parent)
{
  /*QGridLayout *mainLayout =*/ new QGridLayout(this);

  /*minimalStrengthSlider = new QSlider(Qt::Horizontal, this);
  minimalStrengthSlider->setMinimum(0);
  minimalStrengthSlider->setMaximum(100);

  mainLayout->addWidget(minimalStrengthSlider, 0, 0);

  connect(minimalStrengthSlider, SIGNAL(valueChanged(int)), this, SLOT(onMinimalStrengthSliderChanged(int)));*/
}

/*void SettingsView::onMinimalStrengthSliderChanged(int value)
{
  emit minimalStrength(value / 100.);
}*/
