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
	~GameLogicThread();

	RessourceManager *getRessourceManager();
	MapManager *getMapManager();

	void setGameSpeed(double newGameSpeed);
	void changeMoney(double amount);
	double getMoney() const;

protected:
	void run();

private:	
	QTimer *updateTimer;

	RessourceManager ressourceManager;
	MapManager mapManager;

	int tickCounter = 0;
	QDate gameDate;
	double gameSpeed = 1;
	double money = MONEY_START;

	void updateGameDemands();
	void updateBuildingsPower(int *notPoweredBuildings, int *notConnectedToRoadBuildings);
	void addResidents(int residents);
	void collectMoney();

private slots:
	void updateGameLogic();

signals:
	void gameDateChanged(QDate newDate);
	void gameMoneyUpdated(double money);
	void gameDemandsUpdated(double residential, int residents, double commercial, double industrial);
	void gamePowerUpdated(double powerProduction, double powerConsumption);
	void changeStatusBarMessage(QString newMessage);
};

#endif // GAMELOGICTHREAD_H
