#ifndef BN_SPRITE_ITEMS_SPRITES_H
#define BN_SPRITE_ITEMS_SPRITES_H

#include "bn_sprite_item.h"

//{{BLOCK(sprites_bn_gfx)

//======================================================================
//
//	sprites_bn_gfx, 16x128@4, 
//	+ palette 16 entries, not compressed
//	+ 32 tiles not compressed
//	Total size: 32 + 1024 = 1056
//
//	Time-stamp: 2023-09-10, 23:41:51
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_SPRITES_BN_GFX_H
#define GRIT_SPRITES_BN_GFX_H

#define sprites_bn_gfxTilesLen 1024
extern const bn::tile sprites_bn_gfxTiles[32];

#define sprites_bn_gfxPalLen 32
extern const bn::color sprites_bn_gfxPal[16];

#endif // GRIT_SPRITES_BN_GFX_H

//}}BLOCK(sprites_bn_gfx)

namespace bn::sprite_items
{
    constexpr inline sprite_item sprites(sprite_shape_size(sprite_shape::SQUARE, sprite_size::NORMAL), 
            sprite_tiles_item(span<const tile>(sprites_bn_gfxTiles, 32), bpp_mode::BPP_4, compression_type::NONE, 8), 
            sprite_palette_item(span<const color>(sprites_bn_gfxPal, 16), bpp_mode::BPP_4, compression_type::NONE));
}

#endif

