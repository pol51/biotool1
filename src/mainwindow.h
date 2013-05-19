#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindow; }

class QLabel;

class SettingsView;

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = NULL);
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
    void doCellCountChanged(int intervalIgnored, int csdIgnored, int total);
    void doAngleVPatchChanged(int angle);
    void doAngleVBeatingChanged(int angle);
    void doExport();
    void doAdvancedExport();
    void doSettings();
    void doAbout();
    void displayCellMenu(const QPoint& pos);

  protected:
    QString getDefaultFilename();

  private:
    Ui::MainWindow *ui;
    QLabel  *cellsLabel;
    QLabel  *angleVPatchLabel;
    QLabel  *angleVBeatingLabel;
    QString fileName;
    QString imageName;
    QAction *lastModeAction;
};

#endif
