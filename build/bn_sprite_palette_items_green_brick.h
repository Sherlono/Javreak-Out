#ifndef BN_SPRITE_PALETTE_ITEMS_GREEN_BRICK_H
#define BN_SPRITE_PALETTE_ITEMS_GREEN_BRICK_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(green_brick_bn_gfx)

//======================================================================
//
//	green_brick_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2023-09-11, 19:06:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_GREEN_BRICK_BN_GFX_H
#define GRIT_GREEN_BRICK_BN_GFX_H

#define green_brick_bn_gfxPalLen 32
extern const bn::color green_brick_bn_gfxPal[16];

#endif // GRIT_GREEN_BRICK_BN_GFX_H

//}}BLOCK(green_brick_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item green_brick(span<const color>(green_brick_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

