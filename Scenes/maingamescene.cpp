#include "maingamescene.h"

MainGameScene::MainGameScene(CustomGraphicsView *parent) : QGraphicsScene{parent}
{
	this->parent = parent;
	TerrainSquare ***terrainGrid = gameLogicThread.getTerrainGrid();
	terrainPixmapItems = new QGraphicsPixmapItem**[TERRAIN_GRID_SIZE];
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		terrainPixmapItems[x] = new QGraphicsPixmapItem*[TERRAIN_GRID_SIZE];
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			terrainPixmapItems[x][y] = terrainGrid[x][y]->getPixmapItem();
			terrainPixmapItemGroup.addToGroup(terrainPixmapItems[x][y]);
		}
	}
	addItem(&terrainPixmapItemGroup);

	roadPixmapItems = new QGraphicsPixmapItem**[ROAD_GRID_SIZE];
	for(int x=0;x<ROAD_GRID_SIZE;x++){
		roadPixmapItems[x] = new QGraphicsPixmapItem*[ROAD_GRID_SIZE];
		for(int y=0;y<ROAD_GRID_SIZE;y++){
			roadPixmapItems[x][y] = nullptr;
		}
	}

	zonePixmapItems = new QGraphicsPixmapItem**[ZONE_GRID_SIZE];
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		zonePixmapItems[x] = new QGraphicsPixmapItem*[ZONE_GRID_SIZE];
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			zonePixmapItems[x][y] = nullptr;
		}
	}

	buildSquare.setRoadGrid(gameLogicThread.getRoadGrid());
	buildSquare.setZoneGrid(gameLogicThread.getZoneGrid());

	connect(&gameLogicThread, SIGNAL(zoneSquareCreated(ZoneSquare*)), this, SLOT(zoneSquareCreated(ZoneSquare*)));
	connect(&gameLogicThread, SIGNAL(zoneSquareRemoved(ZoneSquare*)), this, SLOT(zoneSquareRemoved(ZoneSquare*)));
	connect(&gameLogicThread, SIGNAL(buildingCreated(Building*)), this, SLOT(buildingCreated(Building*)));
	connect(&gameLogicThread, SIGNAL(gameDateChanged(QDate)), this, SIGNAL(gameDateChanged(QDate)));
	connect(&gameLogicThread, SIGNAL(gameDemandsUpdated(int,int,int,int,int,int)), this, SIGNAL(gameDemandsUpdated(int,int,int,int,int,int)));
}

MainGameScene::~MainGameScene()
{
	gameLogicThread.exit(0);
	gameLogicThread.wait(2000);
}

void MainGameScene::startGameLogic()
{
	gameLogicThread.start();
}

void MainGameScene::enableBuildMode(RoadType roadType)
{
	buildSquare.setSquareToBuild(new RoadSquare(0, 0, roadType, gameLogicThread.getRoadPixmaps()));
	addItem(&buildSquare);
	connect(&buildSquare, SIGNAL(requestBuildSquare(GridSquare*)), this, SLOT(requestBuildSquare(GridSquare*)));
	buildingMode = true;
}

void MainGameScene::enableBuildMode(ZoneType zoneType)
{
	buildSquare.setSquareToBuild(new ZoneSquare(0, 0, zoneType, gameLogicThread.getZonePixmaps()));
	addItem(&buildSquare);
	connect(&buildSquare, SIGNAL(changeZoneSquareType(ZoneSquare*)), this, SLOT(changeZoneSquareType(ZoneSquare*)));
	buildingMode = true;
}

void MainGameScene::disableBuildMode()
{
	buildingMode = false;
	removeItem(&buildSquare);
	disconnect(&buildSquare);
}

void MainGameScene::enableDestroyMode(GridType gridType)
{
	Q_UNUSED(gridType);
	destroyMode = true;
}

void MainGameScene::disableDestroyMode()
{
	destroyMode = false;
}

void MainGameScene::showZones(bool enabled)
{
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zonePixmapItems[x][y] != nullptr){
				zonePixmapItems[x][y]->setVisible(enabled);
			}
		}
	}
}

void MainGameScene::setGameSpeed(double newGameSpeed)
{
	gameLogicThread.setGameSpeed(newGameSpeed);
}

