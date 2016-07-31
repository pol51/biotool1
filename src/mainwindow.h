#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtWidgets/QMainWindow>

namespace Ui { class MainWindow; }

class QLabel;

class QLineEdit;

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
    void doCellCountChanged(int intervalIgnored, int csdIgnored, int total);
    void doAngleVPatchChanged(int angle);
    void doAngleVBeatingChanged(int angle);
    void doExport();
    void doAdvancedExport();
    void doSettings();
    void doAbout();
    void displayCellMenu(const QPoint& pos);

    void setImageWidth();
    void setImageHeight();

  protected:
    QString getDefaultFilename();

  private:
    Ui::MainWindow *ui;
    QLabel  *cellsLabel;
    QLabel  *angleVPatchLabel;
    QLabel  *angleVBeatingLabel;
    QLineEdit *imageWidth;
    QLineEdit *imageHeight;
    QAction *lastModeAction;
    QString fileName;
    QString imageName;
};

#endif
