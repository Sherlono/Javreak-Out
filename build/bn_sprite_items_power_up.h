#ifndef BN_SPRITE_ITEMS_POWER_UP_H
#define BN_SPRITE_ITEMS_POWER_UP_H

#include "bn_sprite_item.h"

//{{BLOCK(power_up_bn_gfx)

//======================================================================
//
//	power_up_bn_gfx, 16x96@4, 
//	+ palette 16 entries, not compressed
//	+ 24 tiles not compressed
//	Total size: 32 + 768 = 800
//
//	Time-stamp: 2023-09-19, 14:33:49
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_POWER_UP_BN_GFX_H
#define GRIT_POWER_UP_BN_GFX_H

#define power_up_bn_gfxTilesLen 768
extern const bn::tile power_up_bn_gfxTiles[24];

#define power_up_bn_gfxPalLen 32
extern const bn::color power_up_bn_gfxPal[16];

#endif // GRIT_POWER_UP_BN_GFX_H

//}}BLOCK(power_up_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item power_up(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(power_up_bn_gfxTiles, 24), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(power_up_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

