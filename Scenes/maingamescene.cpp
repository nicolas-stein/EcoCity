#include "maingamescene.h"

MainGameScene::MainGameScene(QObject *parent) : QGraphicsScene{parent}
{

}

void MainGameScene::startGameLogic()
{
	gameLogicThread.start();
}
