#include "gamelogicthread.h"

GameLogicThread::GameLogicThread(QObject *parent): QThread{parent}
{
	moveToThread(this);

	ressourceManager.loadRessources();
	mapManager.initMap(&ressourceManager);
}

GameLogicThread::~GameLogicThread()
{
	delete updateTimer;
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
	emit gameMoneyUpdated(money);
	updateTimer = new QTimer(this);
	connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateGameLogic()));
	updateTimer->start(1000/GAME_UPS);

	exec();
	updateTimer->stop();
}

double GameLogicThread::getMoney() const
{
	return money;
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
		if(buildings.at(random)->getZoneType() == Grid::Zone::Type::Residential){
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

void GameLogicThread::collectMoney(int daysInMonth)
{
	QList<ZoneBuilding*> zoneBuildingList = mapManager.getZoneBuildings();
	double collectedMoney = 0;
	for(int i=0;i<zoneBuildingList.size();i++){
		if(zoneBuildingList.at(i)->getZoneType()==Grid::Zone::Type::Residential && zoneBuildingList.at(i)->isConnectedToPower()){
			collectedMoney += ((ResidentialBuilding*)zoneBuildingList.at(i))->getResidents()*100;
		}
	}

	QList<ServiceBuilding*> serviceBuildingList = mapManager.getServiceBuildings();
	for(int i=0;i<serviceBuildingList.size();i++){
		collectedMoney -= serviceBuildingList.at(i)->getOperationalCost();
	}

	collectedMoney -= mapManager.getRoadCount()*200;

	changeMoney(collectedMoney/daysInMonth);
}

void GameLogicThread::setGameSpeed(double newGameSpeed)
{
	gameSpeed = newGameSpeed;
}

void GameLogicThread::updateGameDemands()
{
	totalResidentialCapacity = 0;
	totalResidents = 0;
	totalWorkCapacity = 0;
	totalCommercialCapacity = 0;
	QList<ZoneBuilding*> buildingList = mapManager.getZoneBuildings();

	for(int i=0;i<buildingList.size();i++){
		if(buildingList.at(i)->getZoneType() == Grid::Zone::Type::Residential){
			totalResidentialCapacity += ((ResidentialBuilding*)buildingList.at(i))->getMaxResidents();
			totalResidents += ((ResidentialBuilding*)buildingList.at(i))->getResidents();
		}
		else if(buildingList.at(i)->getZoneType() == Grid::Zone::Type::Commercial){
			totalWorkCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxWorkers();
			totalCommercialCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxClients();
		}
		else if(buildingList.at(i)->getZoneType() == Grid::Zone::Type::Industrial){
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

void GameLogicThread::updateBuildingsPower(int *notPoweredBuildings, int *notConnectedToRoadBuildings)
{
	*notPoweredBuildings = 0;
	*notConnectedToRoadBuildings = 0;

	QList<ServiceBuilding*> serviceBuildings = mapManager.getServiceBuildings();
	QList<ZoneBuilding*> zoneBuildings = mapManager.getZoneBuildings();

	double totalElectricalProduction = 0;

	PowerBuilding *currentPowerBuilding;

	for(int i=0;i<serviceBuildings.size();i++){
		if(serviceBuildings.value(i)->getServiceType()==Buildings::Service::Type::Power){
			currentPowerBuilding = (PowerBuilding*)serviceBuildings.value(i);
			totalElectricalProduction += currentPowerBuilding->getCurrentPowerProduction();
			if(!currentPowerBuilding->isConnectedToRoad()){
				*notConnectedToRoadBuildings = *notConnectedToRoadBuildings+1;
			}
		}
	}
	double electricalConsumption = 0;

	for(int i=0;i<zoneBuildings.size();i++){
		if(totalElectricalProduction-electricalConsumption > zoneBuildings.at(i)->getPowerConsumption()){
			zoneBuildings.at(i)->setConnectedToPower(true);
			electricalConsumption += zoneBuildings.value(i)->getPowerConsumption();
		}
		else{
			zoneBuildings.at(i)->setConnectedToPower(false);
			*notPoweredBuildings = *notPoweredBuildings+1;
		}
	}

	emit gamePowerUpdated(totalElectricalProduction, electricalConsumption);
}

void GameLogicThread::updateGamePollution()
{
	double pollution = 0;

	QList<ServiceBuilding*> serviceBuildings = mapManager.getServiceBuildings();
	QList<ZoneBuilding*> zoneBuildings = mapManager.getZoneBuildings();

	for(int i=0;i<serviceBuildings.size();i++){
		pollution += serviceBuildings.at(i)->getPollution();
	}

	for(int i=0;i<zoneBuildings.size();i++){
		pollution += zoneBuildings.at(i)->getPollution();
	}

	if(pollution>totalResidents*15){
		pollution = totalResidents*15;
	}

	emit gamePollutionUpdated(pollution, totalResidents*15);
}

void GameLogicThread::changeMoney(double amount)
{
	money += amount;
	emit gameMoneyUpdated(money);
}

void GameLogicThread::updateGameLogic()	//Every 1/60 seconds
{
	if(gameSpeed == 0){
		return;
	}

	if(tickCounter == 0){ //Every seconds
		//QElapsedTimer timer;
		//timer.start();

		//Game date update
		gameDate = gameDate.addDays(1);
		emit gameDateChanged(gameDate);

		collectMoney(gameDate.daysInMonth());

		//Generating new zone buildings
		if(QRandomGenerator::global()->generate()%100 > 40){
			mapManager.generateNewZoneBuilding(Grid::Zone::Type::Residential);
		}

		if(QRandomGenerator::global()->generate()%100 > 40){
			mapManager.generateNewZoneBuilding(Grid::Zone::Type::Commercial);
		}

		if(QRandomGenerator::global()->generate()%100 > 20){
			mapManager.generateNewZoneBuilding(Grid::Zone::Type::Industrial);
		}

		QString statusBarMessage = "";

		//Updating game demands
		updateGameDemands();

		//Updating buildings power
		int notPoweredBuildings, notConnectedToRoadBuildings;
		updateBuildingsPower(&notPoweredBuildings, &notConnectedToRoadBuildings);
		if(notConnectedToRoadBuildings > 1){
			statusBarMessage += "<strong><font color=orange>"+QString::number(notConnectedToRoadBuildings)+" batiments ne sont pas connectés à la route !</font></strong> ";
		}
		else if(notConnectedToRoadBuildings > 0){
			statusBarMessage += "<strong><font color=orange>"+QString::number(notConnectedToRoadBuildings)+" batiment n'est pas connecté à la route !</font></strong> ";
		}

		if(notPoweredBuildings > 1){
			statusBarMessage += "<strong><font color=orange>"+QString::number(notPoweredBuildings)+"  batiments ne sont pas alimentés en électricité !</font></strong> ";
		}
		else if(notPoweredBuildings > 0){
			statusBarMessage += "<strong><font color=orange>"+QString::number(notPoweredBuildings)+" batiment n'est pas alimenté en électricité !</font></strong> ";
		}

		updateGamePollution();

		emit changeStatusBarMessage(statusBarMessage);

		//qDebug() << "Time taken : " << timer.elapsed() << "ms";
	}

	tickCounter++;
	if(tickCounter >= 60/gameSpeed){
		tickCounter = 0;
	}
}
