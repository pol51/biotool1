#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  App.setOrganizationName("PLab");
  App.setOrganizationDomain("labedan.fr");
  App.setApplicationName("BioTool1");

  App.setFont(QFont("DejaVu Sans", 10));
  App.setStyle(QStyleFactory::create("Fusion"));

  MainWindow Win;
  Win.show();
  return App.exec();
}
