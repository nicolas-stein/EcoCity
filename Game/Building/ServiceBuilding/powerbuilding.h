#ifndef POWERBUILDING_H
#define POWERBUILDING_H

#include "servicebuilding.h"

class PowerBuilding : public ServiceBuilding
{
public:
	PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, PowerType powerType);

	PowerType getPowerType() const;

private :
	PowerType powerType;
};

#endif // POWERBUILDING_H
