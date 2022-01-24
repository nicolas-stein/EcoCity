#ifndef GRIDSQUARE_H
#define GRIDSQUARE_H

#include "constants.h"
#include "Game/ressourcemanager.h"
#include "customgraphicspixmapitem.h"

//Classe représentant une case
class GridSquare
{

public:
	GridSquare(Grid::Type gridType, int posX, int posY, int width, int height, RessourceManager *ressourceManager);
	virtual ~GridSquare(){};

	Grid::Type getGridType() const;				//Retourne le type de case (terrain, zone, route)
	CustomGraphicsPixmapItem *getPixmapItem();	//

	int getPosX() const;
	int getPosY() const;
	void setPos(int posX, int newPosY);


private:
	Grid::Type gridType;
protected :
	CustomGraphicsPixmapItem pixmapItem;	//GraphicsPixmapItem associé à la case (va être affiche sur la scène du jeu)
	int posX, posY, width, height;
	RessourceManager *ressourceManager;		//Gestionnaire de ressource (utilisé pour charger la texture de la case)
};

#endif // GRIDSQUARE_H
