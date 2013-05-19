#ifndef __SETTINGSVIEW_H__
#define __SETTINGSVIEW_H__

#include <QtWidgets/QDialog>

class QCheckBox;
class QSpinBox;

class SettingsView : public QDialog
{
  Q_OBJECT

  public:
    SettingsView(QWidget *parent = NULL);

  protected slots:
    void onAverageArrow(bool display);
    void onAverageArrowVCil(bool display);

  signals:
    void maximalCSD(int);

  protected:
    QCheckBox *averageArrow;
    QCheckBox *averageArrowVCil;
    QSpinBox  *maximalCSDTxt;
};

#endif
