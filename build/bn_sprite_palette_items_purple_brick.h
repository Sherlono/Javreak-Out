#ifndef BN_SPRITE_PALETTE_ITEMS_PURPLE_BRICK_H
#define BN_SPRITE_PALETTE_ITEMS_PURPLE_BRICK_H

#include "bn_sprite_palette_item.h"

//{{BLOCK(purple_brick_bn_gfx)

//======================================================================
//
//	purple_brick_bn_gfx, 8x8@4, 
//	+ palette 16 entries, not compressed
//	Total size: 32 = 32
//
//	Time-stamp: 2023-09-15, 00:44:39
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PURPLE_BRICK_BN_GFX_H
#define GRIT_PURPLE_BRICK_BN_GFX_H

#define purple_brick_bn_gfxPalLen 32
extern const bn::color purple_brick_bn_gfxPal[16];

#endif // GRIT_PURPLE_BRICK_BN_GFX_H

//}}BLOCK(purple_brick_bn_gfx)

namespace bn::sprite_palette_items
{
    constexpr inline sprite_palette_item purple_brick(span<const color>(purple_brick_bn_gfxPal, 16), 
            bpp_mode::BPP_4, compression_type::NONE);
}

#endif

