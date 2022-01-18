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

	musicSlider.setMaximum(100);
	musicSlider.setMinimum(0);
	musicSlider.setOrientation(Qt::Horizontal);
	musicSlider.setStyleSheet("QSlider{background: #525252}"
							  "QSlider::groove:horizontal {border: 1px solid #999999;height: 6px;background: white;margin: 2px 0;}"
							  "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e8e8e8, stop:1 #b8b8b8);border: 1px solid #5c5c5c;width: 18px;margin: -8px 0;border-radius: 2px;}");
	soundSlider.setMaximum(100);
	soundSlider.setMinimum(0);
	soundSlider.setOrientation(Qt::Horizontal);
	soundSlider.setStyleSheet("QSlider{background: #525252}"
							  "QSlider::groove:horizontal {border: 1px solid #999999;height: 6px;background: white;margin: 2px 0;}"
							  "QSlider::handle:horizontal {background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e8e8e8, stop:1 #b8b8b8);border: 1px solid #5c5c5c;width: 18px;margin: -8px 0;border-radius: 2px;}");

	QFont startButtonFont = QFont("Gunplay", 36, QFont::Normal, false);
	startButton.setFont(startButtonFont);
	startButton.setStyleSheet("padding: 16px;");

	QFont quitButtonFont = QFont("Gunplay", 30, QFont::Normal, false);
	quitButton.setFont(quitButtonFont);
	quitButton.setStyleSheet("padding: 12px;");


	startButtonItem = addWidget(&startButton);
	quitButtonItem = addWidget(&quitButton);

	musicSliderItem = addWidget(&musicSlider);
	soundSliderItem = addWidget(&soundSlider);

	startButtonItem->setPos(width()/2 - startButton.width()/2, height()*5/8 - startButton.height()/2);
	quitButtonItem->setPos(width()/2 - quitButton.width()/2, height()*5/6 - quitButton.height()/2);

	musicSliderItem->setPos(width() - musicSlider.width() - 16, height() - musicSlider.height() - 16);
	soundSliderItem->setPos(musicSliderItem->pos().x(), musicSliderItem->pos().y() - musicSlider.height() - 16);

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
