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
	//On commence le jeu à la date actuelle
	gameDate = QDate::currentDate();
	emit gameDateChanged(gameDate);
	emit gameMoneyUpdated(money);

	//On défini un timer qui éxecutera la fonction updateGameLogic 60 fois par secondes dans un thread séparé
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

void GameLogicThread::changeResidents(int residents)
{
	//On ajoute (ou supprime) les résidents un par un dans des batiments résidentiels au hasard
	QList<ZoneBuilding*> buildings = mapManager.getZoneBuildings();
	int i=0;
	int random;
	while(i<abs(residents)){
		if(buildings.size() <= 0){
			return;
		}
		random = QRandomGenerator::global()->generate() % buildings.size();
		if(buildings.at(random)->getZoneType() == Grid::Zone::Type::Residential){
			if(residents > 0 && ((ResidentialBuilding*)buildings.at(random))->getResidents() < ((ResidentialBuilding*)buildings.at(random))->getMaxResidents()){
				((ResidentialBuilding*)buildings.at(random))->changeResidents(1);
				i++;
			}
			else if(residents < 0 && (((ResidentialBuilding*)buildings.at(random))->getResidents() > 0)){
				((ResidentialBuilding*)buildings.at(random))->changeResidents(-1);
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
	//On collecte l'argent des impots
	double collectedMoney = 0;

	//Chaque habitant paie des impôts (100€ par mois)
	collectedMoney += totalResidents*100;

	//Chaque batiment de service à un coup de maintenance qu'il faut soustraire à l'argent récolté
	QList<ServiceBuilding*> serviceBuildingList = mapManager.getServiceBuildings();
	for(int i=0;i<serviceBuildingList.size();i++){
		collectedMoney -= serviceBuildingList.at(i)->getOperationalCost();
	}

	//Les routes ont aussi un coup d'entretien mensuel
	collectedMoney -= mapManager.getRoadCount()*200;

	//On change l'argent du joueur
	//(On divise par daysInMonth car l'argent est récupéré jour par jour et non pas à la fin de chaque mois)
	changeMoney(collectedMoney/daysInMonth);
}

void GameLogicThread::setGameSpeed(double newGameSpeed)
{
	gameSpeed = newGameSpeed;
}


/*	ANCIEN ALGORITHME DE MISE A JOUR DE LA DEMANDE
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
			changeResidents((totalResidentialCapacity-totalResidents)/5+1);
		}
		else{
			changeResidents(((double)totalWorkCapacity*1.2 - totalResidents)/5+1);
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
}*/

void GameLogicThread::updateGameDemands()
{
	totalResidents = 0;
	totalResidentialCapacity= 0;

	totalWorkCapacity = 0;
	totalCommercialCapacity = 0;

	// random gausienne entre 0 et 9
	// centrée sur 5

	//  sur 10000 valeur
	//	0: 173
	//	1: 482
	//	2: 916
	//	3: 1503
	//	4: 1892
	//	5: 1934
	//	6: 1514
	//	7: 894
	//	8: 429
	//	9: 142

	quint32 value = QRandomGenerator::global()->generate();

	std::default_random_engine generator (value);
	std::normal_distribution<double> distribution(5.0,2.0);

	int randGaus;

	double number = distribution(generator);
	if ((number>=0.0)&&(number<10.0)) randGaus=int(number);
	else randGaus=0;
	//

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

	bool limiteResidential = 1;

	randGaus += randGaus + totalResidents *((randGaus) * 0.01) ;// on gagne entre 1 et 10 % d'habitant en plus
	randGaus /= 4;

	if(demandCommercial > (85 + totalResidents*0.2)
			|| demandIndustrial > (85 +totalResidents*0.2)
			|| demandEnergy <= 1){//si pas assez de commerce... bloque la population
		limiteResidential = 0;
	}

	if ( (randGaus + totalResidents < totalResidentialCapacity) && (limiteResidential) ){// ajout d'habitants si possible
		changeResidents(randGaus);
		if(demandResidential - randGaus >= 0){
			demandResidential -= randGaus;
		}else{
			demandResidential = 0;
		}
	}else if (limiteResidential && demandResidential < (100 + totalResidents*0.2) ){// sinon demande augmente
		if (demandResidential + randGaus >(100 + totalResidents*0.2) )
			demandResidential = 100 + totalResidents*0.2;
		else
			demandResidential += randGaus;
	}

	if(totalResidents*0.8 - totalWorkCapacity >0){// generation de la demande industrielle
		demandIndustrial = totalResidents*0.8 - totalWorkCapacity;
	}else{
		demandIndustrial = 0;
	}

	if(totalResidents*0.9 - totalCommercialCapacity >0){// generation de la demande commerciale
		demandCommercial = totalResidents*0.9 - totalCommercialCapacity;
	}else{
		demandCommercial = 0;
	}

	if(demandIndustrial > 110 +totalResidents*0.20
			|| demandCommercial > 110 +totalResidents*0.20){// si trop de demande, les habitant partent
		changeResidents(-randGaus/4);
	}

	int repDemRes   =demandResidential * (100/(100+totalResidents*0.2));
	int repDemIndus =demandIndustrial * (100/(85+totalResidents*0.2));
	int repDemComm  =demandCommercial * (100/(85+totalResidents*0.2));

	emit gameDemandsUpdated( repDemRes,
							totalResidents,
							repDemComm,
							repDemIndus);
}

void GameLogicThread::updateBuildingsPower(int *notPoweredBuildings, int *notConnectedToRoadBuildings)
{
	//Mise à jour de l'électricité dans la ville
	*notPoweredBuildings = 0;			//Nombre de batiments non alimentés en électricité
	*notConnectedToRoadBuildings = 0;	//Nombre de batiments non connectés à la route

	QList<ServiceBuilding*> serviceBuildings = mapManager.getServiceBuildings();
	QList<ZoneBuilding*> zoneBuildings = mapManager.getZoneBuildings();

	double totalElectricalProduction = 0;	//Production totale d'électricité

	PowerBuilding *currentPowerBuilding;

	//On parcours tous les batiments d'électricité et on ajoute leur production au total si ils sont connectés à la route
	for(int i=0;i<serviceBuildings.size();i++){
		if(serviceBuildings.value(i)->getServiceType()==Buildings::Service::Type::Power){
			currentPowerBuilding = (PowerBuilding*)serviceBuildings.value(i);
			totalElectricalProduction += currentPowerBuilding->getCurrentPowerProduction();
			if(!currentPowerBuilding->isConnectedToRoad()){
				*notConnectedToRoadBuildings = *notConnectedToRoadBuildings+1;
			}
		}
	}

	//Calcul de la consommation énergetique
	double electricalConsumption = 0;

	//Si il n'y a plus assez d'électricité pour un batiment de zone, on change son status connectedToPower à false, sinon on le met à true
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

	demandEnergy = totalElectricalProduction - electricalConsumption;

	emit gamePowerUpdated(totalElectricalProduction, electricalConsumption);	//On émet le signal (cela permet de mettre à jour les valeurs dans la fênetre principale)
}

void GameLogicThread::updateGamePollution()
{
	//Mise à jour de la pollution
	double pollution = 0;

	//On somme la totalité des pollutions de tous les batiments
	QList<ServiceBuilding*> serviceBuildings = mapManager.getServiceBuildings();
	QList<ZoneBuilding*> zoneBuildings = mapManager.getZoneBuildings();

	for(int i=0;i<serviceBuildings.size();i++){
		pollution += serviceBuildings.at(i)->getPollution();
	}

	for(int i=0;i<zoneBuildings.size();i++){
		pollution += zoneBuildings.at(i)->getPollution();
	}

	emit gamePollutionUpdated(pollution);
}

void GameLogicThread::changeMoney(double amount)
{
	money += amount;
	emit gameMoneyUpdated(money);
}

void GameLogicThread::updateGameLogic()	//Toutes les 1/60 secondes
{
	//Si le jeu est en pause on ne met pas à jour la logique
	if(gameSpeed == 0){
		return;
	}

	if(tickCounter == 0){ //Toutes les secondes
		//QElapsedTimer timer;	//Chronomètre qui permet de mesurer la durée que met la fonction à s'éxecuter afin de constater les performances du programme (en général la fonction met 1ms à s'éxecuter)
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

	//On incrémente le tick
	tickCounter++;
	if(tickCounter >= 60/gameSpeed){
		tickCounter = 0;
	}
}
