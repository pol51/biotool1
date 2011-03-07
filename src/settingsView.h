#ifndef __SETTINGSVIEW_H__
#define __SETTINGSVIEW_H__

#include <QtGui/QDialog>

class QSlider;
class QCheckBox;

class SettingsView : public QDialog
{
  Q_OBJECT

  public:
    SettingsView(QWidget *parent = NULL);

  protected slots:
    //void onMinimalStrengthSliderChanged(int value);
    void onAverageArrow(bool display);

  signals:
    //void minimalStrength(qreal);

  protected:
    QCheckBox *averageArrow;
    //QSlider *minimalStrengthSlider;
};

#endif
