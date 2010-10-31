#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dataCtrl.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

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
  connect(ui->actResetView, SIGNAL(triggered()), ui->imageView, SLOT(doResetView()));
  connect(ui->actNew, SIGNAL(triggered()), this, SLOT(doNew()));
  connect(ui->actSave, SIGNAL(triggered()), this, SLOT(doSave()));
  connect(ui->actSaveAs, SIGNAL(triggered()), this, SLOT(doSaveAs()));
  connect(ui->actOpen, SIGNAL(triggered()), this, SLOT(doOpen()));

  ui->actModeView->blockSignals(true);
  ui->actModeView->trigger();
  ui->actModeView->blockSignals(false);
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
                                tr("Le travail en cours comporte des modification non sauvegardées.\n"
                                   "Etes-vous sûr de vouloir continuer?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
}

void MainWindow::doLoadImage()
{
  QString filename(QFileDialog::getOpenFileName(this, tr("Choisir une image"), tr("."), tr("Images (*.png *.jpg *.jpeg *.tif *.tiff);;Tous (*)")));
  if (filename.isEmpty()) return;
  if (!QFileInfo(filename).isReadable())
  {
    QMessageBox::warning(this, tr("Erreur"), tr("Impossible de lire le fichier."));
    return;
  }
  QImage Image(filename);
  if (Image.isNull())
  {
    QMessageBox::warning(this, tr("Erreur"), tr("Format d'image incorrect."));
    return;
  }

  emit onLoadWorkImage(Image);
}

void MainWindow::doChangeMode(bool activated)
{
  QAction *action = (QAction*)sender();
  if (activated)
  {
    if (action == ui->actModeView)
    {
      ui->actModeEdit->blockSignals(true);
      ui->actModeEdit->trigger();
      ui->actModeEdit->blockSignals(false);
      ui->imageView->changeMode(ImageView::eModeView);
    }
    if (action == ui->actModeEdit)
    {
      ui->actModeView->blockSignals(true);
      ui->actModeView->trigger();
      ui->actModeView->blockSignals(false);
      ui->imageView->changeMode(ImageView::eModeEdit);
    }
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
  if (askForUnsavedChanges(tr("Nouveau document")))
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
  const QString filename(QFileDialog::getSaveFileName(this, tr("Enregistrer sous"), tr("."), tr("Documents (*.xml);;Tous (*)")));
  if (!filename.isEmpty())
  {
    fileName = filename;
    doSave();
  }
}

void MainWindow::doOpen()
{
  if (askForUnsavedChanges(tr("Ouvrir un document")))
  {
    const QString filename(QFileDialog::getOpenFileName(this, tr("Ouvrir un document"), tr("."), tr("Documents (*.xml);;Tous (*)")));
    if (!filename.isEmpty())
    {
      fileName = filename;
      ui->imageView->data().load(fileName);
    }
  }
}
