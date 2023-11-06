#ifndef JV_GAME_BRICK
#define JV_GAME_BRICK

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_sprite_palette_actions.h"

#include "jv_constants.h"
#include "jv_game_platform.h"
#include "bn_sprite_items_brick.h"

namespace jv
{
class Brick
{
public:
    Brick(int x, int y, int z, int color, char power);
    ~Brick();

    // Getters
    [[nodiscard]] int x(){
        return _brick_position.x();
    }
    [[nodiscard]] int y(){
        return _brick_position.y();
    }

    [[nodiscard]] char power(){
        return _power;
    }

    [[nodiscard]] bool is_powered(){
        if(!_power){
            return false;
        }else{
            return true;
        }
    }

    [[nodiscard]] bn::sprite_ptr get_sprite(){
        return _brick;
    }

    [[nodiscard]] bn::rect get_rect(){
        return _brick_rect;
    }


private:
    bn::sprite_ptr _brick;
    bn::point _brick_position;
    bn::rect _brick_rect;
    char _power;
};

} // End of jv namespace

#endif