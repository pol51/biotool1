#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dataCtrl.h"
#include "settingsView.h"
#include "csvExportOptions.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtGui/QDoubleValidator>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  DataCtrl const * DataC(&ui->imageView->data());

  connect(ui->actExit,                &QAction::triggered, this,          &MainWindow::close);
  connect(ui->actLoadImage,           &QAction::triggered, this,          &MainWindow::doLoadImage);
  connect(ui->actCloseImage,          &QAction::triggered, ui->imageView, &ImageView::doCloseImage);
  connect(ui->actModeEdit,            &QAction::triggered, this,          &MainWindow::doChangeMode);
  connect(ui->actModeView,            &QAction::triggered, this,          &MainWindow::doChangeMode);
  connect(ui->actModeDefineCentroid,  &QAction::triggered, this,          &MainWindow::doChangeMode);
  connect(ui->actResetView,           &QAction::triggered, ui->imageView, &ImageView::doResetView);
  connect(ui->actNew,                 &QAction::triggered, this,          &MainWindow::doNew);
  connect(ui->actSave,                &QAction::triggered, this,          &MainWindow::doSave);
  connect(ui->actSaveAs,              &QAction::triggered, this,          &MainWindow::doSaveAs);
  connect(ui->actOpen,                &QAction::triggered, this,          &MainWindow::doOpen);
  connect(ui->actExport,              &QAction::triggered, this,          &MainWindow::doExport);
  connect(ui->actExportOption,        &QAction::triggered, this,          &MainWindow::doAdvancedExport);
  connect(ui->actSettings,            &QAction::triggered, this,          &MainWindow::doSettings);
  connect(ui->actAbout,               &QAction::triggered, this,          &MainWindow::doAbout);

  connect(this, &MainWindow::onLoadWorkImage, ui->imageView, &ImageView::doChangeImage);

  connect(DataC, &DataCtrl::countChanged,          this, &MainWindow::doCellCountChanged);
  connect(DataC, &DataCtrl::angleVPatchChanged,    this, &MainWindow::doAngleVPatchChanged);
  connect(DataC, &DataCtrl::angleVBeatingChanged,  this, &MainWindow::doAngleVBeatingChanged);

  connect(ui->objectsView, &QTreeView::entered,                     DataC, &DataCtrl::setSelection);
  connect(ui->objectsView, &QTreeView::pressed,                     DataC, &DataCtrl::setSelection);
  connect(ui->objectsView, &QTreeView::clicked,                     DataC, &DataCtrl::setSelection);
  connect(ui->objectsView, &QTreeView::activated,                   DataC, &DataCtrl::setSelection);
  connect(ui->objectsView, &QTreeView::customContextMenuRequested,  this,  &MainWindow::displayCellMenu);

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

  QDoubleValidator *HeightValidator = new QDoubleValidator(ui->toolBar);
  HeightValidator->setDecimals(3);
  HeightValidator->setBottom(0.);
  QDoubleValidator *WidthValidator = new QDoubleValidator(ui->toolBar);
  WidthValidator->setDecimals(3);
  WidthValidator->setBottom(0.);

  ui->toolBar->addWidget(new QLabel("width:", ui->toolBar));
  imageWidth = new QLineEdit(ui->toolBar);
  imageWidth->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  imageWidth->setValidator(WidthValidator);
  ui->toolBar->addWidget(imageWidth);
  ui->toolBar->addSeparator();

  ui->toolBar->addWidget(new QLabel("height:", ui->toolBar));
  imageHeight = new QLineEdit(ui->toolBar);
  imageHeight->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  imageHeight->setValidator(HeightValidator);
  ui->toolBar->addWidget(imageHeight);

  QLabel *Spacer = new QLabel("", ui->toolBar);
  Spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
  ui->toolBar->addWidget(Spacer);

  connect(imageHeight,  &QLineEdit::editingFinished,  this, &MainWindow::setImageHeight);
  connect(imageWidth,   &QLineEdit::editingFinished,  this, &MainWindow::setImageWidth);
  connect(imageHeight,  &QLineEdit::textChanged,      this, &MainWindow::setImageHeight);
  connect(imageWidth,   &QLineEdit::textChanged,      this, &MainWindow::setImageWidth);

  imageWidth->setText(QLocale().toString(1.f));
  setImageWidth();
  ui->imageView->setFocus();
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

  imageWidth->setText(QLocale().toString(1.f));
  setImageWidth();
  ui->imageView->setFocus();
}

void MainWindow::doChangeMode(bool activated)
{
  QAction *action = static_cast<QAction*>(sender());
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
    imageHeight->setText(QLocale().toString(1.f));
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
      imageHeight->setText(QLocale().toString(ui->imageView->imageRealHeight()));
    }
  }

  ui->imageView->grabKeyboard();
}

void MainWindow::doCellCountChanged(int intervalIgnored, int csdIgnored, int count)
{
  cellsLabel->setText(QString(" [%1/%2 cell%3] [!csd: %4%]").
                      arg(count - intervalIgnored).arg(count).arg(count?"s":"").arg(count?((count - static_cast<qreal>(csdIgnored)) / count * 100.):0));
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
  connect(&Settings, &SettingsView::maximalCSD, &ui->imageView->data(), &DataCtrl::setMaximalCSD);
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

void MainWindow::setImageWidth()
{
  imageHeight->blockSignals(true);
  const QLocale Locale;
  ui->imageView->setImageRealWidth(Locale.toDouble(imageWidth->text()));
  imageHeight->setText(Locale.toString(ui->imageView->imageRealHeight()));
  imageHeight->blockSignals(false);
}

void MainWindow::setImageHeight()
{
  imageWidth->blockSignals(true);
  const QLocale Locale;
  ui->imageView->setImageRealHeight(Locale.toDouble(imageHeight->text()));
  imageWidth->setText(Locale.toString(ui->imageView->imageRealWidth()));
  imageWidth->blockSignals(false);
}
