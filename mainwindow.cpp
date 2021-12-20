#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setMinimumSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	setMaximumSize(WINDOW_WIDTH, WINDOW_HEIGHT);


	mainMenu_graphicsScene = new MainMenuScene(this);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestStart, this, &MainWindow::startGame);
	connect(mainMenu_graphicsScene, &MainMenuScene::requestQuit, this, &MainWindow::quitApp);

	ui->graphicsView->setScene(mainMenu_graphicsScene);
}

MainWindow::~MainWindow()
{
	delete ui;
	delete mainMenu_graphicsScene;
	if(mainGame_graphicsScene!=nullptr){
		delete mainGame_graphicsScene;
	}
}

void MainWindow::startGame()
{
	if(ui->graphicsView->scene() == mainMenu_graphicsScene && mainGame_graphicsScene == nullptr){
		mainGame_graphicsScene = new MainGameScene(this);
		ui->graphicsView->setScene(mainGame_graphicsScene);
		mainGame_graphicsScene->startGameLogic();
	}
}


void MainWindow::quitApp(){
	qApp->quit();
}
