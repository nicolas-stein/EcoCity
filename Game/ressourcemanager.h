#ifndef RESSOURCEMANAGER_H
#define RESSOURCEMANAGER_H

#include "constants.h"

#include <QMap>
#include <QPixmap>
#include <QRandomGenerator>
#include <QDebug>

//Classe représentant le gestionnaire des ressources (graphique) du programme
class RessourceManager
{
public:
	RessourceManager();

	void loadRessources();							//Chargement de toutes les ressources

	QMap<Tooltips, QPixmap> *getTooltipsPixmaps();

	QMap<Grid::Terrain::Type, QPixmap> *getTerrainPixmaps();

	QMap<Grid::Road::Type, QMap<Grid::Road::Angle, QPixmap>> *getRoadPixmaps();

	QMap<Grid::Zone::Type, QPixmap> *getZonePixmaps();

	QMap<Grid::Zone::Type, QList<QPixmap>> *getZoneBuildingPixmaps();

	QPixmap getZoneBuildingPixmap(Grid::Zone::Type zoneType, int widthGrid, int heightGrid, unsigned randomPixmapNumber);	//Retourne la texture d'un batiment de zone (avec un numéro aléatoire pour les batiments qui ont différents styles)

	QMap<Buildings::Service::PowerType, QPixmap> *getPowerBuildingPixmaps();

private:
	QMap<Tooltips, QPixmap> tooltipsPixmaps;								//Textures des info-bulles

	QMap<Grid::Terrain::Type, QPixmap> terrainPixmaps;						//Textures du terrain
	QMap<Grid::Road::Angle, QPixmap> twoLanesPixmaps;						//Textures des routes à deux voies
	QMap<Grid::Road::Type, QMap<Grid::Road::Angle, QPixmap>> roadPixmaps;	//Textures des routes
	QMap<Grid::Zone::Type, QPixmap> zonePixmaps;							//Textures des zones

	QMap<Grid::Zone::Type, QList<QPixmap>> zoneBuildingPixmaps;				//Textures des batiments de zone
	QList<QPixmap> residentialPixmaps;										//Textures des batiments résidentiels
	QList<QPixmap> commercialPixmaps;										//Textures des batiments commerciaux
	QList<QPixmap> industrialPixmaps;										//Textures des batiments industriels

	QMap<Buildings::Service::PowerType, QPixmap> powerBuildingPixmaps;		//Textures des batiments producteurs d'électricité

	void loadTooltipsPixmaps();		//On charge les textures des info-bulles

	void loadTerrainPixmaps();		//On charge les textures du terrain
	void loadZonePixmaps();			//On charge les textures des zones

	void loadTwoLaneRoadPixmaps();	//On charge les textures des routes à deux voies

	void loadResidentialPixmaps();	//On charge les textures des batiments résidentiel
	void loadCommercialPixmaps();	//On charge les textures des batiments commerciaux
	void loadIndustrialPixmaps();	//On charge les textures des batiments industriels

	void loadPowerPixmaps();		//On charge les textures des batiments producteurs d'électricité
};

#endif // RESSOURCEMANAGER_H
