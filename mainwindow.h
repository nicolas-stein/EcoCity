#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QKeyEvent>

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

private slots:
	void startGame();
	void quitApp();
};
#endif // MAINWINDOW_H
