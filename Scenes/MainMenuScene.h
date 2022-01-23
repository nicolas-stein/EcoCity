#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "constants.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QSlider>

class MainMenuScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainMenuScene(QObject *parent = nullptr);
	~MainMenuScene();

	void setSlidersValue(int valueMusicSlider, int valueSoundSlider);
private:
	QGraphicsPixmapItem titlePixmap;
	QPushButton startButton;
	QPushButton quitButton;
	QGraphicsPixmapItem musicLogo, soundLogo;
	QSlider musicSlider;
	QSlider soundSlider;

	QGraphicsProxyWidget *startButtonItem;
	QGraphicsProxyWidget *quitButtonItem;
	QGraphicsProxyWidget *musicSliderItem;
	QGraphicsProxyWidget *soundSliderItem;

	void startButton_on_clicked();
	void quitButton_on_clicked();

signals :
	void requestStart();
	void requestQuit();

	void changeSoundEffectVolume(int volume);
	void changeMusicVolume(int volume);

	// QGraphicsScene interface
protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif // MAINMENUSCENE_H
