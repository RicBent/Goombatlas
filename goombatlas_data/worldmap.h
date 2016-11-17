#include <nds.h>

typedef struct path
{
	u8 endingNodeId;
	u8 animationId;
	u8 settings;
	u8 unk;
} path;

typedef struct node
{
	path *paths;	// As many paths as you want. Last path should be FF 00 00 00
	u8 areaId;
	u8 unk0;		// Allways 0?
	u8 unk1;		// Mostly 0. I only saw 0xB in W1. Is this for the blue mushroom house?
	u8 icon;
	u8 settings;
	u8 unk2[3];		// Allways 0? Probably padding
} node;

typedef struct pathBehavior
{
	u8 animationId;
	u8 starcoinCost;
	u8 visibility;
	u8 unk;			// Allways 0? Probably padding
} pathBehavior;

typedef struct sprites
{
	u8 sprite1StartNode;
	u8 sprite1Type;
	u8 sprite2StartNode;
	u8 sprite2Type;
} sprites;

typedef struct visibleNode
{
	u8 unlockedPaths[4];
	u8 unlockedpathsSecret[4];
	u16 cameraLocation;
	u16 cameraLocationSecret;
	s32 x;
	s32 y;
	s32 z;
} visibleNode;

typedef struct mapObject
{
	u8 animationId;
	u8 pad[3];
	s32 x;
	s32 y;
	s32 z;
} mapObject;

typedef struct bowserJrNode
{
	u8 animationId;
	u8 behavior;
} bowserJrNode;

typedef struct world
{
	node *nodes;
	pathBehavior *pathBehaviors;
	sprites *mapObjs;
	visibleNode *visibleNode;
	mapObject *starcoinSigns;		// As many as you want. Last one should be zeroed
	mapObject *strongholds;		// ^
	mapObject *mushroomHouses;		// ^
	bowserJrNode **bowserJrPaths;
	u16 nodeCount;
	u16 pathCount;
	u32 unk1;							// allways 0
} world;