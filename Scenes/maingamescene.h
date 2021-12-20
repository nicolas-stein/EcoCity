#ifndef MAINGAMESCENE_H
#define MAINGAMESCENE_H

#include "Game/gamelogicthread.h"
#include <QGraphicsScene>
#include <QObject>

class MainGameScene : public QGraphicsScene
{
	Q_OBJECT
public:
	explicit MainGameScene(QObject *parent = nullptr);

	void startGameLogic();

private:
	GameLogicThread gameLogicThread;
};

#endif // MAINGAMESCENE_H
