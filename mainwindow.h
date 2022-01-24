#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "constants.h"
#include "Scenes/mainmenuscene.h"
#include "Scenes/maingamescene.h"
#include "Game/audiomanager.h"

#include <QMainWindow>
#include <QLabel>
#include <QSettings>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//Classe représentant la fênetre principale
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	MainMenuScene *mainMenu_graphicsScene;			//Scène du menu principal
	MainGameScene *mainGame_graphicsScene = nullptr;//Scène du jeu

private:
	Ui::MainWindow *ui;

	double gameSpeed = 1;			//Vitesse du jeu
	int residents = 0;				//Nombre de résidents
	double money = 150e3;			//Argent du joueur

	AudioManager audioManager;		//Gestionnaire audio

	QString statusBarMessage;		//Message de la barre d'état
	QLabel statusBarLabel;			//Label de la barre d'état

	QSettings *settings;			//Paramètres du programme

	void set_all_power_buttons_enabled(bool enabled);	//Active ou non tous les boutons de l'onglet Electricité
private slots:
	void startGame();									//Lancer le jeu
	void quitApp();										//Quitter le jeu
	void on_button_place_road_1_clicked(bool checked);	//Clique sur le bouton placer une route n°1 (le plus à gauche)
	void on_button_place_road_2_clicked(bool checked);
	void on_button_place_road_3_clicked(bool checked);
	void on_button_remove_road_clicked(bool checked);	//Clique sur le bouton détruire une route
	void on_button_place_zone_1_clicked(bool checked);	//Clique sur le bouton placer une zone n°1 (le plus à gauche)
	void on_button_place_zone_2_clicked(bool checked);
	void on_button_place_zone_3_clicked(bool checked);
	void on_button_remove_zone_clicked(bool checked);	//Clique sur le bouton supprimer une zone
	void on_tabWidget_construction_currentChanged(int index);
	void on_tabWidget_main_currentChanged(int index);	//Changement d'onglet principal

	void on_button_place_power_1_clicked(bool checked);	//Clique sur le bouton placer un batiment d'électricité n°1 (le plus à gauche)
	void on_button_place_power_2_clicked(bool checked);
	void on_button_place_power_3_clicked(bool checked);
	void on_button_place_power_4_clicked(bool checked);
	void on_button_place_power_5_clicked(bool checked);
	void on_button_place_power_6_clicked(bool checked);
	void on_button_place_power_7_clicked(bool checked);
	void on_button_remove_power_clicked(bool checked);	//Clique sur le bouton supprimer un batiment
	void on_pushButton_gameSpeed_clicked();				//Clique sur le bouton de vitesse du jeu

	void changeSoundEffectVolume(int volume);			//Changement du volume des effets sonores
	void changeMusicVolume(int volume);					//Changement du volume de la musique

	void gameDateChanged(QDate newDate);				//La date du jeu a changé
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);	//La demande du jeu a été mise à jour
	void gamePowerUpdated(double powerProduction, double powerConsumption);	//L'électricité de la ville a été mise à jour
	void gameMoneyUpdated(double money);	//L'argent du joueur a été mis à jour
	void gamePollutionUpdated(int pollution);	//La pollution a été mise à jour
	void changeStatusBarMessage(QString newMessage);	//Le message de la barre d'état doit être mis à jour

};
#endif // MAINWINDOW_H
