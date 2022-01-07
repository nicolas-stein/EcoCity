#ifndef GAMELOGICTHREAD_H
#define GAMELOGICTHREAD_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "Game/mapmanager.h"
#include "Game/Building/ZoneBuilding/residentialbuilding.h"
#include "Game/Building/ZoneBuilding/commercialbuilding.h"
#include "Game/Building/ZoneBuilding/industrialbuilding.h"

#include <QThread>
#include <QTimer>
#include <QDate>

class GameLogicThread : public QThread
{
	Q_OBJECT
public:
	GameLogicThread();

	RessourceManager *getRessourceManager();
	MapManager *getMapManager();

	void setGameSpeed(double newGameSpeed);
	void updateGameDemands();

protected:
	void run();

private:
	QTimer *updateTimer;

	RessourceManager ressourceManager;
	MapManager mapManager;

	int tickCounter = 0;
	QDate gameDate;
	double gameSpeed = 1;

	void addResidents(int residents);

private slots:
	void updateGameLogic();

signals:
	void gameDateChanged(QDate newDate);
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);
};

#endif // GAMELOGICTHREAD_H
