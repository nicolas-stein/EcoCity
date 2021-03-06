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
	//On utilise un générateur de bruit "PerlinNoise" pour générer la carte de manière aléatoire
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
	//Si on veut construire un carré de route
	if(square->getGridType()==Grid::Type::GridRoad){
		RoadSquare *roadSquare = (RoadSquare*)square;
		//Si on a pas assez d'argent on ne peut pas construire le carrée
		if(money < roadSquare->getCost()){
			return nullptr;
		}

		//On vérifie qu'on peut construire le carré (qu'il n'y a pas déjà une route à cette endroit ou un batiment de service)
		if(roadGrid[roadSquare->getPosX()/ROAD_SQUARE_SIZE][roadSquare->getPosY()/ROAD_SQUARE_SIZE] == nullptr && !isThereServiceBuilding(roadSquare->getPosX(), roadSquare->getPosY())){
			//On vérifie que le terrain est propice à la construction d'une route (que ce n'est pas une montagne ou de l'eau)
			if(terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grid::Terrain::Type::Grass || terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grid::Terrain::Type::Sand ||terrainGrid[roadSquare->getPosX()/TERRAIN_SQUARE_SIZE][roadSquare->getPosY()/TERRAIN_SQUARE_SIZE]->getTerrainType() == Grid::Terrain::Type::Rock){

				//On créé le carré de route et on met à jour les routes adjacentes, on créé les zones le long de la route
				RoadSquare *newSquare = new RoadSquare(roadSquare->getPosX(), roadSquare->getPosY(), roadSquare->getRoadType(), ressourceManager);
				int x = newSquare->getPosX()/ROAD_SQUARE_SIZE;
				int y = newSquare->getPosY()/ROAD_SQUARE_SIZE;
				roadGrid[x][y] = newSquare;
				roadCount++;
				newSquare->updatePixmap(roadGrid);
				updateAdjacentRoadPixmaps(x, y, 1);
				updateAdjacentRoadZones(newSquare);

				int zoneX, zoneY;

				//Si on a construit la route sur des zones, on les effaces
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
	//Si on veut construire un batiment de service
	if(building->getBuildingType() == Buildings::Type::BuildingService){
		ServiceBuilding *serviceBuilding = (ServiceBuilding*) building;
		//On vérifie qu'on a assez d'argent et qu'on peut bel et bien construire le batiment voulu
		if(money >= serviceBuilding->getCost() && canPutServiceBuilding(serviceBuilding)){
			//On créé le batiment
			ServiceBuilding* newBuilding = nullptr;
			if(serviceBuilding->getServiceType() == Buildings::Service::Type::Power){
				newBuilding = new PowerBuilding(serviceBuilding->getPosX(), serviceBuilding->getPosY(), ressourceManager, ((PowerBuilding*)serviceBuilding)->getPowerType());
				serviceBuildings.append(newBuilding);
			}

			//Si le batiment a été construit sur un endroit où il y avait des zones => on change le type de ces zones en None pour éviter que des batiments de zone se constuise sur notre batiment de service
			for(int i = serviceBuilding->getPosX()/ZONE_SQUARE_SIZE; i < (serviceBuilding->getPosX()+serviceBuilding->getWidth())/ZONE_SQUARE_SIZE; i++){
				for(int j = serviceBuilding->getPosY()/ZONE_SQUARE_SIZE; j < (serviceBuilding->getPosY()+serviceBuilding->getHeight())/ZONE_SQUARE_SIZE; j++){
					if(zoneGrid[i][j] != nullptr && zoneGrid[i][j]->getZoneType() != Grid::Zone::Type::None){
						zoneGrid[i][j]->setZoneType(Grid::Zone::Type::None);
						if(zoneGrid[i][j]->getBuilding()!=nullptr){
							ZoneBuilding *building = zoneGrid[i][j]->getBuilding();
							for(int i=0;i<building->getCoveringZones().count();i++){
								building->getCoveringZones().value(i)->setBuilding(nullptr);
							}
							zoneBuildings.removeAll(building);
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
	//Quand on supprime une zone, on détruit le batiment qui était dessus s'il y en avait un
	emit zoneSquareRemoved(zoneGrid[zoneX][zoneY]);
	if(zoneGrid[zoneX][zoneY]->getBuilding()!=nullptr){
		requestDestroyBuilding(zoneGrid[zoneX][zoneY]->getBuilding());
	}

	//On libère la mémoire
	delete zoneGrid[zoneX][zoneY];
	zoneGrid[zoneX][zoneY] = nullptr;
}

void MapManager::updateAdjacentRoadPixmaps(int x, int y, int recursive)
{
	//On met à jour les pixmap des routes adjacentes (fonction récursive)
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
	//Mise à jour des zones adjacentes à la route (créé les zones lorsque l'on place une route)
	int zoneX, zoneY;
	//Si la route n'est pas connecté en bas, on créé les zones en bas
	if(!roadSquare->isConnectedBottom() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX() + i*ZONE_SQUARE_SIZE;
				zoneY = (roadSquare->getPosY()+ROAD_SQUARE_SIZE) + j*ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					Grid::Terrain::Type terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Grid::Terrain::Type::Water || terrainType == Grid::Terrain::Type::Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, Grid::Zone::Type::None, ressourceManager);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	//Si la route n'est pas connecté en haut, on créé les zones en haut
	if(!roadSquare->isConnectedTop() && roadSquare->getPosY() > 0){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = roadSquare->getPosX() + i*ZONE_SQUARE_SIZE;
				zoneY = (roadSquare->getPosY()-ZONE_SQUARE_SIZE) - j*ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					Grid::Terrain::Type terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Grid::Terrain::Type::Water || terrainType == Grid::Terrain::Type::Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, Grid::Zone::Type::None, ressourceManager);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	//Si la route n'est pas connecté à gauche, on créé les zones à gauche
	if(!roadSquare->isConnectedLeft() && roadSquare->getPosX() > 0 ){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = (roadSquare->getPosX() - ZONE_SQUARE_SIZE) - j*ZONE_SQUARE_SIZE;
				zoneY = roadSquare->getPosY() + i* ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					Grid::Terrain::Type terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Grid::Terrain::Type::Water || terrainType == Grid::Terrain::Type::Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, Grid::Zone::Type::None, ressourceManager);
						zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] = newZoneSquare;
						emit zoneSquareCreated(newZoneSquare);
					}
				}
			}
		}
	}
	//Si la route n'est pas connecté à droite, on créé les zones à droite
	if(!roadSquare->isConnectedRight() && roadSquare->getPosY() < ROAD_GRID_SIZE*ROAD_SQUARE_SIZE){
		for(int i=0;i<ROAD_SQUARE_SIZE/ZONE_SQUARE_SIZE;i++){
			for(int j=0;j<ROAD_ZONE_RANGE;j++){
				zoneX = (roadSquare->getPosX() + ROAD_SQUARE_SIZE) + j*ZONE_SQUARE_SIZE;
				zoneY = roadSquare->getPosY() + i* ZONE_SQUARE_SIZE;
				if((zoneX < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && (zoneY < ZONE_GRID_SIZE*ZONE_SQUARE_SIZE) && zoneX >= 0 && zoneY >= 0 && zoneGrid[zoneX/ZONE_SQUARE_SIZE][zoneY/ZONE_SQUARE_SIZE] == nullptr && roadGrid[zoneX/ROAD_SQUARE_SIZE][zoneY/ROAD_SQUARE_SIZE] == nullptr){
					Grid::Terrain::Type terrainType = terrainGrid[zoneX/TERRAIN_SQUARE_SIZE][zoneY/TERRAIN_SQUARE_SIZE]->getTerrainType();
					if(terrainType == Grid::Terrain::Type::Water || terrainType == Grid::Terrain::Type::Mountain){
						break;
					}
					else{
						ZoneSquare *newZoneSquare = new ZoneSquare(zoneX, zoneY, Grid::Zone::Type::None, ressourceManager);
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
	//Si on veut détruire une route
	roadGrid[roadSquare->getPosX()/ROAD_SQUARE_SIZE][roadSquare->getPosY()/ROAD_SQUARE_SIZE] = nullptr;
	roadCount--;
	//On met à jour le pixmap des routes adjacentes
	updateAdjacentRoadPixmaps(roadSquare->getPosX()/ROAD_SQUARE_SIZE, roadSquare->getPosY()/ROAD_SQUARE_SIZE, 1);

	//On supprime les zones de la route
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

bool MapManager::generateNewZoneBuilding(Grid::Zone::Type zoneType)
{
	//On souhaite rajouter un batiment de zone à la carte
	//On regarde où on pourrait le mettre (en fonction de la taille qu'il pourrait prendre)
	for(int x=0;x<ZONE_GRID_SIZE;x++){
		for(int y=0;y<ZONE_GRID_SIZE;y++){
			if(zoneGrid[x][y] == nullptr || zoneGrid[x][y]->getZoneType()==Grid::Zone::Type::None || zoneGrid[x][y]->getBuilding() != nullptr){
				continue;
			}
			else if(zoneType == Grid::Zone::Type::Residential){
				if(putZoneBuilding(x, y, 8, 4, zoneType) || putZoneBuilding(x, y, 4, 8, zoneType) || putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 1, 2, zoneType) || putZoneBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Grid::Zone::Type::Commercial){
				if(putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 1, 2, zoneType) || putZoneBuilding(x, y, 2, 1, zoneType)){
					return true;
				}
			}
			else if(zoneType == Grid::Zone::Type::Industrial){
				if(putZoneBuilding(x, y, 4, 4, zoneType) || putZoneBuilding(x, y, 3, 2, zoneType) || putZoneBuilding(x, y, 2, 3, zoneType)){
					return true;
				}
			}
		}
	}
	return false;
}

bool MapManager::putZoneBuilding(int x, int y, int widthGrid, int heightGrid, Grid::Zone::Type zoneType)
{
	//On essaie de mettre un batiment de zone à ces coordonnées
	bool anyAdjacentToRoad = false;
	if(x < 0 || y < 0 || x+widthGrid >= ZONE_GRID_SIZE || y+heightGrid >= ZONE_GRID_SIZE){
		//Si on sort de la carte
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

	//Si il y a assez de zones libres pour mettre le batiment et qu'au moins une des zones est adjacente à une route alors on peut créer le batiment
	ZoneBuilding *newBuilding;
	if(zoneType==Grid::Zone::Type::Residential){
		newBuilding = new ResidentialBuilding(zoneGrid[x][y]->getPosX(), zoneGrid[x][y]->getPosY(), ZONE_SQUARE_SIZE*widthGrid, ZONE_SQUARE_SIZE*heightGrid, ressourceManager);
	}
	else if(zoneType==Grid::Zone::Type::Commercial){
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
	//On vérifie si on peut poser le batiment de service
	int x = building->getPosX();
	int y = building->getPosY();
	int width = building->getWidth();
	int height = building->getHeight();

	if(x < 0 || y < 0 || x+width > (TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE) || y+height > (TERRAIN_SQUARE_SIZE*TERRAIN_GRID_SIZE)){
		return false;
	}

	//Vérification du terrain (on ne peut pas poser le batiment si il y a de l'eau ou une montagne)
	for(int i=x/TERRAIN_SQUARE_SIZE; i < (x+width)/TERRAIN_SQUARE_SIZE; i++){
		for(int j=y/TERRAIN_SQUARE_SIZE; j < (y+height)/TERRAIN_SQUARE_SIZE ; j++){
			if(terrainGrid[i][j]->getTerrainType() == Grid::Terrain::Type::Water || terrainGrid[i][j]->getTerrainType() == Grid::Terrain::Type::Mountain){
				return false;
			}
		}
	}
	
	//Vérification des routes (on ne peut pas poser le batiment sur une route)
	for(int i = x/ROAD_SQUARE_SIZE ; i < (x+width)/ROAD_SQUARE_SIZE ; i++){
		for(int j = y/ROAD_SQUARE_SIZE; j < (y+height)/ROAD_SQUARE_SIZE; j++){
			if(roadGrid[i][j] != nullptr){
				return false;
			}
		}
	}

	//Vérification des autres buildings (on ne peut pas poser le batiment de service sur d'autres batiments de service)
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
	//On parcourt la liste des batiments de service pour vérifier si il y en a un qui serait présent aux coordonnées
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
	//On demande à détruire un batiment
	//Si c'est un batiment de zone il faut mettre à jour toutes ses zones pour leur dire qu'elles n'ont plus de batiment
	if(building->getBuildingType() == Buildings::Type::BuildingZone){
		ZoneBuilding *zoneBuilding = (ZoneBuilding*) building;
		for(int i=0;i<zoneBuilding->getCoveringZones().count();i++){
			zoneBuilding->getCoveringZones().value(i)->setBuilding(nullptr);
		}
		zoneBuildings.removeAll(building);
	}
	else if(building->getBuildingType() == Buildings::Type::BuildingService){
		//Si c'est un batiment de service, on l'enlève simplement de la liste
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
	//On parcourt la liste des batiments de zone et des batiments de service pour chercher si il y a un batiment aux coordonnées
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
