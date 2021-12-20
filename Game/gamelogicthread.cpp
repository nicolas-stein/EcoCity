#include "gamelogicthread.h"

GameLogicThread::GameLogicThread()
{
	updateTimer.moveToThread(this);
}

void GameLogicThread::run()
{
	connect(&updateTimer, SIGNAL(timeout()), this, SLOT(updateGameLogic()));
	updateTimer.start(1000/GAME_UPS);

	exec();
}

void GameLogicThread::updateGameLogic()
{

}
