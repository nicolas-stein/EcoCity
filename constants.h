#ifndef CONSTANTS_H
#define CONSTANTS_H

#define COLOR_MAIN_MENU_BG "#525252"

#define GAME_UPS 60			//Update per seconds

#define TERRAIN_SQUARE_SIZE 512 //px
#define TERRAIN_GRID_SIZE 100
#define TERRAIN_SQUARE_MARGIN 32 //px

#define ROAD_SQUARE_SIZE (TERRAIN_SQUARE_SIZE) //px
#define ROAD_GRID_SIZE (TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE/ROAD_SQUARE_SIZE)
#define ROAD_SQUARE_MARGIN TERRAIN_SQUARE_MARGIN //px

#define ZONE_SQUARE_SIZE (TERRAIN_SQUARE_SIZE/2)
#define ZONE_GRID_SIZE (TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE/ZONE_SQUARE_SIZE)
#define ZONE_SQUARE_MARGIN (TERRAIN_SQUARE_MARGIN/2)

#define ROAD_ZONE_RANGE 4

#define BUILDING_MARGIN TERRAIN_SQUARE_MARGIN

enum Tooltips {NoRoad, NoPower};

namespace Buildings{
	enum Type {BuildingZone, BuildingService};
	namespace Service{
		enum Type {Power};
		enum PowerType {Coal, Gas, Oil, SmallSolar, BigSolar, Wind, Nuclear};
	}
}

namespace Grid{
	enum Type {GridTerrain, GridZone, GridBuilding, GridRoad};
	namespace Terrain{
		enum Type {Grass, Mountain, Rock, Water, Sand};
	}
	namespace Zone {
		enum Type {None, Residential, Commercial, Industrial};
	}
	namespace Road{
		enum Type {TwoLanes, FourLanes, Highway};
		enum Angle {Straight, Turn, TJunction, Cross, DeadEnd};
	}
}


enum SoundEffects {RoadPlacement, ZonePlacement, BuildingPlacement, DestroyLong, DestroyShort};

#endif // CONSTANTS_H
