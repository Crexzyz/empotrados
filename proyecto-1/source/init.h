
//{{BLOCK(init)

//======================================================================
//
//	init, 256x256@8, 
//	+ palette 128 entries, not compressed
//	+ 32 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 256 + 2048 + 2048 = 4352
//
//	Time-stamp: 2020-10-06, 00:15:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_INIT_H
#define GRIT_INIT_H

#define initTilesLen 2048
extern const unsigned char initTiles[2048];

#define initMapLen 2048
extern const unsigned char initMap[2048];

#define initPalLen 256
extern const unsigned char initPal[256];

#endif // GRIT_INIT_H

//}}BLOCK(init)
