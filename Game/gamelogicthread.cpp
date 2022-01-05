#include "gamelogicthread.h"

GameLogicThread::GameLogicThread()
{
	updateTimer = new QTimer();
	updateTimer->moveToThread(this);

	//Terrain Pixmap
	terrainPixmaps[Grass] = QPixmap(":/pixmaps/terrain/grassTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Mountain] = QPixmap(":/pixmaps/terrain/mountainTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Rock] = QPixmap(":/pixmaps/terrain/rockTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Water] = QPixmap(":/pixmaps/terrain/waterTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Sand] = QPixmap(":/pixmaps/terrain/sandTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);

	//Road Pixmap
	twoLanesPixmaps[Straight] = QPixmap(":/pixmaps/roads/two-lanes/straight-1").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Turn] = QPixmap(":/pixmaps/roads/two-lanes/turn").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[TJunction] = QPixmap(":/pixmaps/roads/two-lanes/T-junction").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Cross] = QPixmap(":/pixmaps/roads/two-lanes/cross").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[DeadEnd] = QPixmap(":/pixmaps/roads/two-lanes/dead-end").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);

	roadPixmaps[TwoLanes] = twoLanesPixmaps;

	//Zone Pixmap
	zonePixmaps[None] = QPixmap(":/pixmaps/zones/noneTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Residential] = QPixmap(":/pixmaps/zones/residentialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Commercial] = QPixmap(":/pixmaps/zones/commercialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Industrial] = QPixmap(":/pixmaps/zones/industrialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);

	//Building Pixmap
	//Residential
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/house_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/caravan").scaled(ZONE_SQUARE_SIZE*1+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/caravan_rotate").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*1+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/skyscrapper_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN));
	buildingPixmaps[Residential] = residentialPixmaps;

	//Commercial
	commercialPixmaps.append(QPixmap(":/pixmaps/commercial/burger").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	commercialPixmaps.append(QPixmap(":/pixmaps/commercial/food_truck").scaled(ZONE_SQUARE_SIZE*1+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	commercialPixmaps.append(QPixmap(":/pixmaps/commercial/food_truck_rotate").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*1+BUILDING_MARGIN));
	buildingPixmaps[Commercial] = commercialPixmaps;

	//Industrial
	industrialPixmaps.append(QPixmap(":/pixmaps/industrial/factory_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/industrial/factory_1_rotate").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/industrial/factory_2").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*3+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/industrial/factory_2_rotate").scaled(ZONE_SQUARE_SIZE*3+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	buildingPixmaps[Industrial] = industrialPixmaps;

	//Init map
	const siv::PerlinNoise::seed_type seed = QRandomGenerator::global()->generate();
	const siv::PerlinNoise perlin{seed};

	terrainGrid = new TerrainSquare**[TERRAIN_GRID_SIZE];

	double noise;
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		terrainGrid[x] = new TerrainSquare*[TERRAIN_GRID_SIZE];
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			noise = perlin.octave2D_01((x*0.02), (y*0.02), 16);
			terrainGrid[x][y] = new TerrainSquare(noise, x*TERRAIN_SQUARE_SIZE, y*TERRAIN_SQUARE_SIZE, &terrainPixmaps);
		}
	}

	//Init road
	roadGrid = new RoadSquare**[ROAD_GRID_SIZE];
	for(int x=0;x<ROAD_GRID_SIZE;x++){
		roadGrid[x] = new RoadSquare*[ROAD_GRID_SIZE];
		for(int y=0;y<ROAD_GRID_SIZE;y++){
			roadGrid[x][y] = nullptr;
		}
	}

	//Init zone
	zoneGrid = new ZoneSquare**[ZONE_GRID_SIZE];
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		zoneGrid[x] = new ZoneSquare*[ZONE_GRID_SIZE];
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			zoneGrid[x][y] = nullptr;
		}
	}
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

void GameLogicThread::removeZoneSquare(int zoneX, int zoneY)
{
	emit zoneSquareRemoved(zoneGrid[zoneX][zoneY]);
	if(zoneGrid[zoneX][zoneY]->getBuilding()!=nullptr){
		requestDestroyBuilding(zoneGrid[zoneX][zoneY]->getBuilding());
	}
	delete zoneGrid[zoneX][zoneY];
	zoneGrid[zoneX][zoneY] = nullptr;
}

