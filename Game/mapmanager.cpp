#include "mapmanager.h"

MapManager::MapManager()
{

}

MapManager::~MapManager()
{
	//Delete terrain
	for(int x=0;x<TERRAIN_GRID_SIZE;x++){
		for(int y=0;y<TERRAIN_GRID_SIZE;y++){
			delete terrainGrid[x][y];
		}
		delete[] terrainGrid[x];
	}
	delete[] terrainGrid;

	//Delete roads
	for(int x=0;x<ROAD_GRID_SIZE;x++){
		for(int y=0;y<ROAD_GRID_SIZE;y++){
			if(roadGrid[x][y] != nullptr){
				delete roadGrid[x][y];
			}
		}
		delete[] roadGrid[x];
	}
	delete[] roadGrid;

	//Delete zones
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zoneGrid[x][y] != nullptr){
                if(zoneGrid[x][y]->getBuilding()!=nullptr){
                    ZoneBuilding *building = zoneGrid[x][y]->getBuilding();
                    for(int i=0;i<building->getCoveringZones().size();i++){
                        building->getCoveringZones().value(i)->setBuilding(nullptr);
                    }
                    delete building;
                }
				delete zoneGrid[x][y];
			}
		}
		delete[] zoneGrid[x];
	}
	delete[] zoneGrid;

    //Delete service buildings
    for(int i=0;i<serviceBuildings.size();i++){
        delete serviceBuildings[i];
    }
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

GridSquare* MapManager::requestBuildSquare(GridSquare *square, double money)
{
	if(square->getGridType()==GridRoad){
		RoadSquare *roadSquare = (RoadSquare*)square;
		if(money < roadSquare->getCost()){
			return nullptr;
		}

		if(roadGrid[roadSquare->getPosX()/ROAD_SQUARE_SIZE][roadSquare->getPosY()/ROAD_SQUARE_SIZE] == nullptr && !isThereServiceBuilding(roadSquare->getPosX(), roadSquare->getPosY())){
			if(terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grass || terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Sand ||terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Rock){

				RoadSquare *newSquare = new RoadSquare(roadSquare->getPosX(), roadSquare->getPosY(), roadSquare->getRoadType(), ressourceManager);
				int x = newSquare->getPosX()/ROAD_SQUARE_SIZE;
				int y = newSquare->getPosY()/ROAD_SQUARE_SIZE;
				roadGrid[x][y] = newSquare;
				roadCount++;
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

				for(int i=0;i<serviceBuildings.size();i++){
					serviceBuildings.at(i)->updateConnectedToRoad(roadGrid);
				}

				return newSquare;
			}
		}
	}
	return nullptr;
}

