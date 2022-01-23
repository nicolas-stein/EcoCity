#include "ressourcemanager.h"

RessourceManager::RessourceManager()
{

}

void RessourceManager::loadRessources()
{
	loadTooltipsPixmaps();
	loadTerrainPixmaps();
	loadZonePixmaps();

	//Load roads
	loadTwoLaneRoadPixmaps();

	//Load buildings
	loadResidentialPixmaps();
	loadCommercialPixmaps();
	loadIndustrialPixmaps();

	loadPowerPixmaps();
}

QMap<Tooltips, QPixmap> *RessourceManager::getTooltipsPixmaps()
{
	return &tooltipsPixmaps;
}

QMap<Grid::Terrain::Type, QPixmap> *RessourceManager::getTerrainPixmaps()
{
	return &terrainPixmaps;
}

QMap<Grid::Road::Type, QMap<Grid::Road::Angle, QPixmap>> *RessourceManager::getRoadPixmaps()
{
	return &roadPixmaps;
}

QMap<Grid::Zone::Type, QPixmap> *RessourceManager::getZonePixmaps()
{
	return &zonePixmaps;
}

QMap<Grid::Zone::Type, QList<QPixmap> > *RessourceManager::getZoneBuildingPixmaps()
{
	return &zoneBuildingPixmaps;
}

QPixmap RessourceManager::getZoneBuildingPixmap(Grid::Zone::Type zoneType, int widthGrid, int heightGrid, unsigned randomPixmapNumber)
{
	if(zoneType == Grid::Zone::Residential){
		if(widthGrid == 4 && heightGrid == 4){
			return residentialPixmaps.value(0);
		}
		else if(widthGrid == 1 && heightGrid == 2){
			return residentialPixmaps.value(1);
		}
		else if(widthGrid == 2 && heightGrid == 1){
			return residentialPixmaps.value(2);
		}
		else if(widthGrid == 4 && heightGrid == 8){
			return residentialPixmaps.value(3);
		}
		else if(widthGrid == 8 && heightGrid == 4){
			return residentialPixmaps.value(4);
		}
	}
	else if(zoneType == Grid::Zone::Commercial){
		if(widthGrid == 4 && heightGrid == 4){
			return commercialPixmaps.value(0);
		}
		else if(widthGrid == 1 && heightGrid == 2){
			return commercialPixmaps.value(1);
		}
		else if(widthGrid == 2 && heightGrid == 1){
			return commercialPixmaps.value(2);
		}
	}
	else if(zoneType == Grid::Zone::Industrial){
		if(widthGrid == 4 && heightGrid == 4){
			return industrialPixmaps.value(0+randomPixmapNumber%2);
		}
		else if(widthGrid == 2 && heightGrid == 3){
			return industrialPixmaps.value(2);
		}
		else if(widthGrid == 3 && heightGrid == 2){
			return industrialPixmaps.value(3);
		}
	}

	qDebug() << "Error loading ressource : " << zoneType << " width : " << widthGrid << " height : " << heightGrid;

	return QPixmap();
}

QMap<Buildings::Service::PowerType, QPixmap> *RessourceManager::getPowerBuildingPixmaps()
{
	return &powerBuildingPixmaps;
}

void RessourceManager::loadTooltipsPixmaps()
{
	tooltipsPixmaps[NoRoad] = QPixmap(":/pixmaps/tooltips/no_road").scaled(ROAD_SQUARE_SIZE, ROAD_SQUARE_SIZE);
	tooltipsPixmaps[NoPower] = QPixmap(":/pixmaps/tooltips/no_power").scaled(ROAD_SQUARE_SIZE, ROAD_SQUARE_SIZE);
}

