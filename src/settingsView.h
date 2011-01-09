#ifndef __SETTINGSVIEW_H__
#define __SETTINGSVIEW_H__

#include <QtGui/QDialog>

class QSlider;

class SettingsView : public QDialog
{
  Q_OBJECT

  public:
    SettingsView(QWidget *parent = NULL);

  protected slots:
    //void onMinimalStrengthSliderChanged(int value);

  signals:
    //void minimalStrength(qreal);

  protected:
    //QSlider *minimalStrengthSlider;
};

#endif