Building *MapManager::requestBuildBuilding(Building *building, double money)
{
	if(building->getBuildingType() == BuildingService){
		ServiceBuilding *serviceBuilding = (ServiceBuilding*) building;
		if(money >= serviceBuilding->getCost() && canPutServiceBuilding(serviceBuilding)){
			ServiceBuilding* newBuilding = nullptr;
			if(serviceBuilding->getServiceType() == Power){
				newBuilding = new PowerBuilding(serviceBuilding->getPosX(), serviceBuilding->getPosY(), ressourceManager, ((PowerBuilding*)serviceBuilding)->getPowerType());
				serviceBuildings.append(newBuilding);
			}

			for(int i = serviceBuilding->getPosX()/ZONE_SQUARE_SIZE; i < (serviceBuilding->getPosX()+serviceBuilding->getWidth())/ZONE_SQUARE_SIZE; i++){
				for(int j = serviceBuilding->getPosY()/ZONE_SQUARE_SIZE; j < (serviceBuilding->getPosY()+serviceBuilding->getHeight())/ZONE_SQUARE_SIZE; j++){
					if(zoneGrid[i][j] != nullptr && zoneGrid[i][j]->getZoneType() != None){
						zoneGrid[i][j]->setZoneType(None);
						if(zoneGrid[i][j]->getBuilding()!=nullptr){
							ZoneBuilding *building = zoneGrid[i][j]->getBuilding();
							for(int i=0;i<building->getCoveringZones().count();i++){
								building->getCoveringZones().value(i)->setBuilding(nullptr);
							}
							emit buildingRemoved(building);
							delete building;
						}
					}
				}
			}
			newBuilding->updateConnectedToRoad(roadGrid);
			newBuilding->updatePixmap(true);
			return newBuilding;
		}
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
	roadCount--;
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

	for(int i=0;i<serviceBuildings.size();i++){
		serviceBuildings.at(i)->updateConnectedToRoad(roadGrid);
	}
}

bool MapManager::generateNewZoneBuilding(ZoneType zoneType)
{
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zoneGrid[x][y] == nullptr || zoneGrid[x][y]->getZoneType()==None || zoneGrid[x][y]->getBuilding() != nullptr){
				continue;
			}
			else if(zoneType == Residential){
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
	newBuilding->updatePixmap(true);
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

bool MapManager::canPutServiceBuilding(ServiceBuilding *building)
{
	int x = building->getPosX();
	int y = building->getPosY();
	int width = building->getWidth();
	int height = building->getHeight();

	if(x < 0 || y < 0 || x+width > (TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE) || y+height > (TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE)){
		return false;
	}

	//Vérification du terrain
	for(int i=x/TERRAIN_SQUARE_SIZE; i < (x+width)/TERRAIN_SQUARE_SIZE; i++){
		for(int j=y/TERRAIN_SQUARE_SIZE; j < (y+height)/TERRAIN_SQUARE_SIZE ; j++){
			if(terrainGrid[i][j]->getTerrainType() == Water || terrainGrid[i][j]->getTerrainType() == Mountain){
				return false;
			}
		}
	}
	
	//Vérification des routes
	for(int i = x/ROAD_SQUARE_SIZE ; i < (x+width)/ROAD_SQUARE_SIZE ; i++){
		for(int j = y/ROAD_SQUARE_SIZE; j < (y+height)/ROAD_SQUARE_SIZE; j++){
			if(roadGrid[i][j] != nullptr){
				return false;
			}
		}
	}

	//Vérification des autres buildings
	for(int i=0;i<serviceBuildings.size();i++){
		if(x >= serviceBuildings.at(i)->getPosX() && x < serviceBuildings.at(i)->getPosX() + serviceBuildings.at(i)->getWidth()
				&& y >= serviceBuildings.at(i)->getPosY() && y < serviceBuildings.at(i)->getPosY() + serviceBuildings.at(i)->getHeight()){
			return false;
		}
		else if(x+width-1 >= serviceBuildings.at(i)->getPosX() && x+width-1 < serviceBuildings.at(i)->getPosX() + serviceBuildings.at(i)->getWidth()
				&& y >= serviceBuildings.at(i)->getPosY() && y < serviceBuildings.at(i)->getPosY() + serviceBuildings.at(i)->getHeight()){
			return false;
		}
		else if(x >= serviceBuildings.at(i)->getPosX() && x < serviceBuildings.at(i)->getPosX() + serviceBuildings.at(i)->getWidth()
				&& y+height-1 >= serviceBuildings.at(i)->getPosY() && y+height-1 < serviceBuildings.at(i)->getPosY() + serviceBuildings.at(i)->getHeight()){
			return false;
		}
		else if(x+width-1 >= serviceBuildings.at(i)->getPosX() && x+width-1 < serviceBuildings.at(i)->getPosX() + serviceBuildings.at(i)->getWidth()
				&& y+height-1 >= serviceBuildings.at(i)->getPosY() && y+height-1 < serviceBuildings.at(i)->getPosY() + serviceBuildings.at(i)->getHeight()){
			return false;
		}
	}

	return true;
}

bool MapManager::isThereServiceBuilding(int x, int y)
{
	for(int i=0;i<serviceBuildings.size();i++){
		if(x >= serviceBuildings.at(i)->getPosX() && x < serviceBuildings.at(i)->getPosX() + serviceBuildings.at(i)->getWidth()
				&& y >= serviceBuildings.at(i)->getPosY() && y < serviceBuildings.at(i)->getPosY() + serviceBuildings.at(i)->getHeight()){
			return true;
		}
	}

	return false;
}

void MapManager::requestDestroyBuilding(Building *building)
{
	if(building->getBuildingType() == BuildingZone){
		ZoneBuilding *zoneBuilding = (ZoneBuilding*) building;
		for(int i=0;i<zoneBuilding->getCoveringZones().count();i++){
			zoneBuilding->getCoveringZones().value(i)->setBuilding(nullptr);
		}
		zoneBuildings.removeAll(building);
	}
	else if(building->getBuildingType() == BuildingService){
		serviceBuildings.removeAll(building);
	}

	delete building;
}

QList<ZoneBuilding*> MapManager::getZoneBuildings()
{
	return zoneBuildings;
}

QList<ServiceBuilding *> MapManager::getServiceBuildings()
{
	return serviceBuildings;
}

Building *MapManager::getBuildingFromPos(int posX, int posY)
{
	Building *currentBuilding;
	for(int i=0;i<zoneBuildings.size();i++){
		currentBuilding = zoneBuildings.at(i);
		if(posX >= currentBuilding->getPosX() && posX < currentBuilding->getPosX()+currentBuilding->getWidth() && posY >= currentBuilding->getPosY() && posY < currentBuilding->getPosY()+currentBuilding->getHeight()){
			return currentBuilding;
		}
	}

	for(int i=0;i<serviceBuildings.size();i++){
		currentBuilding = serviceBuildings.at(i);
		if(posX >= currentBuilding->getPosX() && posX < currentBuilding->getPosX()+currentBuilding->getWidth() && posY >= currentBuilding->getPosY() && posY < currentBuilding->getPosY()+currentBuilding->getHeight()){
			return currentBuilding;
		}
	}

	return nullptr;
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

int MapManager::getRoadCount()
{
	return roadCount;
}
