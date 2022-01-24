#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "constants.h"
#include "PerlinNoise.hpp"
#include "Game/Grid/terrainsquare.h"
#include "Game/Grid/zonesquare.h"
#include "Game/Grid/roadsquare.h"
#include "Game/Building/ZoneBuilding/residentialbuilding.h"
#include "Game/Building/ZoneBuilding/commercialbuilding.h"
#include "Game/Building/ZoneBuilding/industrialbuilding.h"
#include "Game/Building/ServiceBuilding/powerbuilding.h"

#include <QObject>
#include <QList>

//Classe représentant le gestionnaire de la carte
class MapManager: public QObject
{
	Q_OBJECT
public:
	MapManager();
	~MapManager();

	void initMap(RessourceManager *ressourceManager);	//Initialisation de la carte

	void removeZoneSquare(int zoneX, int zoneY);		//Suppression d'un carré de zone

	TerrainSquare ***getTerrainGrid();
	ZoneSquare ***getZoneGrid();
	RoadSquare ***getRoadGrid();
	int getRoadCount();
	QList<ZoneBuilding*> getZoneBuildings();
	QList<ServiceBuilding*> getServiceBuildings();
	Building *getBuildingFromPos(int posX, int posY);

	GridSquare *requestBuildSquare(GridSquare *square, double money);	//Demande de création d'un carré
	Building *requestBuildBuilding(Building *building, double money);	//Demande de création d'un batiment
	void updateAdjacentRoadPixmaps(int x, int y, int recursive);		//Mise à jour du pixmap des routes adjacentes
	void updateAdjacentRoadZones(RoadSquare *roadSquare);				//Mise à jours des zones des routes adjacentes
	void requestDestroyRoad(RoadSquare *roadSquare);					//Demande de destruction d'une route

	bool generateNewZoneBuilding(Grid::Zone::Type zoneType);			//Demande de création d'un batiment de zone
	bool putZoneBuilding(int x, int y, int widthGrid, int heightGrid, Grid::Zone::Type zoneType);	//Créé si possible un batiment de zone
	bool canPutServiceBuilding(ServiceBuilding *building);				//Est-ce que le batiment de service peut être posé là ?
	bool isThereServiceBuilding(int x, int y);							//Est-ce qu'il y a un batiment de service à ces coordonnées ?
	void requestDestroyBuilding(Building *building);					//Demande de destruction d'un batiment de service

private:
	TerrainSquare ***terrainGrid;				//Grille du terrain (tableau 2D de pointeurs sur TerrainSquare)
	RoadSquare ***roadGrid;						//Grille des routes (tableau 2D de pointeurs sur RoadSquare)
	ZoneSquare ***zoneGrid;						//Grille des zones (tableau 2D de pointeurs sur ZoneSquare)
	int roadCount = 0;							//Nombre de routes (pour évité de parcourir tout le tableau roadGrid pour les compter)

	QList<ZoneBuilding*> zoneBuildings;			//Liste de tous les batiments de zone
	QList<ServiceBuilding*> serviceBuildings;	//Liste de tous les batiments de service

	RessourceManager *ressourceManager;			//Gestionnaire de ressources

signals:
	void zoneSquareCreated(ZoneSquare *zoneSquare);	//Signal indiquant qu'un carrée de zone a été créé par le gestionnaire de carte
	void zoneSquareRemoved(ZoneSquare *zoneSquare);	//Signal indiquant qu'un carré de zone a été supprimé par le gestionnaire de carte
	void buildingCreated(Building *newBuilding);	//Signal indiquant qu'un batiment a été créé par le gestionnaire de carte
	void buildingRemoved(Building *building);		//Signal indiquant qu'un batiment a été supprimé par le gestionnaire de carte
};

#endif // MAPMANAGER_H
