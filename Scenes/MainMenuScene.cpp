#include "mainmenuscene.h"

MainMenuScene::MainMenuScene(QObject *parent) : QGraphicsScene{parent}
{
	setSceneRect(0, 0, 800, 600);
	setBackgroundBrush(QBrush(QColor(COLOR_MAIN_MENU_BG)));

	QPixmap pixmap(":/pixmaps/mainMenuLogo");
	pixmap = pixmap.scaled(300, 300);
	titlePixmap.setPixmap(pixmap);
	titlePixmap.setPos(width()/2 - pixmap.width()/2, 0);
	addItem(&titlePixmap);

	startButton.setText("Démarrer une partie");
	quitButton.setText("Quitter");

	QFont startButtonFont = QFont("Gunplay", 36, QFont::Normal, false);
	startButton.setFont(startButtonFont);
	startButton.setStyleSheet("padding: 16px;");

	QFont quitButtonFont = QFont("Gunplay", 30, QFont::Normal, false);
	quitButton.setFont(quitButtonFont);
	quitButton.setStyleSheet("padding: 12px;");


	startButtonItem = addWidget(&startButton);
	quitButtonItem = addWidget(&quitButton);

	startButtonItem->setPos(width()/2 - startButton.width()/2, height()*5/8 - startButton.height()/2);
	quitButtonItem->setPos(width()/2 - quitButton.width()/2, height()*5/6 - quitButton.height()/2);

	connect(&startButton, &QPushButton::clicked, this, &MainMenuScene::startButton_on_clicked);
	connect(&quitButton, &QPushButton::clicked, this, &MainMenuScene::quitButton_on_clicked);
}

void MainMenuScene::startButton_on_clicked()
{
	emit requestStart();
}

void MainMenuScene::quitButton_on_clicked()
{
	emit requestQuit();
}

void MainMenuScene::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key::Key_Escape){
		emit requestQuit();
	}
}
