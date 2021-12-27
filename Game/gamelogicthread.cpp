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
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/house_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/caravan").scaled(ZONE_SQUARE_SIZE*1+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/residential/caravan_rotate").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*1+BUILDING_MARGIN));
	buildingPixmaps[Residential] = residentialPixmaps;

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
		Building *building = zoneGrid[zoneX][zoneY]->getBuilding();
		for(int i=0;i<building->getCoveringZones().count();i++){
			building->getCoveringZones().value(i)->setBuilding(nullptr);
		}
		delete building;
	}
	delete zoneGrid[zoneX][zoneY];
	zoneGrid[zoneX][zoneY] = nullptr;
}

void GameLogicThread::setGameSpeed(double newGameSpeed)
{
	gameSpeed = newGameSpeed;
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

void GameLogicThread::requestDestroy(GridSquare *gridSquare)
{
	if(gridSquare->getGridType() == Road){
		RoadSquare *roadSquare = roadGrid[gridSquare->getPosX()/ROAD_SQUARE_SIZE][gridSquare->getPosY()/ROAD_SQUARE_SIZE];
		roadGrid[gridSquare->getPosX()/ROAD_SQUARE_SIZE][gridSquare->getPosY()/ROAD_SQUARE_SIZE] = nullptr;
		updateAdjacentRoadPixmaps(gridSquare->getPosX()/ROAD_SQUARE_SIZE, gridSquare->getPosY()/ROAD_SQUARE_SIZE, 1);

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

		int random;
		for(int x=0;x<ZONE_GRID_SIZE;x++){
			for(int y=0;y<ZONE_GRID_SIZE;y++){
				if(zoneGrid[x][y]!=nullptr && zoneGrid[x][y]->getBuilding()==nullptr){
					random = QRandomGenerator::global()->generate()%1000;
					if(zoneGrid[x][y]->getZoneType()==Residential){
						if(random > 970){ //Generate building
							bool canPut4x4 = true;
							for(int i=x;i<x+4;i++){
								for(int j=y;j<y+4;j++){
									if(i>=ZONE_GRID_SIZE || j>=ZONE_GRID_SIZE){
										canPut4x4 = false;
									}
									else if(zoneGrid[i][j] == nullptr || zoneGrid[i][j]->getBuilding() != nullptr || zoneGrid[i][j]->getZoneType() != Residential){
										canPut4x4 = false;
									}
								}
							}

							if(canPut4x4){
								ResidentialBuilding *newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*4, ZONE_SQUARE_SIZE*4, 2, 5, &buildingPixmaps);
								for(int i=x;i<x+4;i++){
									for(int j=y;j<y+4;j++){
										zoneGrid[i][j]->setBuilding(newBuilding);
										newBuilding->addCoveringZone(zoneGrid[i][j]);
									}
								}
								emit buildingCreated(newBuilding);
							}
							else if(random >997){
								int zoneAdjacentToRoad = zoneGrid[x][y]->isZoneAdjacentToRoad(roadGrid);
								if(zoneAdjacentToRoad == 2 && y > 0 && zoneGrid[x][y-1]!=nullptr && zoneGrid[x][y-1]->getZoneType()==Residential && zoneGrid[x][y-1]->getBuilding()==nullptr){
									ResidentialBuilding *newBuilding = new ResidentialBuilding(zoneGrid[x][y-1]->getPosX(), zoneGrid[x][y-1]->getPosY(), ZONE_SQUARE_SIZE*1, ZONE_SQUARE_SIZE*2, 1, 3, &buildingPixmaps);
									zoneGrid[x][y]->setBuilding(newBuilding);
									zoneGrid[x][y-1]->setBuilding(newBuilding);
									newBuilding->addCoveringZone(zoneGrid[x][y]);
									newBuilding->addCoveringZone(zoneGrid[x][y-1]);
									emit buildingCreated(newBuilding);
								}
								else if(zoneAdjacentToRoad == 8 && y < ZONE_GRID_SIZE-1 && zoneGrid[x][y+1]!=nullptr && zoneGrid[x][y+1]->getZoneType()==Residential && zoneGrid[x][y+1]->getBuilding()==nullptr){
									ResidentialBuilding *newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*1, ZONE_SQUARE_SIZE*2, 1, 3, &buildingPixmaps);
									zoneGrid[x][y]->setBuilding(newBuilding);
									zoneGrid[x][y+1]->setBuilding(newBuilding);
									newBuilding->addCoveringZone(zoneGrid[x][y]);
									newBuilding->addCoveringZone(zoneGrid[x][y+1]);
									emit buildingCreated(newBuilding);
								}
								else if(zoneAdjacentToRoad == 6 && x > 0 && zoneGrid[x-1][y]!=nullptr && zoneGrid[x-1][y]->getZoneType()==Residential && zoneGrid[x-1][y]->getBuilding()==nullptr){
									ResidentialBuilding *newBuilding = new ResidentialBuilding(zoneGrid[x-1][y]->getPosX(), zoneGrid[x-1][y]->getPosY(), ZONE_SQUARE_SIZE*2, ZONE_SQUARE_SIZE*1, 1, 3, &buildingPixmaps);
									zoneGrid[x][y]->setBuilding(newBuilding);
									zoneGrid[x-1][y]->setBuilding(newBuilding);
									newBuilding->addCoveringZone(zoneGrid[x][y]);
									newBuilding->addCoveringZone(zoneGrid[x-1][y]);
									emit buildingCreated(newBuilding);
								}
								else if(zoneAdjacentToRoad == 4 && x < ZONE_GRID_SIZE-1 && zoneGrid[x+1][y]!=nullptr && zoneGrid[x+1][y]->getZoneType()==Residential && zoneGrid[x+1][y]->getBuilding()==nullptr){
									ResidentialBuilding *newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*2, ZONE_SQUARE_SIZE*1, 1, 3, &buildingPixmaps);
									zoneGrid[x][y]->setBuilding(newBuilding);
									zoneGrid[x+1][y]->setBuilding(newBuilding);
									newBuilding->addCoveringZone(zoneGrid[x][y]);
									newBuilding->addCoveringZone(zoneGrid[x+1][y]);
									emit buildingCreated(newBuilding);
								}
							}
						}
					}
				}
			}
		}

		//qDebug() << "Time taken : " << timer.elapsed() << "ms";
	}
}

