#ifndef BN_SPRITE_ITEMS_BRICK_H
#define BN_SPRITE_ITEMS_BRICK_H

#include "bn_sprite_item.h"

//{{BLOCK(brick_bn_gfx)

//======================================================================
//
//	brick_bn_gfx, 32x96@4, 
//	+ palette 16 entries, not compressed
//	+ 48 tiles not compressed
//	Total size: 32 + 1536 = 1568
//
//	Time-stamp: 2023-09-19, 01:49:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BRICK_BN_GFX_H
#define GRIT_BRICK_BN_GFX_H

#define brick_bn_gfxTilesLen 1536
extern const bn::tile brick_bn_gfxTiles[48];

#define brick_bn_gfxPalLen 32
extern const bn::color brick_bn_gfxPal[16];

#endif // GRIT_BRICK_BN_GFX_H

//}}BLOCK(brick_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item brick(sprite_shape_size(sprite_shape::WIDE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(brick_bn_gfxTiles, 48), bpp_mode::BPP_4, compression_type::NONE, 6), 
            sprite_palette_item(span<const color>(brick_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

