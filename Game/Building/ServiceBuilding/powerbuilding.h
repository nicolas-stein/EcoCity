#ifndef POWERBUILDING_H
#define POWERBUILDING_H

#include "servicebuilding.h"
#include <QPainter>

class PowerBuilding : public ServiceBuilding
{
public:
	PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, Buildings::Service::PowerType powerType);

	Buildings::Service::PowerType getPowerType() const;
	void updatePixmap(bool showToolTips);

	double getPollution() const;

	double getCost() const;
	double getOperationalCost() const;

	double getBasePowerProduction() const;
	double getCurrentPowerProduction();

private :
	Buildings::Service::PowerType powerType;
	double basePowerProduction;
	double currentPowerProduction = 0;
};

#endif // POWERBUILDING_H
