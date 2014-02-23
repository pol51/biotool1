#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dataCtrl.h"
#include "settingsView.h"
#include "csvExportOptions.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  connect(ui->actExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(this, SIGNAL(onLoadWorkImage(QImage)), ui->imageView, SLOT(doChangeImage(QImage)));
  connect(ui->actCloseImage, SIGNAL(triggered()), ui->imageView, SLOT(doCloseImage()));
  connect(ui->actModeEdit, SIGNAL(triggered(bool)), this, SLOT(doChangeMode(bool)));
  connect(ui->actModeView, SIGNAL(triggered(bool)), this, SLOT(doChangeMode(bool)));
  connect(ui->actModeDefineCentroid, SIGNAL(triggered(bool)), this, SLOT(doChangeMode(bool)));
  connect(ui->actResetView, SIGNAL(triggered()), ui->imageView, SLOT(doResetView()));
  connect(ui->actNew, SIGNAL(triggered()), this, SLOT(doNew()));
  connect(ui->actSave, SIGNAL(triggered()), this, SLOT(doSave()));
  connect(ui->actSaveAs, SIGNAL(triggered()), this, SLOT(doSaveAs()));
  connect(ui->actOpen, SIGNAL(triggered()), this, SLOT(doOpen()));
  connect(&ui->imageView->data(), SIGNAL(countChanged(int, int, int)), this, SLOT(doCellCountChanged(int, int, int)));
  connect(&ui->imageView->data(), SIGNAL(angleVPatchChanged(int)), this, SLOT(doAngleVPatchChanged(int)));
  connect(&ui->imageView->data(), SIGNAL(angleVBeatingChanged(int)), this, SLOT(doAngleVBeatingChanged(int)));
  connect(ui->actExport, SIGNAL(triggered()), this, SLOT(doExport()));
  connect(ui->actExportOption, SIGNAL(triggered()), this, SLOT(doAdvancedExport()));
  connect(ui->actSettings, SIGNAL(triggered()), this, SLOT(doSettings()));
  connect(ui->actAbout, SIGNAL(triggered()), this, SLOT(doAbout()));
  connect(ui->objectsView, SIGNAL(entered(QModelIndex)), &ui->imageView->data(), SLOT(setSelection(QModelIndex)));
  connect(ui->objectsView, SIGNAL(pressed(QModelIndex)), &ui->imageView->data(), SLOT(setSelection(QModelIndex)));
  connect(ui->objectsView, SIGNAL(clicked(QModelIndex)), &ui->imageView->data(), SLOT(setSelection(QModelIndex)));
  connect(ui->objectsView, SIGNAL(activated(QModelIndex)), &ui->imageView->data(), SLOT(setSelection(QModelIndex)));
  connect(ui->objectsView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(displayCellMenu(QPoint)));

  ui->actModeView->blockSignals(true);
  ui->actModeView->trigger();
  ui->actModeView->blockSignals(false);
  lastModeAction = ui->actModeView;

  ui->objectsView->setModel(&ui->imageView->data());
  ui->objectsView->setContextMenuPolicy(Qt::CustomContextMenu);

  cellsLabel = new QLabel(" [000000 cells] ");
  cellsLabel->setAlignment(Qt::AlignLeft);
  cellsLabel->setMinimumSize(cellsLabel->sizeHint());
  doCellCountChanged(0, 0, 0);

  angleVPatchLabel = new QLabel(" vPatch[-000 deg] ");
  angleVPatchLabel->setAlignment(Qt::AlignLeft);
  angleVPatchLabel->setMinimumSize(angleVPatchLabel->sizeHint());
  angleVPatchLabel->setVisible(false);
  doAngleVPatchChanged(0);

  angleVBeatingLabel = new QLabel(" vBeating[-000 deg] ");
  angleVBeatingLabel->setAlignment(Qt::AlignLeft);
  angleVBeatingLabel->setMinimumSize(angleVBeatingLabel->sizeHint());
  angleVBeatingLabel->setVisible(false);
  doAngleVBeatingChanged(0);

  statusBar()->addWidget(cellsLabel);
  statusBar()->addWidget(angleVPatchLabel);
  statusBar()->addWidget(angleVBeatingLabel);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
  QMainWindow::changeEvent(e);
  switch (e->type())
  {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

bool MainWindow::askForUnsavedChanges(const QString &title)
{
  return (ui->imageView->data().isSaved() ||
          QMessageBox::question(this,
                                title,
                                tr("The work-in-progress contains unsaved changes.\n"
                                   "Are you sure you want to continue?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void MainWindow::doLoadImage()
{
  ui->imageView->releaseKeyboard();

  imageName = QString();

  QString filename(QFileDialog::getOpenFileName(this, tr("Select an image"), tr("."), tr("Images (*.png *.jpg *.jpeg *.tif *.tiff);;All (*)")));
  if (filename.isEmpty()) return;
  if (!QFileInfo(filename).isReadable())
  {
    QMessageBox::warning(this, tr("Error"), tr("Impossible to read the file."));
    ui->imageView->grabKeyboard();
    return;
  }
  QImage Image(filename);
  if (Image.isNull())
  {
    QMessageBox::warning(this, tr("Error"), tr("Unsupported image format."));
    ui->imageView->grabKeyboard();
    return;
  }

  imageName = QFileInfo(filename).baseName();

  emit onLoadWorkImage(Image);
  ui->imageView->grabKeyboard();
}

void MainWindow::doChangeMode(bool activated)
{

  QAction *action = (QAction*)sender();
  if (activated)
  {
    if (lastModeAction)
    {
      lastModeAction->blockSignals(true);
      lastModeAction->trigger();
      lastModeAction->blockSignals(false);
    }
    if (action == ui->actModeView)
      ui->imageView->changeMode(DataCtrl::eModeView);
    if (action == ui->actModeEdit)
      ui->imageView->changeMode(DataCtrl::eModeEdit);
    if (action == ui->actModeDefineCentroid)
      ui->imageView->changeMode(DataCtrl::eModeDefineCentroid);
    lastModeAction = action;
  }
  else
  {
    action->blockSignals(true);
    action->trigger();
    action->blockSignals(false);
  }
}

void MainWindow::doNew()
{
  if (askForUnsavedChanges(tr("New document")))
  {
    ui->imageView->data().clear();
    fileName.clear();
  }
}

void MainWindow::doSave()
{
  if (fileName.isEmpty())
    doSaveAs();
  else
    ui->imageView->data().save(fileName);
}

void MainWindow::doSaveAs()
{
  ui->imageView->releaseKeyboard();

  QFileDialog FileDialog(this, tr("Save as"), tr("."), tr("Documents (*.xml);;All (*)"));
  FileDialog.setAcceptMode(QFileDialog::AcceptSave);
  FileDialog.setFileMode(QFileDialog::AnyFile);
  FileDialog.selectFile(getDefaultFilename().append(".xml"));
  if (FileDialog.exec() == QDialog::Accepted)
  {
    const QString filename(FileDialog.selectedFiles().at(0));
    if (!filename.isEmpty())
    {
      fileName = filename;
      doSave();
    }
  }

  ui->imageView->grabKeyboard();
}

void MainWindow::doOpen()
{
  ui->imageView->releaseKeyboard();

  if (askForUnsavedChanges(tr("Load a document")))
  {
    const QString filename(QFileDialog::getOpenFileName(this, tr("Load a document"), tr("."), tr("Documents (*.xml);;All (*)")));
    if (!filename.isEmpty())
    {
      fileName = filename;
      ui->imageView->data().load(fileName);
    }
  }

  ui->imageView->grabKeyboard();
}

void MainWindow::doCellCountChanged(int intervalIgnored, int csdIgnored, int count)
{
  cellsLabel->setText(QString(" [%1/%2 cell%3] [!csd: %4%]").
                      arg(count - intervalIgnored).arg(count).arg(count?"s":"").arg(count?((count - (qreal)csdIgnored) / count * 100.):0));
}

void MainWindow::doAngleVPatchChanged(int angle)
{
  angleVPatchLabel->setText(QString(" vPatch[%1 deg] ").arg(angle));
}

void MainWindow::doAngleVBeatingChanged(int angle)
{
  angleVBeatingLabel->setText(QString(" vBeating[%1 deg] ").arg(angle));
}

void MainWindow::doExport()
{
  ui->imageView->releaseKeyboard();

  QFileDialog FileDialog(this, tr("Export as"), tr("."), tr("Csv files (*.csv)"));
  FileDialog.setAcceptMode(QFileDialog::AcceptSave);
  FileDialog.setFileMode(QFileDialog::AnyFile);
  FileDialog.selectFile(getDefaultFilename().append(ui->imageView->data().getCsvSuffix()).append(QString("-CsdMax%1").arg(DataCtrl::maxCSD())).append(".csv"));
  if (FileDialog.exec() == QDialog::Accepted)
  {
    const QString filename(FileDialog.selectedFiles().at(0));
    if (!filename.isEmpty())
      ui->imageView->data().exportCsv(filename);
  }

  ui->imageView->grabKeyboard();
}

void MainWindow::doAdvancedExport()
{
  ui->imageView->releaseKeyboard();

  CsvExportOptions CsvExportDialog(this);
  if (CsvExportDialog.exec() == QDialog::Accepted)
    doExport();

  ui->imageView->grabKeyboard();
}

void MainWindow::doSettings()
{
  SettingsView Settings(this);
  connect(&Settings, SIGNAL(maximalCSD(const int&)), &ui->imageView->data(), SLOT(setMaximalCSD(const int&)));
  Settings.exec();
}

void MainWindow::doAbout()
{
  #define _XSTR(var)  _STR(var)
  #define _STR(var)   #var
  #define GIT_VERSION _XSTR(BT1_GIT_VERSION)

  QMessageBox::information(this, tr("About"), tr("Biotool1\nversion 1-%1\nhttps://github.com/pol51/biotool1").arg(GIT_VERSION));
}

void MainWindow::displayCellMenu(const QPoint &pos)
{
  if (Cell::selected())
  {
    QMenu Menu(ui->objectsView);
    Menu.addAction(tr("Delete"), &ui->imageView->data(), SLOT(removeSelectedForm()));
    if (!Cell::edited() || Cell::edited() != Cell::selected())
      Menu.addAction(tr("Start edition"), &ui->imageView->data(), SLOT(startEditSelectedForm()));
    else
      Menu.addAction(tr("Stop edition"),  &ui->imageView->data(), SLOT(stopEditSelectedForm()));
    Menu.exec(ui->objectsView->mapToGlobal(pos));
  }
}

QString MainWindow::getDefaultFilename()
{
  if (fileName.isEmpty())
    return imageName;
  return QFileInfo(fileName).baseName();
}
