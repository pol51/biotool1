#pragma once

#include <QtWidgets/QDialog>

class SettingsView : public QDialog
{
  Q_OBJECT

  public:
    SettingsView(QWidget *parent = nullptr);

  signals:
    void maximalCSD(int);
};
