#include "servicebuilding.h"

ServiceBuilding::ServiceBuilding(int posX, int posY, ServiceBuildingType serviceType, RessourceManager *ressourceManager): Building(posX, posY, 0, 0, BuildingService, ressourceManager), serviceType(serviceType)
{

}

ServiceBuildingType ServiceBuilding::getServiceType() const
{
	return serviceType;
}

void ServiceBuilding::updateConnectedToRoad(RoadSquare ***roadGrid)
{
	bool previousState = connectedToRoad;
	for(int x=getPosX()/ROAD_SQUARE_SIZE;x < (getPosX()+width)/ROAD_SQUARE_SIZE;x++){
		if(roadGrid[x][getPosY()/ROAD_SQUARE_SIZE-1] != nullptr || roadGrid[x][(getPosY()+height)/ROAD_SQUARE_SIZE] != nullptr){
			connectedToRoad = true;
			if(previousState!=connectedToRoad){
				updatePixmap(true);
			}
			return;
		}
	}

	for(int y=getPosY()/ROAD_SQUARE_SIZE;y < (getPosY()+height)/ROAD_SQUARE_SIZE;y++){
		if(roadGrid[getPosX()/ROAD_SQUARE_SIZE-1][y] != nullptr || roadGrid[(getPosX()+width)/ROAD_SQUARE_SIZE][y] != nullptr){
			connectedToRoad = true;
			if(previousState!=connectedToRoad){
				updatePixmap(true);
			}
			return;
		}
	}

	connectedToRoad = false;
	if(previousState!=connectedToRoad){
		updatePixmap(true);
	}
}
