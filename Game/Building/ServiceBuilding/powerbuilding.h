#ifndef POWERBUILDING_H
#define POWERBUILDING_H

#include "servicebuilding.h"
#include <QPainter>

//Classe représentant un batiment produisant de l'électricité
class PowerBuilding : public ServiceBuilding
{
public:
	PowerBuilding(int posX, int posY, RessourceManager *ressourceManager, Buildings::Service::PowerType powerType);

	Buildings::Service::PowerType getPowerType() const;
	void updatePixmap(bool showToolTips);

	double getPollution() const;

	double getCost() const;				//Cout de construction
	double getOperationalCost() const;	//Cout de maintenance (chaque mois)

	double getBasePowerProduction() const;
	double getCurrentPowerProduction();

private :
	Buildings::Service::PowerType powerType;	//Type de batiment électrique
	double basePowerProduction;					//Production d'énergie de base
	double currentPowerProduction = 0;			//Production d'énergie actuellement
};

#endif // POWERBUILDING_H
