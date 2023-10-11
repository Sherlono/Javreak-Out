#ifndef BN_SPRITE_PALETTE_ITEMS_YELLOW_BRICK_H
#define BN_SPRITE_PALETTE_ITEMS_YELLOW_BRICK_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(yellow_brick_bn_gfx)

//======================================================================
//
//	yellow_brick_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2023-09-11, 19:06:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_YELLOW_BRICK_BN_GFX_H
#define GRIT_YELLOW_BRICK_BN_GFX_H

#define yellow_brick_bn_gfxPalLen 32
extern const bn::color yellow_brick_bn_gfxPal[16];

#endif // GRIT_YELLOW_BRICK_BN_GFX_H

//}}BLOCK(yellow_brick_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item yellow_brick(span<const color>(yellow_brick_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

