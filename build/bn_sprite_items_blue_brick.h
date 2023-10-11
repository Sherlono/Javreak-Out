#ifndef BN_SPRITE_ITEMS_BLUE_BRICK_H
#define BN_SPRITE_ITEMS_BLUE_BRICK_H

#include "bn_sprite_item.h"

//{{BLOCK(blue_brick_bn_gfx)

//======================================================================
//
//	blue_brick_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	+ 1 tiles not compressed
//	Total size: 32 + 32 = 64
//
//	Time-stamp: 2023-09-11, 18:25:06
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BLUE_BRICK_BN_GFX_H
#define GRIT_BLUE_BRICK_BN_GFX_H

#define blue_brick_bn_gfxTilesLen 32
extern const bn::tile blue_brick_bn_gfxTiles[1];

#define blue_brick_bn_gfxPalLen 32
extern const bn::color blue_brick_bn_gfxPal[16];

#endif // GRIT_BLUE_BRICK_BN_GFX_H

//}}BLOCK(blue_brick_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item blue_brick(sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL), 
            sprite_tiles_item(span<const tile>(blue_brick_bn_gfxTiles, 1), bpp_mode::BPP_4, compression_type::NONE, 1), 
            sprite_palette_item(span<const color>(blue_brick_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

