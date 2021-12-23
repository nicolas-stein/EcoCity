#ifndef CONSTANTS_H
#define CONSTANTS_H

#define COLOR_MAIN_MENU_BG "#525252"

#define GAME_UPS 60			//Update per seconds

#define TERRAIN_SQUARE_SIZE 512 //px
#define TERRAIN_GRID_SIZE 100
#define TERRAIN_SQUARE_MARGIN 32 //px

#define ROAD_SQUARE_SIZE 1024 //px
#define ROAD_GRID_SIZE TERRAIN_GRID_SIZE*TERRAIN_SQUARE_SIZE/ROAD_SQUARE_SIZE
#define ROAD_SQUARE_MARGIN TERRAIN_SQUARE_MARGIN //px

#define ZONE_SQUARE_SIZE TERRAIN_SQUARE_SIZE
#define ZONE_GRID_SIZE TERRAIN_GRID_SIZE
#define ZONE_SQUARE_MARGIN TERRAIN_SQUARE_MARGIN

#define ROAD_ZONE_RANGE 4
#define BUILDING_MARGIN TERRAIN_SQUARE_MARGIN

enum GridType {Terrain, Zone, ServiceBuilding, Road};
enum TerrainType {Grass, Mountain, Rock, Water, Sand};
enum ZoneType {None, Residential, Commercial, Industrial};

enum RoadType {TwoLanes, FourLanes, Highway};
enum RoadAngle {Straight, Turn, TJunction, Cross, DeadEnd};

#endif // CONSTANTS_H
