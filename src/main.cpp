#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  App.setOrganizationName("PLab");
  App.setOrganizationDomain("labedan.fr");
  App.setApplicationName("BioTool1");

  MainWindow Win;
  Win.show();
  return App.exec();
}
