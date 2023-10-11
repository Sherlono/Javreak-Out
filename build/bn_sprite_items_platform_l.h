#ifndef BN_SPRITE_ITEMS_PLATFORM_L_H
#define BN_SPRITE_ITEMS_PLATFORM_L_H

#include "bn_sprite_item.h"

//{{BLOCK(platform_l_bn_gfx)

//======================================================================
//
//	platform_l_bn_gfx, 64x256@4, 
//	+ palette 16 entries, not compressed
//	+ 256 tiles not compressed
//	Total size: 32 + 8192 = 8224
//
//	Time-stamp: 2023-09-18, 22:28:26
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLATFORM_L_BN_GFX_H
#define GRIT_PLATFORM_L_BN_GFX_H

#define platform_l_bn_gfxTilesLen 8192
extern const bn::tile platform_l_bn_gfxTiles[256];

#define platform_l_bn_gfxPalLen 32
extern const bn::color platform_l_bn_gfxPal[16];

#endif // GRIT_PLATFORM_L_BN_GFX_H

//}}BLOCK(platform_l_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item platform_l(sprite_shape_size(sprite_shape::WIDE, sprite_size::HUGE), 
            sprite_tiles_item(span<const tile>(platform_l_bn_gfxTiles, 256), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(platform_l_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

