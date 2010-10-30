  #include "mainwindow.h"
#include "ui_mainwindow.h"

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

