#ifndef BN_SPRITE_ITEMS_BALL_H
#define BN_SPRITE_ITEMS_BALL_H

#include "bn_sprite_item.h"

//{{BLOCK(ball_bn_gfx)

//======================================================================
//
//	ball_bn_gfx, 8x16@4, 
//	+ palette 16 entries, not compressed
//	+ 2 tiles not compressed
//	Total size: 32 + 64 = 96
//
//	Time-stamp: 2023-11-01, 22:48:39
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BALL_BN_GFX_H
#define GRIT_BALL_BN_GFX_H

#define ball_bn_gfxTilesLen 64
extern const bn::tile ball_bn_gfxTiles[2];

#define ball_bn_gfxPalLen 32
extern const bn::color ball_bn_gfxPal[16];

#endif // GRIT_BALL_BN_GFX_H

//}}BLOCK(ball_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item ball(sprite_shape_size(sprite_shape::SQUARE, sprite_size::SMALL), 
            sprite_tiles_item(span<const tile>(ball_bn_gfxTiles, 2), bpp_mode::BPP_4, compression_type::NONE, 2), 
            sprite_palette_item(span<const color>(ball_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

