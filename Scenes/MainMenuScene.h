#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "constants.h"

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QSlider>

//Classe représentant la scène du menu principal
class MainMenuScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainMenuScene(QObject *parent = nullptr);
	~MainMenuScene();

	void setSlidersValue(int valueMusicSlider, int valueSoundSlider);	//Permet de changer les valeurs des sliders
private:
	QGraphicsPixmapItem titlePixmap;			//Logo du jeu
	QPushButton startButton;					//Bouton démarrer
	QPushButton quitButton;						//Bouton quitter
	QGraphicsPixmapItem musicLogo, soundLogo;	//Logo des sliders d'effets sonore et de musique
	QSlider musicSlider;						//Slider du volume de la musique
	QSlider soundSlider;						//Slider du volume des effets sonores

	QGraphicsProxyWidget *startButtonItem;
	QGraphicsProxyWidget *quitButtonItem;
	QGraphicsProxyWidget *musicSliderItem;
	QGraphicsProxyWidget *soundSliderItem;

	void startButton_on_clicked();				//Clique sur le bouton démarrer
	void quitButton_on_clicked();				//Clique sur le bouton quitter

signals :
	void requestStart();						//Signal indiquant que l'on souhaite démarrer une partie
	void requestQuit();							//Signal indiquant que l'on souhaite quitter le jeu

	void changeSoundEffectVolume(int volume);	//Signal indiquant qu'on souhaite changer le volume des effets sonore
	void changeMusicVolume(int volume);			//Signal indiquant qu'on souhaite changer le volume de la musique

	// QGraphicsScene interface
protected:
	void keyPressEvent(QKeyEvent *event);		//Signal indiquant qu'on a appuyé sur une touche
};

#endif // MAINMENUSCENE_H
