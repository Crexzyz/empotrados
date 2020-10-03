
//{{BLOCK(bombmap)

//======================================================================
//
//	bombmap, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 19 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 1216 + 2048 = 3776
//
//	Time-stamp: 2020-09-28, 14:54:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BOMBMAP_H
#define GRIT_BOMBMAP_H

#define bombmapTilesLen 1216
extern const unsigned int bombmapTiles[304];

#define bombmapMapLen 2048
extern const unsigned int bombmapMap[512];

#define bombmapPalLen 512
extern const unsigned int bombmapPal[128];

#endif // GRIT_BOMBMAP_H

//}}BLOCK(bombmap)
