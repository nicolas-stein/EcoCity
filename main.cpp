#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFontDatabase::addApplicationFont(":/fonts/gunplay-rg");
	MainWindow w;
	w.show();
	return a.exec();
}
