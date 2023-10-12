#ifndef JV_GAME_POWERUPS
#define JV_GAME_POWERUPS

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_sprite_palette_actions.h"

#include "jv_constants.h"
#include "jv_game_platform.h"
#include "bn_sprite_items_power_up.h"

namespace jv
{
class PowerUp
{
public:
PowerUp(bn::fixed x, bn::fixed y, int power);
PowerUp(int x, int y, int power);

// Setters
void set_y(bn::fixed y);

// Getters
[[nodiscard]] bn::fixed x(){
    return _pu_position.x();
}
[[nodiscard]] bn::fixed y(){
    return _pu_position.y();
}

[[nodiscard]] int power(){
    return _power;
}

[[nodiscard]] bn::rect get_rect(){
    return _pu_rect;
}

private:
    bn::sprite_ptr _pu;
    bn::fixed_point _pu_position;
    bn::rect _pu_rect;
    int _power;
};
} // End of jv namespace

#endif