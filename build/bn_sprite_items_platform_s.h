#ifndef BN_SPRITE_ITEMS_PLATFORM_S_H
#define BN_SPRITE_ITEMS_PLATFORM_S_H

#include "bn_sprite_item.h"

//{{BLOCK(platform_s_bn_gfx)

//======================================================================
//
//	platform_s_bn_gfx, 32x128@4, 
//	+ palette 16 entries, not compressed
//	+ 64 tiles not compressed
//	Total size: 32 + 2048 = 2080
//
//	Time-stamp: 2023-09-18, 17:37:42
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_PLATFORM_S_BN_GFX_H
#define GRIT_PLATFORM_S_BN_GFX_H

#define platform_s_bn_gfxTilesLen 2048
extern const bn::tile platform_s_bn_gfxTiles[64];

#define platform_s_bn_gfxPalLen 32
extern const bn::color platform_s_bn_gfxPal[16];

#endif // GRIT_PLATFORM_S_BN_GFX_H

//}}BLOCK(platform_s_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item platform_s(sprite_shape_size(sprite_shape::WIDE, sprite_size::BIG), 
            sprite_tiles_item(span<const tile>(platform_s_bn_gfxTiles, 64), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(platform_s_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

