#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "constants.h"
#include "Scenes/mainmenuscene.h"
#include "Scenes/maingamescene.h"
#include "Game/audiomanager.h"

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

	MainMenuScene *mainMenu_graphicsScene;
	MainGameScene *mainGame_graphicsScene = nullptr;

private:
	Ui::MainWindow *ui;

	double gameSpeed = 1;
	int residents = 0;
	long money = MONEY_START;

	AudioManager audioManager;

	QString statusBarMessage;
	QLabel statusBarLabel;

private slots:
	void startGame();
	void quitApp();
	void on_button_place_road_1_clicked(bool checked);
	void on_button_place_road_2_clicked(bool checked);
	void on_button_place_road_3_clicked(bool checked);
	void on_button_remove_road_clicked(bool checked);
	void on_button_place_zone_1_clicked(bool checked);
	void on_button_place_zone_2_clicked(bool checked);
	void on_button_place_zone_3_clicked(bool checked);
	void on_button_remove_zone_clicked(bool checked);
	void on_tabWidget_construction_currentChanged(int index);
	void on_tabWidget_main_currentChanged(int index);

	void on_button_place_power_1_clicked(bool checked);
	void on_button_place_power_2_clicked(bool checked);
	void on_button_remove_power_clicked(bool checked);
	void on_pushButton_gameSpeed_clicked();

	void gameDateChanged(QDate newDate);
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);
	void gamePowerUpdated(double powerProduction, double powerConsumption);
	void gameMoneyUpdated(double money);
	void changeStatusBarMessage(QString newMessage);

};
#endif // MAINWINDOW_H
