#ifndef __SETTINGSVIEW_H__
#define __SETTINGSVIEW_H__

#include <QtWidgets/QDialog>

class QCheckBox;
class QSpinBox;
class QSlider;

class SettingsView : public QDialog
{
  Q_OBJECT

  public:
    SettingsView(QWidget *parent = NULL);

  protected slots:
    void onAverageArrow(bool display);
    void onAverageArrowVCil(bool display);
    void onCellBackground(bool display);
    void onBGAlpha(int value);

  signals:
    void maximalCSD(int);

  protected:
    QCheckBox *averageArrow;
    QCheckBox *averageArrowVCil;
    QCheckBox *cellBackground;
    QSlider   *backgroundAlpha;
    QSpinBox  *maximalCSDTxt;
};

#endif
