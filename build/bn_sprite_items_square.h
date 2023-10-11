#ifndef BN_SPRITE_ITEMS_SQUARE_H
#define BN_SPRITE_ITEMS_SQUARE_H

#include "bn_sprite_item.h"

//{{BLOCK(square_bn_gfx)

//======================================================================
//
//	square_bn_gfx, 32x176@4, 
//	+ palette 16 entries, not compressed
//	+ 88 tiles not compressed
//	Total size: 32 + 2816 = 2848
//
//	Time-stamp: 2023-09-18, 12:32:26
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SQUARE_BN_GFX_H
#define GRIT_SQUARE_BN_GFX_H

#define square_bn_gfxTilesLen 2816
extern const bn::tile square_bn_gfxTiles[88];

#define square_bn_gfxPalLen 32
extern const bn::color square_bn_gfxPal[16];

#endif // GRIT_SQUARE_BN_GFX_H

//}}BLOCK(square_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item square(sprite_shape_size(sprite_shape::WIDE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(square_bn_gfxTiles, 88), bpp_mode::BPP_4, compression_type::NONE, 11), 
            sprite_palette_item(span<const color>(square_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

