#include "servicebuilding.h"

ServiceBuilding::ServiceBuilding(int posX, int posY, Buildings::Service::Type serviceType, RessourceManager *ressourceManager): Building(posX, posY, 0, 0, Buildings::Type::BuildingService, ressourceManager), serviceType(serviceType)
{

}

Buildings::Service::Type ServiceBuilding::getServiceType() const
{
	return serviceType;
}

void ServiceBuilding::updateConnectedToRoad(RoadSquare ***roadGrid)
{
	//On enregistre le previous state pour le comparer plus tard et savoir s'il faut ou non faire une mise à jour de la texture (pour afficher ou retirer l'info-bulle)
	bool previousState = connectedToRoad;

	//On parcours les bords du batiments et on regarde si une route existe
	for(int x=getPosX()/ROAD_SQUARE_SIZE;x < (getPosX()+width)/ROAD_SQUARE_SIZE;x++){
		if((getPosY()>0 && roadGrid[x][getPosY()/ROAD_SQUARE_SIZE-1] != nullptr) || (getPosY()+height < ROAD_SQUARE_SIZE*ROAD_GRID_SIZE && roadGrid[x][(getPosY()+height)/ROAD_SQUARE_SIZE] != nullptr)){
			connectedToRoad = true;
			if(previousState!=connectedToRoad){
				updatePixmap(true);
			}
			return;
		}
	}

	//On parcours les bords du batiments et on regarde si une route existe
	for(int y=getPosY()/ROAD_SQUARE_SIZE;y < (getPosY()+height)/ROAD_SQUARE_SIZE;y++){
		if((getPosX()>0 && roadGrid[getPosX()/ROAD_SQUARE_SIZE-1][y] != nullptr) || (getPosX()+width < ROAD_SQUARE_SIZE*ROAD_GRID_SIZE && roadGrid[(getPosX()+width)/ROAD_SQUARE_SIZE][y] != nullptr)){
			connectedToRoad = true;
			if(previousState!=connectedToRoad){
				updatePixmap(true);
			}
			return;
		}
	}

	//Si on a pas trouvé de route
	connectedToRoad = false;
	if(previousState!=connectedToRoad){
		updatePixmap(true);
	}
}

bool ServiceBuilding::isConnectedToRoad() const
{
	return connectedToRoad;
}
