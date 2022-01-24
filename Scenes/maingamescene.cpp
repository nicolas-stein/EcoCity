#include "maingamescene.h"

MainGameScene::MainGameScene(QObject *parent) : QGraphicsScene{parent}
{
	//On ajoute les pixmapItem des terrains à la scène
	TerrainSquare ***terrainGrid = gameLogicThread.getMapManager()->getTerrainGrid();
	terrainPixmapItems = new CustomGraphicsPixmapItem**[TERRAIN_GRID_SIZE];
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		terrainPixmapItems[x] = new CustomGraphicsPixmapItem*[TERRAIN_GRID_SIZE];
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			terrainPixmapItems[x][y] = terrainGrid[x][y]->getPixmapItem();
			terrainPixmapItems[x][y]->mainThreadConnected();
			terrainPixmapItemGroup.addToGroup(terrainPixmapItems[x][y]);
		}
	}
	addItem(&terrainPixmapItemGroup);

	roadPixmapItems = new CustomGraphicsPixmapItem**[ROAD_GRID_SIZE];
	for(int x=0;x<ROAD_GRID_SIZE;x++){
		roadPixmapItems[x] = new CustomGraphicsPixmapItem*[ROAD_GRID_SIZE];
		for(int y=0;y<ROAD_GRID_SIZE;y++){
			roadPixmapItems[x][y] = nullptr;
		}
	}

	zonePixmapItems = new CustomGraphicsPixmapItem**[ZONE_GRID_SIZE];
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		zonePixmapItems[x] = new CustomGraphicsPixmapItem*[ZONE_GRID_SIZE];
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			zonePixmapItems[x][y] = nullptr;
		}
	}

	//On passe au buildCursor les gestionnaires
	buildCursor.setManagers(gameLogicThread.getRessourceManager(), gameLogicThread.getMapManager());

	//On connecte les signaux et les slots
	connect(gameLogicThread.getMapManager(), SIGNAL(zoneSquareCreated(ZoneSquare*)), this, SLOT(zoneSquareCreated(ZoneSquare*)));
	connect(gameLogicThread.getMapManager(), SIGNAL(zoneSquareRemoved(ZoneSquare*)), this, SLOT(zoneSquareRemoved(ZoneSquare*)));
	connect(gameLogicThread.getMapManager(), SIGNAL(buildingCreated(Building*)), this, SLOT(buildingCreated(Building*)));
	connect(gameLogicThread.getMapManager(), SIGNAL(buildingRemoved(Building*)), this, SLOT(buildingRemoved(Building*)));

	connect(&gameLogicThread, SIGNAL(gameDateChanged(QDate)), this, SIGNAL(gameDateChanged(QDate)));
	connect(&gameLogicThread, SIGNAL(gameMoneyUpdated(double)), this, SIGNAL(gameMoneyUpdated(double)));
	connect(&gameLogicThread, SIGNAL(gameDemandsUpdated(double,int,double,double)), this, SIGNAL(gameDemandsUpdated(double,int,double,double)));
	connect(&gameLogicThread, SIGNAL(gamePowerUpdated(double,double)), this, SIGNAL(gamePowerUpdated(double,double)));
	connect(&gameLogicThread, SIGNAL(gamePollutionUpdated(int)), this, SIGNAL(gamePollutionUpdated(int)));
	connect(&gameLogicThread, SIGNAL(changeStatusBarMessage(QString)), this, SIGNAL(changeStatusBarMessage(QString)));
}

MainGameScene::~MainGameScene()
{
	//On supprime tous les pixmapItem alloué dynamiquement
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			removeItem(terrainPixmapItems[x][y]);
		}
		delete[] terrainPixmapItems[x];
	}
	delete[] terrainPixmapItems;

	for(int x=0;x<ROAD_GRID_SIZE;x++){
		for(int y=0;y<ROAD_GRID_SIZE;y++){
			if(roadPixmapItems[x][y] != nullptr){
				removeItem(roadPixmapItems[x][y]);
			}
		}
		delete[] roadPixmapItems[x];
	}
	delete[] roadPixmapItems;

	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zonePixmapItems[x][y] != nullptr){
				removeItem(zonePixmapItems[x][y]);
			}
		}
		delete[] zonePixmapItems[x];
	}
	delete[] zonePixmapItems;

	//On arrête le thread qui gère la logique du jeu
	gameLogicThread.quit();
	gameLogicThread.wait(2000);
}