void RessourceManager::loadTerrainPixmaps()
{
	terrainPixmaps[Grid::Terrain::Type::Grass] = QPixmap(":/pixmaps/terrain/grassTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Grid::Terrain::Type::Mountain] = QPixmap(":/pixmaps/terrain/mountainTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Grid::Terrain::Type::Rock] = QPixmap(":/pixmaps/terrain/rockTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Grid::Terrain::Type::Water] = QPixmap(":/pixmaps/terrain/waterTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
	terrainPixmaps[Grid::Terrain::Type::Sand] = QPixmap(":/pixmaps/terrain/sandTile").scaled(TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN, TERRAIN_SQUARE_SIZE+TERRAIN_SQUARE_MARGIN);
}

void RessourceManager::loadZonePixmaps()
{
	zonePixmaps[Grid::Zone::Type::None] = QPixmap(":/pixmaps/zones/noneTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Grid::Zone::Type::Residential] = QPixmap(":/pixmaps/zones/residentialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Grid::Zone::Type::Commercial] = QPixmap(":/pixmaps/zones/commercialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
	zonePixmaps[Grid::Zone::Type::Industrial] = QPixmap(":/pixmaps/zones/industrialTile").scaled(ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN, ZONE_SQUARE_SIZE+ZONE_SQUARE_MARGIN);
}

void RessourceManager::loadTwoLaneRoadPixmaps()
{
	twoLanesPixmaps[Grid::Road::Angle::Straight] = QPixmap(":/pixmaps/roads/two-lanes/straight-1").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Grid::Road::Angle::Turn] = QPixmap(":/pixmaps/roads/two-lanes/turn").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Grid::Road::Angle::TJunction] = QPixmap(":/pixmaps/roads/two-lanes/T-junction").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Grid::Road::Angle::Cross] = QPixmap(":/pixmaps/roads/two-lanes/cross").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	twoLanesPixmaps[Grid::Road::Angle::DeadEnd] = QPixmap(":/pixmaps/roads/two-lanes/dead-end").scaled(ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN, ROAD_SQUARE_SIZE+ROAD_SQUARE_MARGIN);
	roadPixmaps[Grid::Road::Type::TwoLanes] = twoLanesPixmaps;
}

void RessourceManager::loadResidentialPixmaps()
{
	residentialPixmaps.append(QPixmap(":/pixmaps/zones/residential/house_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/zones/residential/caravan").scaled(ZONE_SQUARE_SIZE*1+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/zones/residential/caravan_rotate").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*1+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/zones/residential/skyscrapper_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN));
	residentialPixmaps.append(QPixmap(":/pixmaps/zones/residential/skyscrapper_2_rotate").scaled(ZONE_SQUARE_SIZE*8+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	zoneBuildingPixmaps[Grid::Zone::Type::Residential] = residentialPixmaps;
}

void RessourceManager::loadCommercialPixmaps()
{
	commercialPixmaps.append(QPixmap(":/pixmaps/zones/commercial/burger").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	commercialPixmaps.append(QPixmap(":/pixmaps/zones/commercial/food_truck").scaled(ZONE_SQUARE_SIZE*1+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	commercialPixmaps.append(QPixmap(":/pixmaps/zones/commercial/food_truck_rotate").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*1+BUILDING_MARGIN));
	zoneBuildingPixmaps[Grid::Zone::Type::Commercial] = commercialPixmaps;
}

void RessourceManager::loadIndustrialPixmaps()
{
	industrialPixmaps.append(QPixmap(":/pixmaps/zones/industrial/factory_1").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/zones/industrial/factory_1_rotate").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/zones/industrial/factory_2").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*3+BUILDING_MARGIN));
	industrialPixmaps.append(QPixmap(":/pixmaps/zones/industrial/factory_2_rotate").scaled(ZONE_SQUARE_SIZE*3+BUILDING_MARGIN, ZONE_SQUARE_SIZE*2+BUILDING_MARGIN));
	zoneBuildingPixmaps[Grid::Zone::Type::Industrial] = industrialPixmaps;
}

void RessourceManager::loadPowerPixmaps()
{
	powerBuildingPixmaps[Buildings::Service::Coal] = QPixmap(":/pixmaps/services/power/coal").scaled(ZONE_SQUARE_SIZE*12+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::Gas] = QPixmap(":/pixmaps/services/power/gas").scaled(ZONE_SQUARE_SIZE*8+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::Oil] = QPixmap(":/pixmaps/services/power/oil").scaled(ZONE_SQUARE_SIZE*24+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::SmallSolar] = QPixmap(":/pixmaps/services/power/small_solar").scaled(ZONE_SQUARE_SIZE*2+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::BigSolar] = QPixmap(":/pixmaps/services/power/big_solar").scaled(ZONE_SQUARE_SIZE*8+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::Wind] = QPixmap(":/pixmaps/services/power/wind").scaled(ZONE_SQUARE_SIZE*4+BUILDING_MARGIN, ZONE_SQUARE_SIZE*4+BUILDING_MARGIN);
	powerBuildingPixmaps[Buildings::Service::Nuclear] = QPixmap(":/pixmaps/services/power/nuclear").scaled(ZONE_SQUARE_SIZE*8+BUILDING_MARGIN, ZONE_SQUARE_SIZE*8+BUILDING_MARGIN);
}
