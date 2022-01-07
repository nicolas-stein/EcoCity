#include "mapmanager.h"

MapManager::MapManager()
{

}

void MapManager::initMap(RessourceManager *ressourceManager)
{
	this->ressourceManager = ressourceManager;
	const siv::PerlinNoise::seed_type seed = QRandomGenerator::global()->generate();
	const siv::PerlinNoise perlin{seed};

	terrainGrid = new TerrainSquare**[TERRAIN_GRID_SIZE];

	double noise;
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		terrainGrid[x] = new TerrainSquare*[TERRAIN_GRID_SIZE];
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			noise = perlin.octave2D_01((x*0.02), (y*0.02), 16);
			terrainGrid[x][y] = new TerrainSquare(noise, x*TERRAIN_SQUARE_SIZE, y*TERRAIN_SQUARE_SIZE, ressourceManager);
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

GridSquare* MapManager::requestBuildSquare(GridSquare *square)
{
	if(square->getGridType()==GridRoad){
		if(roadGrid[square->getPosX()/ROAD_SQUARE_SIZE][square->getPosY()/ROAD_SQUARE_SIZE] == nullptr){
			if((terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Rock)
					&& (terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Grass || terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Sand ||terrainGrid[square->getPosX()/TERRAIN_SQUARE_SIZE+1][square->getPosY()/TERRAIN_SQUARE_SIZE+1]->getTerrainType() == Rock)){

				RoadSquare *newSquare = new RoadSquare(square->getPosX(), square->getPosY(), ((RoadSquare*)square)->getRoadType(), ressourceManager);
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

Building *MapManager::requestBuildBuilding(Building *building)
{
	if(building->getBuildingType() == BuildingService){
		ServiceBuilding* newBuilding = nullptr;
		if(((ServiceBuilding*)building)->getServiceType() == Power){
			newBuilding = new PowerBuilding(building->getPosX(), building->getPosY(), ressourceManager, ((PowerBuilding*)building)->getPowerType());
			serviceBuildings.append(newBuilding);
		}
		return newBuilding;
	}
	return nullptr;
}

void MapManager::removeZoneSquare(int zoneX, int zoneY)
{
	emit zoneSquareRemoved(zoneGrid[zoneX][zoneY]);
	if(zoneGrid[zoneX][zoneY]->getBuilding()!=nullptr){
		requestDestroyBuilding(zoneGrid[zoneX][zoneY]->getBuilding());
	}
	delete zoneGrid[zoneX][zoneY];
	zoneGrid[zoneX][zoneY] = nullptr;
}

void MapManager::updateAdjacentRoadPixmaps(int x, int y, int recursive)
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

void MapManager::updateAdjacentRoadZones(RoadSquare *roadSquare)
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
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, ressourceManager);
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
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, ressourceManager);
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
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, ressourceManager);
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
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, None, ressourceManager);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
}

void MapManager::requestDestroyRoad(RoadSquare *roadSquare)
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

bool MapManager::generateNewZoneBuilding(ZoneType zoneType)
{
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zoneType == Residential){
				if(putZoneBuilding(x, y, 4, 8, zoneType) || putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 1, 2, zoneType) || putZoneBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Commercial){
				if(putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 1, 2, zoneType) || putZoneBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Industrial){
				if(putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 3, 2, zoneType) || putZoneBuilding(x, y, 2, 3, zoneType)){
					return true;
				}
			}
		}
	}
	return false;
}

bool MapManager::putZoneBuilding(int x, int y, int widthGrid, int heightGrid, ZoneType zoneType)
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

	ZoneBuilding *newBuilding;
	if(zoneType==Residential){
		newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, ressourceManager);
	}
	else if(zoneType==Commercial){
		newBuilding = new CommercialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, ressourceManager);
	}
	else{
		newBuilding = new IndustrialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, ressourceManager);
	}
	for(int i=x;i<x+widthGrid;i++){
		for(int j=y;j<y+heightGrid;j++){
			zoneGrid[i][j]->setBuilding(newBuilding);
			newBuilding->addCoveringZone(zoneGrid[i][j]);
		}
	}
	zoneBuildings.append(newBuilding);
	emit buildingCreated(newBuilding);
	return true;
}

void MapManager::requestDestroyBuilding(Building *building)
{
	if(building->getBuildingType() == BuildingZone){
		ZoneBuilding *zoneBuilding = (ZoneBuilding*)building;
		for(int i=0;i<zoneBuilding->getCoveringZones().count();i++){
			zoneBuilding->getCoveringZones().value(i)->setBuilding(nullptr);
		}
		zoneBuildings.removeAll(zoneBuilding);
		delete building;
	}
}

QList<ZoneBuilding*> MapManager::getZoneBuildings()
{
	return zoneBuildings;
}

TerrainSquare ***MapManager::getTerrainGrid()
{
	return terrainGrid;
}

ZoneSquare ***MapManager::getZoneGrid()
{
	return zoneGrid;
}

RoadSquare ***MapManager::getRoadGrid()
{
	return roadGrid;
}
