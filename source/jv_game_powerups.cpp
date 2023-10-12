#include "jv_game_powerups.h"

namespace jv
{
PowerUp::PowerUp(bn::fixed x, bn::fixed y, int power):
    _pu(bn::sprite_items::power_up.create_sprite(x, y)),
    _pu_position(bn::fixed_point(x, y)),
    _pu_rect(x.round_integer(), y.round_integer(), 20, 8),
    _power(power)
{
    _pu.set_z_order(1);
    _pu.set_bg_priority(1);
    switch(power % 5){
        case 0:     // Extra
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(green_brick_palette);
        break;
        case 1:     // Large
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(blue_brick_palette);
        break;
        case 2:     // Multi
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(red_brick_palette);
        break;
        case 3:     // Jackpot
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(yellow_brick_palette);
        break;
        case 4:     // Magnetic
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(purple_brick_palette);
        break;

        default:
        break;
    }
}

PowerUp::PowerUp(int x, int y, int power):
    _pu(bn::sprite_items::power_up.create_sprite(x, y)),
    _pu_position(bn::fixed_point(x, y)),
    _pu_rect(x, y, 20, 8),
    _power(power)
{
    _pu.set_z_order(1);
    _pu.set_bg_priority(1);
    switch(power % 5){
        case 0:     // Extra
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(green_brick_palette);
        break;
        case 1:     // Large
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(blue_brick_palette);
        break;
        case 2:     // Multi
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(red_brick_palette);
        break;
        case 3:     // Jackpot
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(yellow_brick_palette);
        break;
        case 4:     // Magnetic
            _pu.set_tiles(bn::sprite_items::power_up.tiles_item().create_tiles(power));
            _pu.set_palette(purple_brick_palette);
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