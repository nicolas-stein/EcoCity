#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QDate>

#include "Scenes/mainmenuscene.h"
#include "Scenes/maingamescene.h"
#include "Constants.h"


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

	void gameDateChanged(QDate newDate);
	void on_pushButton_gameSpeed_clicked();
};
#endif // MAINWINDOW_H
