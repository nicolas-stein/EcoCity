#ifndef POWERBUILDING_H
#define POWERBUILDING_H

#include "servicebuilding.h"
#include <QPainter>

class PowerBuilding : public ServiceBuilding
{
public:
	PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, PowerType powerType);

	PowerType getPowerType() const;
	void updatePixmap(bool showToolTips);

	int getPowerProduction() const;

private :
	PowerType powerType;
	double powerProduction;
};

#endif // POWERBUILDING_H