void GameLogicThread::setGameSpeed(double newGameSpeed)
{
	gameSpeed = newGameSpeed;
}

void GameLogicThread::updateGameDemands()
{
	//TODO : update game demands
	int totalResidents = 0;
	int totalResidentialCapacity = 0;
	int totalWorkCapacity = 0;
	int totalCommercialCapacity = 0;

	for(int i=0;i<buildingList.size();i++){
		if(buildingList.at(i)->getBuildingType() == Residential){
			totalResidentialCapacity += ((ResidentialBuilding*)buildingList.at(i))->getMaxResidents();
			totalResidents += ((ResidentialBuilding*)buildingList.at(i))->getResidents();
		}
		else if(buildingList.at(i)->getBuildingType() == Commercial){
			totalWorkCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxWorkers();
			totalCommercialCapacity += ((CommercialBuilding*)buildingList.at(i))->getMaxClients();
		}
		else if(buildingList.at(i)->getBuildingType() == Industrial){
			totalWorkCapacity += ((IndustrialBuilding*)buildingList.at(i))->getMaxWorkers();
		}
	}

	int res ;
	if(totalWorkCapacity > totalResidents+(double)totalResidents*0.15){
		res = totalWorkCapacity-totalResidents;
	}
	else{
		res = 0;
	}
	int maxRes = totalWorkCapacity;
	if(maxRes == 0){
		if(totalResidents == 0){
			res = 1;
		}
		maxRes = 5;
	}

	int com;
	if(totalResidents < totalCommercialCapacity){
		com = 0;
	}
	else{
		com = totalResidents-totalCommercialCapacity;
	}
	int maxCom = totalResidents;
	if(maxCom == 0){
		maxCom = 1;
	}

	int indu;
	if(totalResidents < totalWorkCapacity){
		indu = 0;
	}
	else{
		indu = totalResidents - totalWorkCapacity;
	}
	int maxIndu = totalResidents;
	if(maxIndu == 0){
		maxIndu = 1;
	}

	emit gameDemandsUpdated(res, maxRes, com, maxCom, indu, maxIndu);
}

RoadSquare ***GameLogicThread::getRoadGrid() const
{
	return roadGrid;
}

QMap<RoadType, QMap<RoadAngle, QPixmap>>* GameLogicThread::getRoadPixmaps()
{
	return &roadPixmaps;
}

QMap<ZoneType, QPixmap> *GameLogicThread::getZonePixmaps()
{
	return &zonePixmaps;
}

ZoneSquare ***GameLogicThread::getZoneGrid() const
{
	return zoneGrid;
}

GridSquare* GameLogicThread::requestBuildSquare(GridSquare *square)
{
	if(square->getGridType()==Road){
		if(roadGrid[square->getPosX()/ROAD_SQUARE_SIZE][square->getPosY()/ROAD_SQUARE_SIZE] == nullptr){
			if((terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Rock)){

				RoadSquare *newSquare = new RoadSquare(square->getPosX(), square->getPosY(), ((RoadSquare*)square)->getRoadType(), &roadPixmaps);
				int x = newSquare->getPosX()/ROAD_SQUARE_SIZE;
				int y = newSquare->getPosY()/ROAD_SQUARE_SIZE;
				roadGrid[x][y] = newSquare;
				newSquare->updatePixmap(roadGrid);
				updateAdjacentRoadPixmaps(x, y, 1);
				updateAdjacentRoadZones(newSquare);

				int zoneX, zoneY;

				for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
					for(int j=0;j<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;j++){
						zoneX = (newSquare->getPosX() + i*ZONE_SQUARE_SIZE)/ZONE_SQUARE_SIZE;
						zoneY = (newSquare->getPosY() + j* ZONE_SQUARE_SIZE)/ZONE_SQUARE_SIZE;
						if(zoneGrid[zoneX][zoneY]!=nullptr){
							removeZoneSquare(zoneX, zoneY);
						}
					}
				}

				return newSquare;
			}
		}
	}
	return nullptr;
}