void MainGameScene::startGameLogic()
{
	gameLogicThread.start();
}

void MainGameScene::enableBuildMode(Grid::Road::Type roadType)
{
	//On active le curseur de construction avec une route
	buildCursor.setSquareToBuild(new RoadSquare(0, 0, roadType, gameLogicThread.getRessourceManager()));
	addItem(&buildCursor);
	connect(&buildCursor, SIGNAL(requestBuildSquare(GridSquare*)), this, SLOT(requestBuildSquare(GridSquare*)));
	buildingMode = true;
}

void MainGameScene::enableBuildMode(Grid::Zone::Type zoneType)
{
	//On active le curseur de construction avec une zone
	buildCursor.setSquareToBuild(new ZoneSquare(0, 0, zoneType, gameLogicThread.getRessourceManager()));
	addItem(&buildCursor);
	connect(&buildCursor, SIGNAL(changeZoneSquareType(ZoneSquare*,bool)), this, SLOT(changeZoneSquareType(ZoneSquare*,bool)));
	buildingMode = true;
}

void MainGameScene::enableBuildMode(Buildings::Service::PowerType powerType)
{
	//On active le curseur de construction avec un batiment de service
	buildCursor.setBuildingToBuild(new PowerBuilding(0, 0, gameLogicThread.getRessourceManager(), powerType));
	addItem(&buildCursor);
	connect(&buildCursor, SIGNAL(requestBuildBuilding(Building*)), this, SLOT(requestBuildBuilding(Building*)));
	buildingMode = true;
}

void MainGameScene::disableBuildMode()
{
	//On désactive le curseur de construction
	buildingMode = false;
	removeItem(&buildCursor);
	buildCursor.disconnect();
}

void MainGameScene::enableDestroyMode(Grid::Type gridType)
{
	//On active le mode de destruction
	destroyMode = true;
	destroyGridType = gridType;
}

void MainGameScene::disableDestroyMode()
{
	//On désactive le mode de destruction
	destroyMode = false;
}

void MainGameScene::showZones(bool enabled)
{
	//On affiche (ou masque) toutes les zones

	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zonePixmapItems[x][y] != nullptr){
				zonePixmapItems[x][y]->setVisible(enabled);
			}
		}
	}

	//On fait l'opposé sur les batiments de zone (pour éviter que le jeu ne rame trop en affichant en même temps les batiments de zone et les zones)
	QList<ZoneBuilding*> zoneBuildings = gameLogicThread.getMapManager()->getZoneBuildings();
	foreach(ZoneBuilding *zoneBuilding, zoneBuildings){
		zoneBuilding->getPixmapItem()->setVisible(!enabled);
	}
}

void MainGameScene::setGameSpeed(double newGameSpeed)
{
	gameLogicThread.setGameSpeed(newGameSpeed);
}

void MainGameScene::requestBuildSquare(GridSquare *gridSquare)
{
	//On demande au gestionnaire de carte de créer un carré
	GridSquare *result = gameLogicThread.getMapManager()->requestBuildSquare(gridSquare, gameLogicThread.getMoney());
	if(result != nullptr){
		//Si il a réussi à créer un carré, on l'ajoute au tableau et à la scène
		connect(result->getPixmapItem(), SIGNAL(requestPosChange(qreal,qreal)), this, SLOT(pixmapRequestedPosChange(qreal,qreal)));
		connect(result->getPixmapItem(), SIGNAL(requestPixmapChange(QPixmap)), this, SLOT(pixmapRequestChangePixmap(QPixmap)));
		result->getPixmapItem()->mainThreadConnected();

		//Si c'est une route, on soustrait le cout de la route et on joue un son
		if(result->getGridType()==Grid::Type::GridRoad){
			roadPixmapItems[result->getPosX()/ROAD_SQUARE_SIZE][result->getPosY()/ROAD_SQUARE_SIZE] = result->getPixmapItem();
			addItem(result->getPixmapItem());
			gameLogicThread.changeMoney(-((RoadSquare*)result)->getCost());
			emit playSoundEffect(RoadPlacement);
		}
	}
}

