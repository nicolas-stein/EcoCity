#include "gamelogicthread.h"

GameLogicThread::GameLogicThread()
{
	updateTimer = new QTimer();
	updateTimer->moveToThread(this);

	ressourceManager.loadRessources();
	mapManager.initMap(&ressourceManager);
}

RessourceManager *GameLogicThread::getRessourceManager()
{
	return &ressourceManager;
}

MapManager *GameLogicThread::getMapManager()
{
	return &mapManager;
}

void GameLogicThread::run()
{
	gameDate = QDate::currentDate();
	emit gameDateChanged(gameDate);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGameLogic()));
	updateTimer->start(1000/GAME_UPS);

	exec();
	updateTimer->stop();
}

void GameLogicThread::addResidents(int residents)
{
	QList<ZoneBuilding*> buildings = mapManager.getZoneBuildings();
	int i=0;
	int random;
	while(i<residents){
		if(buildings.size() <= 0){
			return;
		}
		random = QRandomGenerator::global()->generate() % buildings.size();
		if(buildings.at(random)->getZoneType() == Residential){
			if(((ResidentialBuilding*)buildings.at(random))->getResidents() < ((ResidentialBuilding*)buildings.at(random))->getMaxResidents()){
				((ResidentialBuilding*)buildings.at(random))->addResidents(1);
				i++;
			}
			else{
				buildings.removeAt(random);
			}
		}
		else{
			buildings.removeAt(random);
		}
	}
}

void GameLogicThread::setGameSpeed(double newGameSpeed)
{
	gameSpeed = newGameSpeed;
}

void GameLogicThread::updateGameDemands()
{
	int totalResidents = 0;
	int totalResidentialCapacity = 0;
	int totalWorkCapacity = 0;
	int totalCommercialCapacity = 0;

	QList<ZoneBuilding*> buildingList = mapManager.getZoneBuildings();

	for(int i=0;i<buildingList.size();i++){
		if(buildingList.at(i)->getZoneType() == Residential){
			totalResidentialCapacity += ((ResidentialBuilding*)buildingList.at(i))->getMaxResidents();
			totalResidents += ((ResidentialBuilding*)buildingList.at(i))->getResidents();
		}
		else if(buildingList.at(i)->getZoneType() == Commercial){
			totalWorkCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxWorkers();
			totalCommercialCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxClients();
		}
		else if(buildingList.at(i)->getZoneType() == Industrial){
			totalWorkCapacity += ((IndustrialBuilding*)buildingList.at(i))->getMaxWorkers();
		}
	}

	if(totalResidentialCapacity > totalResidents && (double)totalWorkCapacity*1.2 > totalResidents){
		if((double)totalWorkCapacity*1.2 > totalResidentialCapacity){
			addResidents((totalResidentialCapacity-totalResidents)/5+1);
		}
		else{
			addResidents(((double)totalWorkCapacity*1.2 - totalResidents)/5+1);
		}
	}
	double residential = 0;
	double commercial = 0;
	double industrial = 0;

	if(totalWorkCapacity == 0){
		residential = 20;
	}
	else if(totalResidents < totalWorkCapacity*1.2){
		residential = (totalWorkCapacity*1.2-totalResidents)*100/(totalWorkCapacity*1.2);
	}
	else{
		residential = 0;
	}

	if(totalResidents == 0){
		commercial = 20;
	}
	else if(totalCommercialCapacity < totalResidents){
		commercial = (totalResidents-totalCommercialCapacity)*100/totalResidents;
	}
	else{
		commercial = 0;
	}

	if (totalResidentialCapacity == 0){
		industrial = 20;
	}
	else if(totalWorkCapacity < totalResidentialCapacity){
		industrial = (totalResidentialCapacity-totalWorkCapacity)*100/totalResidentialCapacity;
	}

	emit gameDemandsUpdated(residential, totalResidents, commercial, industrial);
}

void GameLogicThread::updateGameLogic()	//Every 1/60 seconds
{
	tickCounter++;
	if(tickCounter >= 60/gameSpeed){
		tickCounter = 0;
	}

	if(tickCounter == 0){ //Every seconds
		//QElapsedTimer timer;
		//timer.start();
		gameDate = gameDate.addDays(1);
		emit gameDateChanged(gameDate);

		if(QRandomGenerator::global()->generate()%100 > 60){
			mapManager.generateNewZoneBuilding(Residential);
		}

		if(QRandomGenerator::global()->generate()%100 > 60){
			mapManager.generateNewZoneBuilding(Commercial);
		}

		if(QRandomGenerator::global()->generate()%100 > 60){
			mapManager.generateNewZoneBuilding(Industrial);
		}

		updateGameDemands();
		//qDebug() << "Time taken : " << timer.elapsed() << "ms";
	}
}
