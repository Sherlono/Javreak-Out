#ifndef BN_REGULAR_BG_ITEMS_TITLE_SCREEN_H
#define BN_REGULAR_BG_ITEMS_TITLE_SCREEN_H

#include "bn_regular_bg_item.h"

//{{BLOCK(title_screen_bn_gfx)

//======================================================================
//
//	title_screen_bn_gfx, 256x256@4, 
//	+ palette 16 entries, not compressed
//	+ 258 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 32 + 8256 + 2048 = 10336
//
//	Time-stamp: 2023-09-16, 16:48:17
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_TITLE_SCREEN_BN_GFX_H
#define GRIT_TITLE_SCREEN_BN_GFX_H

#define title_screen_bn_gfxTilesLen 8256
extern const bn::tile title_screen_bn_gfxTiles[258];

#define title_screen_bn_gfxMapLen 2048
extern const bn::regular_bg_map_cell title_screen_bn_gfxMap[1024];

#define title_screen_bn_gfxPalLen 32
extern const bn::color title_screen_bn_gfxPal[16];

#endif // GRIT_TITLE_SCREEN_BN_GFX_H

//}}BLOCK(title_screen_bn_gfx)

namespace bn::regular_bg_items
{
    constexpr inline regular_bg_item title_screen(
            regular_bg_tiles_item(span<const tile>(title_screen_bn_gfxTiles, 258), bpp_mode::BPP_4, compression_type::NONE), 
            bg_palette_item(span<const color>(title_screen_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE),
            regular_bg_map_item(title_screen_bn_gfxMap[0], size(32, 32), compression_type::NONE));
}

#endif

