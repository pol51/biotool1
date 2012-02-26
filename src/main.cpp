#include <QtGui/QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  a.setOrganizationName("PLab");
  a.setOrganizationDomain("labedan.fr");
  a.setApplicationName("BioTool1");

  MainWindow w;
  w.show();
  return a.exec();
}
