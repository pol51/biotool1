#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui/QMainWindow>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  protected:
    void changeEvent(QEvent *e);

  signals:
    void onLoadWorkImage(QImage image);

  protected slots:
    void doLoadImage();

  private:
    Ui::MainWindow *ui;
};

#endif