void MainGameScene::requestBuildBuilding(Building *building)
{
	//On demande au gestionnaire de carte de créer batiment
	Building *result = gameLogicThread.getMapManager()->requestBuildBuilding(building, gameLogicThread.getMoney());
	if(result != nullptr){
		//Si il a réussi à créer un batiment, on l'ajoute au tableau et à la scène
		connect(result->getPixmapItem(), SIGNAL(requestPosChange(qreal,qreal)), this, SLOT(pixmapRequestedPosChange(qreal,qreal)));
		connect(result->getPixmapItem(), SIGNAL(requestPixmapChange(QPixmap)), this, SLOT(pixmapRequestChangePixmap(QPixmap)));
		result->getPixmapItem()->mainThreadConnected();
		buildingList.append(result->getPixmapItem());
		addItem(result->getPixmapItem());

		//Si c'est un batiment de service, on soustrait le cout du batiment et on joue un son
		if(result->getBuildingType() == Buildings::Type::BuildingService){
			ServiceBuilding *serviceBuilding = (ServiceBuilding*)result;
			gameLogicThread.changeMoney(-serviceBuilding->getCost());
			emit playSoundEffect(BuildingPlacement);
		}
	}
}

void MainGameScene::zoneSquareCreated(ZoneSquare *zoneSquare)
{
	//Si le gestionnaire de carte a créé un carré de zone, on l'ajoute au tableau et à la scène
	connect(zoneSquare->getPixmapItem(), SIGNAL(requestPosChange(qreal,qreal)), this, SLOT(pixmapRequestedPosChange(qreal,qreal)));
	connect(zoneSquare->getPixmapItem(), SIGNAL(requestPixmapChange(QPixmap)), this, SLOT(pixmapRequestChangePixmap(QPixmap)));
	zoneSquare->getPixmapItem()->mainThreadConnected();
	zoneSquare->setPos(zoneSquare->getPosX(), zoneSquare->getPosY());
	int posX = zoneSquare->getPosX()/ZONE_SQUARE_SIZE;
	int posY = zoneSquare->getPosY()/ZONE_SQUARE_SIZE;
	zonePixmapItems[posX][posY] = zoneSquare->getPixmapItem();
	addItem(zoneSquare->getPixmapItem());
}

void MainGameScene::zoneSquareRemoved(ZoneSquare *zoneSquare)
{
	//Si le gestionnaire de carte a supprimé un carré de zone, on le supprime du tableau et de la scène
	int posX = zoneSquare->getPosX()/ZONE_SQUARE_SIZE;
	int posY = zoneSquare->getPosY()/ZONE_SQUARE_SIZE;
	if(zonePixmapItems[posX][posY]!=nullptr){
		removeItem(zonePixmapItems[posX][posY]);
		zonePixmapItems[posX][posY] = nullptr;
	}
	//Si il y avait un batiment sur le carré de zone, on supprime également le batiment de la liste des batiments et de la scène
	if(zoneSquare->getBuilding()!=nullptr && buildingList.contains(zoneSquare->getBuilding()->getPixmapItem())){
		removeItem(zoneSquare->getBuilding()->getPixmapItem());
		buildingList.removeAll(zoneSquare->getBuilding()->getPixmapItem());
	}
}

