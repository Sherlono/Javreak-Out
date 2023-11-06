#ifndef BN_REGULAR_BG_ITEMS_ATTRACT1_H
#define BN_REGULAR_BG_ITEMS_ATTRACT1_H

#include "bn_regular_bg_item.h"

//{{BLOCK(attract1_bn_gfx)

//======================================================================
//
//	attract1_bn_gfx, 256x256@8, 
//	+ palette 32 entries, not compressed
//	+ 24 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 64 + 1536 + 2048 = 3648
//
//	Time-stamp: 2023-10-30, 13:15:54
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_ATTRACT1_BN_GFX_H
#define GRIT_ATTRACT1_BN_GFX_H

#define attract1_bn_gfxTilesLen 1536
extern const bn::tile attract1_bn_gfxTiles[48];

#define attract1_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell attract1_bn_gfxMap[1024];

#define attract1_bn_gfxPalLen 64
extern const bn::color attract1_bn_gfxPal[32];

#endif // GRIT_ATTRACT1_BN_GFX_H

//}}BLOCK(attract1_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item attract1(
            regular_bg_tiles_item(span<const tile>(attract1_bn_gfxTiles, 48), bpp_mode::BPP_8, compression_type::NONE), 
            bg_palette_item(span<const color>(attract1_bn_gfxPal, 32), bpp_mode::BPP_8, compression_type::NONE),
            regular_bg_map_item(attract1_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

