#ifndef BN_REGULAR_BG_ITEMS_LEVEL1_BG_H
#define BN_REGULAR_BG_ITEMS_LEVEL1_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(level1_bg_bn_gfx)

//======================================================================
//
//	level1_bg_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 149 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 4768 + 2048 = 6848
//
//	Time-stamp: 2023-09-16, 15:17:59
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL1_BG_BN_GFX_H
#define GRIT_LEVEL1_BG_BN_GFX_H

#define level1_bg_bn_gfxTilesLen 4768
extern const bn::tile level1_bg_bn_gfxTiles[149];

#define level1_bg_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell level1_bg_bn_gfxMap[1024];

#define level1_bg_bn_gfxPalLen 32
extern const bn::color level1_bg_bn_gfxPal[16];

#endif // GRIT_LEVEL1_BG_BN_GFX_H

//}}BLOCK(level1_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item level1_bg(
            regular_bg_tiles_item(span<const tile>(level1_bg_bn_gfxTiles, 149), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(level1_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(level1_bg_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