void MainGameScene::changeZoneSquareType(ZoneSquare *zoneSquare, bool wholeArea)
{
	//On change le type de zone (wholeArea correspond à un maintient sur Shift lors du clique pour changer la zone, cela permet de changer toutes les zones aux alentours)
	if(zoneSquare != nullptr && gameLogicThread.getMapManager()->getZoneGrid()[zoneSquare->getPosX()/ZONE_SQUARE_SIZE][zoneSquare->getPosY()/ZONE_SQUARE_SIZE]!=nullptr
			&& (gameLogicThread.getMapManager()->getBuildingFromPos(zoneSquare->getPosX(), zoneSquare->getPosY()) == nullptr || gameLogicThread.getMapManager()->getBuildingFromPos(zoneSquare->getPosX(), zoneSquare->getPosY())->getBuildingType() == Buildings::Type::BuildingZone)){
		ZoneSquare *oldSquare = gameLogicThread.getMapManager()->getZoneGrid()[zoneSquare->getPosX()/ZONE_SQUARE_SIZE][zoneSquare->getPosY()/ZONE_SQUARE_SIZE];
		if(oldSquare->getZoneType() != zoneSquare->getZoneType() && (!wholeArea || oldSquare->getZoneType() == Grid::Zone::Type::None || zoneSquare->getZoneType() == Grid::Zone::Type::None)){
			Grid::Zone::Type originalZoneType = oldSquare->getZoneType();
			oldSquare->setZoneType(zoneSquare->getZoneType());
			if(oldSquare->getBuilding()!=nullptr){
				buildingRemoved(oldSquare->getBuilding());
				gameLogicThread.getMapManager()->requestDestroyBuilding(oldSquare->getBuilding());
			}
			emit playSoundEffect(ZonePlacement);
			if(wholeArea){
				int x = zoneSquare->getPosX();
				int y = zoneSquare->getPosY();
				if(x > 0 && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE-1][y/ZONE_SQUARE_SIZE]!=nullptr && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE-1][y/ZONE_SQUARE_SIZE]->getZoneType() == originalZoneType){
					zoneSquare->setPos(x-ZONE_SQUARE_SIZE, y);
					changeZoneSquareType(zoneSquare, true);
				}
				if(x < (ZONE_GRID_SIZE-1)*ZONE_SQUARE_SIZE && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE+1][y/ZONE_SQUARE_SIZE]!=nullptr && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE+1][y/ZONE_SQUARE_SIZE]->getZoneType() == originalZoneType){
					zoneSquare->setPos(x+ZONE_SQUARE_SIZE, y);
					changeZoneSquareType(zoneSquare, true);
				}
				if(y > 0 && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE-1]!=nullptr && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE-1]->getZoneType() == originalZoneType){
					zoneSquare->setPos(x, y-ZONE_SQUARE_SIZE);
					changeZoneSquareType(zoneSquare, true);
				}
				if(y < (ZONE_GRID_SIZE-1)*ZONE_SQUARE_SIZE && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE+1]!=nullptr && gameLogicThread.getMapManager()->getZoneGrid()[x/ZONE_SQUARE_SIZE][y/ZONE_SQUARE_SIZE+1]->getZoneType() == originalZoneType){
					zoneSquare->setPos(x, y+ZONE_SQUARE_SIZE);
					changeZoneSquareType(zoneSquare, true);
				}
			}
		}
	}
}

void MainGameScene::buildingCreated(Building *building)
{
	//Si le gestionnaire de carte a créé un batiment
	//On l'ajoute à la liste et on l'affiche sur la scène
	connect(building->getPixmapItem(), SIGNAL(requestPosChange(qreal,qreal)), this, SLOT(pixmapRequestedPosChange(qreal,qreal)));
	connect(building->getPixmapItem(), SIGNAL(requestPixmapChange(QPixmap)), this, SLOT(pixmapRequestChangePixmap(QPixmap)));
	building->getPixmapItem()->mainThreadConnected();
	buildingList.append(building->getPixmapItem());
	addItem(building->getPixmapItem());
}

