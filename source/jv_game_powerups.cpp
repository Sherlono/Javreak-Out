#include "jv_game_powerups.h"

namespace jv
{
PowerUp::PowerUp(int x, int y, char power, char z):
    _pu(bn::sprite_items::power_up.create_sprite(x, y)),
    _pu_position(bn::fixed_point(x, y)),
    _pu_rect(x, y, 20, 8),
    _power(power)
{
    _pu.set_z_order(z);
    _pu.set_bg_priority(0);
    _pu.set_blending_enabled(true);
    switch(power){
        case jv::Power::large:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(red_brick_palette);
        break;
        case jv::Power::multi:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(yellow_brick_palette);
        break;
        case jv::Power::magnet:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(green_brick_palette);
        break;
        case jv::Power::jackpot:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(purple_brick_palette);
        break;
        case jv::Power::extra:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(blue_brick_palette);
        break;
        case jv::Power::powerful:
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(orange_brick_palette);
        break;

        default:
        break;
    }
}

// Setters
void PowerUp::set_y(bn::fixed y){
    _pu_position = bn::fixed_point(_pu_position.x(), y);
    _pu.set_y(y);
    _pu_rect.set_y(y.round_integer());
}

} // End of jv namespace