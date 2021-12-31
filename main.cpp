#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QResource>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	/*if(!QResource::registerResource("ressources.rcc")){
		QMessageBox msgBox(QMessageBox::Critical, "Erreur de chargement", "Erreur lors du chargement des ressources de l'application (ressources.rcc) !", QMessageBox::Ok);
		return msgBox.exec();
	}*/

	QFontDatabase::addApplicationFont(":/fonts/gunplay-rg");
	MainWindow w;
	w.show();
	return a.exec();
}
