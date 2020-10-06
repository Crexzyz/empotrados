
//{{BLOCK(win)

//======================================================================
//
//	win, 256x256@8, 
//	+ palette 128 entries, not compressed
//	+ 78 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 256 + 4992 + 2048 = 7296
//
//	Time-stamp: 2020-10-06, 22:20:03
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.3
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_WIN_H
#define GRIT_WIN_H

#define winTilesLen 4992
extern const unsigned char winTiles[4992];

#define winMapLen 2048
extern const unsigned char winMap[2048];

#define winPalLen 256
extern const unsigned char winPal[256];

#endif // GRIT_WIN_H

//}}BLOCK(win)
