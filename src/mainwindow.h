#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui/QMainWindow>

namespace Ui { class MainWindow; }

class QLabel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

  protected:
    void changeEvent(QEvent *e);
    bool askForUnsavedChanges(const QString &title);

  signals:
    void onLoadWorkImage(QImage image);

  protected slots:
    void doLoadImage();
    void doChangeMode(bool activated);
    void doNew();
    void doSave();
    void doSaveAs();
    void doOpen();
    void doCellCountChanged(int count);

  private:
    Ui::MainWindow *ui;
    QLabel *cellsLabel;
    QString fileName;
};

#endif
