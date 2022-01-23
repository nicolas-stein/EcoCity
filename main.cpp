#include "mainwindow.h"

#include <QApplication>
#include <QFontDatabase>
#include <QResource>
#include <QMessageBox>
#include <QStringList>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Chargement des ressources
	QStringList ressources_list = {"ressources_graphics.rcc", "ressources_audio.rcc", "ressources_others.rcc"};
	foreach(QString ressource, ressources_list){
		if(!QResource::registerResource(ressource)){
				QMessageBox msgBox(QMessageBox::Critical, "Erreur de chargement", "Erreur lors du chargement des ressources de l'application ("+ressource+") !", QMessageBox::Ok);
				return msgBox.exec();
		}
	}
	QFontDatabase::addApplicationFont(":/fonts/gunplay-rg");

	//Affichage de la fênetre principal et passage de la main à Qt
	MainWindow w;
	w.show();
	int returnCode = a.exec();

	//Déchargement des ressources avant de quitter le programme
	foreach(QString ressource, ressources_list){
		QResource::unregisterResource(ressource);
	}

	return returnCode;
}