void GameLogicThread::updateAdjacentRoadPixmaps(int x, int y, int recursive)
{
	if(x>0 && roadGrid[x-1][y]!=nullptr){
		if(recursive > 0){
			updateAdjacentRoadPixmaps(x-1, y, recursive-1);
		}
		roadGrid[x-1][y]->updatePixmap(roadGrid);
		updateAdjacentRoadZones(roadGrid[x-1][y]);
	}
	if(x < ROAD_GRID_SIZE-1 && roadGrid[x+1][y]!=nullptr){
		if(recursive > 0){
			updateAdjacentRoadPixmaps(x+1, y, recursive-1);
		}
		roadGrid[x+1][y]->updatePixmap(roadGrid);
		updateAdjacentRoadZones(roadGrid[x+1][y]);
	}
	if(y > 0 &&roadGrid[x][y-1]!=nullptr){
		if(recursive > 0){
			updateAdjacentRoadPixmaps(x, y-1, recursive-1);
		}
		roadGrid[x][y-1]->updatePixmap(roadGrid);
		updateAdjacentRoadZones(roadGrid[x][y-1]);
	}
	if(y < ROAD_GRID_SIZE-1 && roadGrid[x][y+1]!=nullptr){
		if(recursive > 0){
			updateAdjacentRoadPixmaps(x, y+1, recursive-1);
		}
		roadGrid[x][y+1]->updatePixmap(roadGrid);
		updateAdjacentRoadZones(roadGrid[x][y+1]);
	}
}

void GameLogicThread::updateAdjacentRoadZones(RoadSquare *roadSquare)
{
	int zoneX, zoneY;
	if(!roadSquare->isConnectedBottom() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX() + i*ZONE_SQUARE_SIZE;
				zoneY = (roadSquare->getPosY()+ROAD_SQUARE_SIZE) + j*ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					TerrainType terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Water || terrainType == Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, &zonePixmaps);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	if(!roadSquare->isConnectedTop() && roadSquare->getPosY() > 0){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX() + i*ZONE_SQUARE_SIZE;
				zoneY = (roadSquare->getPosY()-ZONE_SQUARE_SIZE) - j*ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					TerrainType terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Water || terrainType == Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, &zonePixmaps);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	if(!roadSquare->isConnectedLeft() && roadSquare->getPosX() > 0 ){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = (roadSquare->getPosX() - ZONE_SQUARE_SIZE) - j*ZONE_SQUARE_SIZE;
				zoneY = roadSquare->getPosY() + i* ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					TerrainType terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Water || terrainType == Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, &zonePixmaps);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	if(!roadSquare->isConnectedRight() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = (roadSquare->getPosX() + ROAD_SQUARE_SIZE) + j*ZONE_SQUARE_SIZE;
				zoneY = roadSquare->getPosY() + i* ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					TerrainType terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Water || terrainType == Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, &zonePixmaps);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
}

void GameLogicThread::requestDestroyRoad(RoadSquare *roadSquare)
{
	roadGrid[roadSquare->getPosX()/ROAD_SQUARE_SIZE][roadSquare->getPosY()/ROAD_SQUARE_SIZE] = nullptr;
	updateAdjacentRoadPixmaps(roadSquare->getPosX()/ROAD_SQUARE_SIZE, roadSquare->getPosY()/ROAD_SQUARE_SIZE, 1);

	int zoneX, zoneY;
	if(!roadSquare->isConnectedBottom() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX()/ZONE_SQUARE_SIZE + i;
				zoneY = (roadSquare->getPosY()+ROAD_SQUARE_SIZE)/ZONE_SQUARE_SIZE + j;
				if((zoneX < ZONE_GRID_SIZE) && (zoneY < ZONE_GRID_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX][zoneY]!=nullptr && !zoneGrid[zoneX][zoneY]->isZoneValid(zoneGrid, roadGrid)){
					removeZoneSquare(zoneX, zoneY);
				}
			}
		}
	}
	if(!roadSquare->isConnectedTop() && roadSquare->getPosY() > 0){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX()/ZONE_SQUARE_SIZE + i;
				zoneY = (roadSquare->getPosY()-ZONE_SQUARE_SIZE)/ZONE_SQUARE_SIZE - j;
				if((zoneX < ZONE_GRID_SIZE) && (zoneY < ZONE_GRID_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX][zoneY]!=nullptr && !zoneGrid[zoneX][zoneY]->isZoneValid(zoneGrid, roadGrid)){
					removeZoneSquare(zoneX, zoneY);
				}
			}
		}
	}

	if(!roadSquare->isConnectedLeft() && roadSquare->getPosX() > 0 ){
		for(int i=0;i<ROAD_ZONE_RANGE;i++){
			for(int j=0;j<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;j++){
				zoneX = (roadSquare->getPosX()-ZONE_SQUARE_SIZE)/ZONE_SQUARE_SIZE - i;
				zoneY = roadSquare->getPosY()/ZONE_SQUARE_SIZE + j;
				if((zoneX < ZONE_GRID_SIZE) && (zoneY < ZONE_GRID_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX][zoneY]!=nullptr && !zoneGrid[zoneX][zoneY]->isZoneValid(zoneGrid, roadGrid)){
					removeZoneSquare(zoneX, zoneY);;
				}
			}
		}
	}

	if(!roadSquare->isConnectedRight() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_ZONE_RANGE;i++){
			for(int j=0;j<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;j++){
				zoneX = (roadSquare->getPosX()+ROAD_SQUARE_SIZE)/ZONE_SQUARE_SIZE + i;
				zoneY = roadSquare->getPosY()/ZONE_SQUARE_SIZE + j;
				if((zoneX < ZONE_GRID_SIZE) && (zoneY < ZONE_GRID_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX][zoneY]!=nullptr && !zoneGrid[zoneX][zoneY]->isZoneValid(zoneGrid, roadGrid)){
					removeZoneSquare(zoneX, zoneY);
				}
			}
		}
	}

	delete roadSquare;
}

