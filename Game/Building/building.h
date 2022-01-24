#ifndef BUILDING_H
#define BUILDING_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "customgraphicspixmapitem.h"

#include <QPainter>

//Classe représentant un batiment
class Building
{
public:
	Building(int posX, int posY, int width, int height, Buildings::Type buildingType, RessourceManager *ressourceManager);
    virtual ~Building(){};

	CustomGraphicsPixmapItem *getPixmapItem();
	Buildings::Type getBuildingType() const;

	void setPos(int newPosX, int newPosY);
	int getPosX() const;
	int getPosY() const;
	int getWidth() const;
	int getHeight() const;

	bool isConnectedToPower();

	virtual void updatePixmap(bool showToolTips) = 0;
	virtual double getPollution() const = 0;
private :
	int posX;						//Coordonnée X du batiment
	int posY;						//Coordonnée Y du batiment
	Buildings::Type buildingType;	//Type de batiment (Zone ou Service)


protected :
	RessourceManager *ressourceManager;		//Gestionnaire de ressource (utilisé pour charger la texture du batiment)
	CustomGraphicsPixmapItem pixmapItem;	//GraphicsPixmapItem associé au batiment (va être affiche sur la scène du jeu)
	int width;								//Largeur du batiment
	int height;								//Hauteur du batiment
	bool connectedToPower = false;			//Alimenté en électricité
};

#endif // BUILDING_H
