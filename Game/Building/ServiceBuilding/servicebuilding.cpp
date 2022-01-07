#include "servicebuilding.h"

ServiceBuilding::ServiceBuilding(int posX, int posY, ServiceBuildingType serviceType, RessourceManager *ressourceManager): Building(posX, posY, 0, 0, BuildingService, ressourceManager), serviceType(serviceType)
{

}

ServiceBuildingType ServiceBuilding::getServiceType() const
{
	return serviceType;
}