void GameLogicThread::requestDestroyBuilding(Building *building)
{
	for(int i=0;i<building->getCoveringZones().count();i++){
		building->getCoveringZones().value(i)->setBuilding(nullptr);
	}
	buildingList.removeAll(building);
	delete building;
}

TerrainSquare ***GameLogicThread::getTerrainGrid() const
{
	return terrainGrid;
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
			generateNewBuilding(Residential);
		}

		if(QRandomGenerator::global()->generate()%100 > 60){
			generateNewBuilding(Commercial);
		}

		if(QRandomGenerator::global()->generate()%100 > 60){
			generateNewBuilding(Industrial);
		}

		updateGameDemands();
		//qDebug() << "Time taken : " << timer.elapsed() << "ms";
	}
}

bool GameLogicThread::putBuilding(int x, int y, int widthGrid, int heightGrid, ZoneType zoneType)
{
	bool anyAdjacentToRoad = false;
	if(x < 0 || y < 0 || x+widthGrid >= ZONE_GRID_SIZE || y+heightGrid >= ZONE_GRID_SIZE){
		return false;
	}

	for(int i = x; i < x+widthGrid; i++){
		for(int j = y; j < y+heightGrid; j++){
			if(zoneGrid[i][j] == nullptr || zoneGrid[i][j]->getBuilding() != nullptr || zoneGrid[i][j]->getZoneType() != zoneType){
				return false;
			}
			if(zoneGrid[i][j]->isZoneAdjacentToRoad(roadGrid) != 0){
				anyAdjacentToRoad = true;
			}
		}
	}
	if(!anyAdjacentToRoad){
		return false;
	}

	Building *newBuilding;
	if(zoneType==Residential){
		newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, &buildingPixmaps);
	}
	else if(zoneType==Commercial){
		newBuilding = new CommercialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, &buildingPixmaps);
	}
	else{
		newBuilding = new IndustrialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, &buildingPixmaps);
	}
	for(int i=x;i<x+widthGrid;i++){
		for(int j=y;j<y+heightGrid;j++){
			zoneGrid[i][j]->setBuilding(newBuilding);
			newBuilding->addCoveringZone(zoneGrid[i][j]);
		}
	}
	buildingList.append(newBuilding);
	emit buildingCreated(newBuilding);
	return true;
}

bool GameLogicThread::generateNewBuilding(ZoneType zoneType)
{
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zoneType == Residential){
				if(putBuilding(x, y, 4, 8, zoneType) || putBuilding(x, y, 4, 4, zoneType) || putBuilding(x, y, 1, 2, zoneType) || putBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Commercial){
				if(putBuilding(x, y, 4, 4, zoneType) || putBuilding(x, y, 1, 2, zoneType) || putBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Industrial){
				if(putBuilding(x, y, 4, 4, zoneType) || putBuilding(x, y, 3, 2, zoneType) || putBuilding(x, y, 2, 3, zoneType)){
					return true;
				}
			}
		}
	}
	return false;
}
