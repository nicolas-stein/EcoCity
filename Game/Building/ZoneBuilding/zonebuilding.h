#ifndef ZONEBUILDING_H
#define ZONEBUILDING_H

#include "../building.h"

class ZoneSquare;

//Classe représentant un batiment d'une zone (résidentielle, commercial, industrielle)
class ZoneBuilding : public Building
{
public:
	ZoneBuilding(int posX, int posY, int width, int height, Grid::Zone::Type zoneType, RessourceManager *ressourceManager);

	Grid::Zone::Type getZoneType() const;

	void addCoveringZone(ZoneSquare* zoneSquare);
	const QList<ZoneSquare *> &getCoveringZones() const;

	void updatePixmap(bool showToolTips);

	double getPowerConsumption() const;
	void setConnectedToPower(bool isConnectedToPower);

protected:
	int widthGrid();					//Renvoie la largueur du batiment en nombre de zones
	int heightGrid();					//Renvoie la hauteur du batiment en nombre de zones
	unsigned randomPixmapNumber;		//Nombre aléatoire permettant d'avoir différents style de texture pour un même type de batiment
	double powerConsumption = 0;		//Consommation énergétique du batiment

private :
	QList<ZoneSquare*> coveringZones;	//Liste des carrés de zone que le batiment recouvre
	Grid::Zone::Type zoneType;			//Type de zone du batiment
};

#endif // ZONEBUILDING_H
