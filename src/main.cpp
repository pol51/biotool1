#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
  QApplication App(argc, argv);
  App.setOrganizationName("PLab");
  App.setOrganizationDomain("labedan.fr");
  App.setApplicationName("BioTool1");

  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

  MainWindow Win;
  Win.show();
  return App.exec();
}
