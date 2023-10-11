#include "jv_game_brick.h"

namespace jv
{
Brick::Brick(int x, int y, int z = 0, int color = 0, int power = jv::Power::powerless) :
    _brick(bn::sprite_items::brick.create_sprite(x, y)),
    _brick_position(bn::point(x, y)),
    _brick_rect(x, y, 32, 12),
    _power(power)
{
    _brick.set_z_order(z);
    _brick.set_bg_priority(2);
    switch(color % 6){
        case 1:
            _brick.set_palette(red_brick_palette);
        break;
        case 2:
            _brick.set_palette(blue_brick_palette);
        break;
        case 3:
            _brick.set_palette(green_brick_palette);
        break;
        case 4:
            _brick.set_palette(yellow_brick_palette);
        break;
        case 5:
            _brick.set_palette(purple_brick_palette);
        break;

        default:
        break;
    }
}
Brick::~Brick() = default;
    
} // End of jv namespace