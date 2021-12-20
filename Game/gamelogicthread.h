#ifndef GAMELOGICTHREAD_H
#define GAMELOGICTHREAD_H

#include "Constants.h"
#include <QTimer>
#include <QThread>
#include <QDebug>

class GameLogicThread : public QThread
{
	Q_OBJECT
public:
	GameLogicThread();

	// QThread interface
protected:
	void run();

private:
	QTimer updateTimer;

private slots:
	void updateGameLogic();
};

#endif // GAMELOGICTHREAD_H
