
//{{BLOCK(space)

//======================================================================
//
//	space, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 35 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 2240 + 2048 = 4800
//
//	Time-stamp: 2020-11-16, 22:07:53
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPACE_H
#define GRIT_SPACE_H

#define spaceTilesLen 2240
extern const unsigned int spaceTiles[560];

#define spaceMapLen 2048
extern const unsigned int spaceMap[512];

#define spacePalLen 512
extern const unsigned int spacePal[128];

#endif // GRIT_SPACE_H

//}}BLOCK(space)
