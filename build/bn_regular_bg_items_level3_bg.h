#ifndef BN_REGULAR_BG_ITEMS_LEVEL3_BG_H
#define BN_REGULAR_BG_ITEMS_LEVEL3_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(level3_bg_bn_gfx)

//======================================================================
//
//	level3_bg_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 196 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 6272 + 2048 = 8352
//
//	Time-stamp: 2023-09-25, 22:15:24
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_LEVEL3_BG_BN_GFX_H
#define GRIT_LEVEL3_BG_BN_GFX_H

#define level3_bg_bn_gfxTilesLen 6272
extern const bn::tile level3_bg_bn_gfxTiles[196];

#define level3_bg_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell level3_bg_bn_gfxMap[1024];

#define level3_bg_bn_gfxPalLen 32
extern const bn::color level3_bg_bn_gfxPal[16];

#endif // GRIT_LEVEL3_BG_BN_GFX_H

//}}BLOCK(level3_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item level3_bg(
            regular_bg_tiles_item(span<const tile>(level3_bg_bn_gfxTiles, 196), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(level3_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(level3_bg_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

