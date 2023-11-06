#ifndef BN_REGULAR_BG_ITEMS_ATTRACT1_BG_H
#define BN_REGULAR_BG_ITEMS_ATTRACT1_BG_H

#include "bn_regular_bg_item.h"

//{{BLOCK(attract1_bg_bn_gfx)

//======================================================================
//
//	attract1_bg_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 3 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 96 + 2048 = 2176
//
//	Time-stamp: 2023-11-01, 17:26:10
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ATTRACT1_BG_BN_GFX_H
#define GRIT_ATTRACT1_BG_BN_GFX_H

#define attract1_bg_bn_gfxTilesLen 96
extern const bn::tile attract1_bg_bn_gfxTiles[3];

#define attract1_bg_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell attract1_bg_bn_gfxMap[1024];

#define attract1_bg_bn_gfxPalLen 32
extern const bn::color attract1_bg_bn_gfxPal[16];

#endif // GRIT_ATTRACT1_BG_BN_GFX_H

//}}BLOCK(attract1_bg_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item attract1_bg(
            regular_bg_tiles_item(span<const tile>(attract1_bg_bn_gfxTiles, 3), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(attract1_bg_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(attract1_bg_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

