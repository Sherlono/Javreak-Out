#ifndef BN_REGULAR_BG_ITEMS_INTRO1_H
#define BN_REGULAR_BG_ITEMS_INTRO1_H

#include "bn_regular_bg_item.h"

//{{BLOCK(intro1_bn_gfx)

//======================================================================
//
//	intro1_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 84 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 2688 + 2048 = 4768
//
//	Time-stamp: 2023-09-12, 02:25:19
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_INTRO1_BN_GFX_H
#define GRIT_INTRO1_BN_GFX_H

#define intro1_bn_gfxTilesLen 2688
extern const bn::tile intro1_bn_gfxTiles[84];

#define intro1_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell intro1_bn_gfxMap[1024];

#define intro1_bn_gfxPalLen 32
extern const bn::color intro1_bn_gfxPal[16];

#endif // GRIT_INTRO1_BN_GFX_H

//}}BLOCK(intro1_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item intro1(
            regular_bg_tiles_item(span<const tile>(intro1_bn_gfxTiles, 84), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(intro1_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(intro1_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