void MainGameScene::requestBuildSquare(GridSquare *gridSquare)
{
	GridSquare *result = gameLogicThread.requestBuildSquare(gridSquare);
	if(result != nullptr){
		if(result->getGridType()==Road){
			roadPixmapItems[result->getPosX()/ROAD_SQUARE_SIZE][result->getPosY()/ROAD_SQUARE_SIZE] = result->getPixmapItem();
			addItem(result->getPixmapItem());
		}
	}
}

void MainGameScene::zoneSquareCreated(ZoneSquare *zoneSquare)
{
	int posX = zoneSquare->getPosX()/ZONE_SQUARE_SIZE;
	int posY = zoneSquare->getPosY()/ZONE_SQUARE_SIZE;
	zonePixmapItems[posX][posY] = zoneSquare->getPixmapItem();
	addItem(zoneSquare->getPixmapItem());
}

void MainGameScene::zoneSquareRemoved(ZoneSquare *zoneSquare)
{
	int posX = zoneSquare->getPosX()/ZONE_SQUARE_SIZE;
	int posY = zoneSquare->getPosY()/ZONE_SQUARE_SIZE;
	if(zonePixmapItems[posX][posY]!=nullptr){
		removeItem(zonePixmapItems[posX][posY]);
		zonePixmapItems[posX][posY] = nullptr;
	}
	if(zoneSquare->getBuilding()!=nullptr && buildingList.contains(zoneSquare->getBuilding()->getPixmapItem())){
		removeItem(zoneSquare->getBuilding()->getPixmapItem());
		buildingList.removeAll(zoneSquare->getBuilding()->getPixmapItem());
	}
}

void MainGameScene::changeZoneSquareType(ZoneSquare *zoneSquare)
{
	if(gameLogicThread.getZoneGrid()[zoneSquare->getPosX()/ZONE_SQUARE_SIZE][zoneSquare->getPosY()/ZONE_SQUARE_SIZE]!=nullptr){
		ZoneSquare *oldSquare = gameLogicThread.getZoneGrid()[zoneSquare->getPosX()/ZONE_SQUARE_SIZE][zoneSquare->getPosY()/ZONE_SQUARE_SIZE];
		if(oldSquare->getZoneType() != zoneSquare->getZoneType()){
			oldSquare->setZoneType(zoneSquare->getZoneType());
			if(oldSquare->getBuilding()!=nullptr){
				Building *building = oldSquare->getBuilding();
				for(int i=0;i<building->getCoveringZones().count();i++){
					building->getCoveringZones().value(i)->setBuilding(nullptr);
				}
				if(buildingList.contains(building->getPixmapItem())){
					buildingList.removeAll(building->getPixmapItem());
				}
				delete building;
			}
		}
	}
}

void MainGameScene::buildingCreated(Building *building)
{
	buildingList.append(building->getPixmapItem());
	addItem(building->getPixmapItem());
}

void MainGameScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	int x = event->scenePos().x();
	int y = event->scenePos().y();
	if(buildingMode){
		buildSquare.setPosition(x, y);
		if(parent->isPan() && buildSquare.isVisible()){
			buildSquare.setVisible(false);
		}
		else if(!parent->isPan() && !buildSquare.isVisible()){
			buildSquare.setVisible(true);
		}
	}
	else if(destroyMode && QApplication::mouseButtons() == Qt::LeftButton){
		RoadSquare *roadSquare = gameLogicThread.getRoadGrid()[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE];
		if(roadSquare != nullptr){
			removeItem(roadSquare->getPixmapItem());
			gameLogicThread.requestDestroyRoad(roadSquare);
		}
	}
}

void MainGameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	int x = event->scenePos().x();
	int y = event->scenePos().y();
	if(event->button() == Qt::LeftButton){
		if(destroyMode && QApplication::mouseButtons() == Qt::LeftButton){
			RoadSquare *roadSquare = gameLogicThread.getRoadGrid()[x/ROAD_SQUARE_SIZE][y/ROAD_SQUARE_SIZE];
			if(roadSquare != nullptr){
				removeItem(roadSquare->getPixmapItem());
				gameLogicThread.requestDestroyRoad(roadSquare);
			}
		}
	}
}
