#ifndef BUILDCURSOR_H
#define BUILDCURSOR_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "Game/mapmanager.h"
#include "Game/Grid/gridsquare.h"
#include "Game/Building/building.h"

#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QApplication>


//Classe représentant le curseur de construction
class BuildCursor : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
public:
	BuildCursor(QObject *parent = nullptr);
	~BuildCursor();
	void setSquareToBuild(GridSquare *newSquareToBuild);
	void setBuildingToBuild(Building *building);
	void setPosition(int x, int y);								//Changement de la position du curseur (quand on déplace la souris dans la scène)

	void setManagers(RessourceManager *ressourceManager, MapManager *mapManager);	//On passe les différents gestionnaires
	void freeSquareBuildingToBuild();							//Libère la mémoire lorsqu'on a plus besoin du curseur

private :
	GridSquare *squareToBuild = nullptr;						//Carré que l'on souhaite construire (route ou zone)
	Building *buildingToBuild = nullptr;						//Batiment que l'on souhaite construire

private:
	RessourceManager *ressourceManager;							//Gestionnaire de ressource (utilisé pour charger la texture du curseur)
	MapManager *mapManager;										//Gestionnaire de la carte

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);		//Lors d'un clique sur ce curseur
signals:
	void requestBuildSquare(GridSquare *square);				//Signal permettant de demander la construction d'un carré (route)
	void requestBuildBuilding(Building *building);				//Signal permettant de demander la construction d'un batiment
	void changeZoneSquareType(ZoneSquare *zoneSquare, bool wholeArea);	//Signal permettant de demander de changer le type d'une zone
};

#endif // BUILDCURSOR_H