void MainGameScene::buildingRemoved(Building *building)
{
	buildingList.removeAll(building->getPixmapItem());
	removeItem(building->getPixmapItem());
}

void MainGameScene::pixmapRequestedPosChange(qreal x, qreal y)
{
	//Si un pixmapItem doit changer de position -> on change sa position depuis le thread graphique (thread principal)
	CustomGraphicsPixmapItem *item = (CustomGraphicsPixmapItem*) sender();
	item->setPosMainThread(x, y);
}

void MainGameScene::pixmapRequestChangePixmap(const QPixmap &pixmap)
{
	//Si un pixmapItem doit changer de texture -> on change sa texture depuis le thread graphique (thread principal)
	CustomGraphicsPixmapItem *item = (CustomGraphicsPixmapItem*) sender();
	item->setPixmapMainThread(pixmap);
}

void MainGameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	//Si on déplace la souris sur la scène
	QGraphicsScene::mouseMoveEvent(event);
	int x = event->scenePos().x();
	int y = event->scenePos().y();

	//Si on est en mode construction, on déplace le curseur de construction et on l'affiche (ou le masque) en fonction de si l'on est en train de déplacer la scène (avec un clique droit)
	if(buildingMode){
		buildCursor.setPosition(x, y);
		if(((CustomGraphicsView*)parent())->isPan() && buildCursor.isVisible()){
			buildCursor.setVisible(false);
		}
		else if(!((CustomGraphicsView*)parent())->isPan() && !buildCursor.isVisible()){
			buildCursor.setVisible(true);
		}
	}
	else if(destroyMode && QApplication::mouseButtons() == Qt::LeftButton){
		//Si l'on déplace la souris en maintenant clique gauche, on peut supprimer plusieurs routes
		if(destroyGridType == Grid::Type::GridRoad){
			RoadSquare *roadSquare = gameLogicThread.getMapManager()->getRoadGrid()[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE];
			if(roadSquare != nullptr){
				removeItem(roadSquare->getPixmapItem());
				roadPixmapItems[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE] = nullptr;
				gameLogicThread.getMapManager()->requestDestroyRoad(roadSquare);
				emit playSoundEffect(DestroyShort);
			}
		}
	}
}

void MainGameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	int x = event->scenePos().x();
	int y = event->scenePos().y();
	//Si on fait un clique gauche sur la scène
	if(event->button() == Qt::LeftButton){
		//En mode destruction, on détruit le carré le route ou le batiment visé
		if(destroyMode && QApplication::mouseButtons() == Qt::LeftButton){
			if(destroyGridType == Grid::Type::GridRoad){
				RoadSquare *roadSquare = gameLogicThread.getMapManager()->getRoadGrid()[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE];
				if(roadSquare != nullptr){
					removeItem(roadSquare->getPixmapItem());
					roadPixmapItems[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE] = nullptr;
					gameLogicThread.changeMoney(roadSquare->getCost()*0.5);
					gameLogicThread.getMapManager()->requestDestroyRoad(roadSquare);
					emit playSoundEffect(DestroyShort);
				}
			}
			else if(destroyGridType == Grid::Type::GridBuilding){
				Building *building = gameLogicThread.getMapManager()->getBuildingFromPos(x, y);
				if(building != nullptr){
					removeItem(building->getPixmapItem());
					buildingList.removeAll(building->getPixmapItem());
					if(building->getBuildingType()==Buildings::Type::BuildingService){
						gameLogicThread.changeMoney(((ServiceBuilding*)building)->getCost()*0.5);
					}
					gameLogicThread.getMapManager()->requestDestroyBuilding(building);
					emit playSoundEffect(DestroyLong);
				}

			}
		}
	}
}

void MainGameScene::resizeEvent(QResizeEvent *event)
{
	//En cas de redimensionnement de la scène (si on agrandi la fênetre par exemple, on réinitialise le carré de la scène pour éviter que l'on puisse déplacer la caméra hors de la scène
	Q_UNUSED(event);
	setSceneRect(0, 0, TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE, TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE);
}
