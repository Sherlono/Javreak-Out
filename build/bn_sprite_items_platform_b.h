#ifndef BN_SPRITE_ITEMS_PLATFORM_B_H
#define BN_SPRITE_ITEMS_PLATFORM_B_H

#include "bn_sprite_item.h"

//{{BLOCK(platform_b_bn_gfx)

//======================================================================
//
//	platform_b_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2023-09-18, 17:46:38
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLATFORM_B_BN_GFX_H
#define GRIT_PLATFORM_B_BN_GFX_H

#define platform_b_bn_gfxTilesLen 8192
extern const bn::tile platform_b_bn_gfxTiles[256];

#define platform_b_bn_gfxPalLen 32
extern const bn::color platform_b_bn_gfxPal[16];

#endif // GRIT_PLATFORM_B_BN_GFX_H

//}}BLOCK(platform_b_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item platform_b(sprite_shape_size(sprite_shape::WIDE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(platform_b_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(platform_b_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

