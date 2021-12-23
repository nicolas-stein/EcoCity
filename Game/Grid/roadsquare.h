#ifndef ROADSQUARE_H
#define ROADSQUARE_H

#include "gridsquare.h"
#include "Constants.h"

class RoadSquare : public GridSquare
{
public:
	RoadSquare(int posX, int posY, RoadType roadType, QMap<RoadType, QMap<RoadAngle, QPixmap>> *roadPixmaps);
	void updatePixmap(RoadSquare ***roadGrid);

	RoadType getRoadType() const;

	bool isConnectedBottom() const;
	bool isConnectedTop() const;
	bool isConnectedLeft() const;
	bool isConnectedRight() const;

private :
	RoadType roadType;

	bool connectedTop = false;
	bool connectedBottom = false;
	bool connectedLeft = false;
	bool connectedRight = false;


	QMap<RoadType, QMap<RoadAngle, QPixmap>> *roadPixmaps;
};

#endif // ROADSQUARE_H
