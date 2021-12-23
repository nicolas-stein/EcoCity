#ifndef MAINMENUSCENE_H
#define MAINMENUSCENE_H

#include "Constants.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QFont>
#include <QBrush>
#include <QColor>

class MainMenuScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainMenuScene(QObject *parent = nullptr);

private:
	QGraphicsPixmapItem titlePixmap;
	QPushButton startButton;
	QPushButton quitButton;

	QGraphicsProxyWidget *startButtonItem;
	QGraphicsProxyWidget *quitButtonItem;

	void startButton_on_clicked();
	void quitButton_on_clicked();

signals :
	void requestStart();
	void requestQuit();

	// QGraphicsScene interface
protected:
	void keyPressEvent(QKeyEvent *event);
};

#endif // MAINMENUSCENE_H
