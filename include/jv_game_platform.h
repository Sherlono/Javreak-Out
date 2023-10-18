#ifndef JV_GAME_PLATFORM
#define JV_GAME_PLATFORM

#include "bn_sprite_ptr.h"
#include "bn_sprite_tiles_ptr.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"
#include "bn_keypad.h"
#include "bn_sprite_animate_actions.h"

#include "bn_sprite_items_platform_s.h"
#include "bn_sprite_items_platform_l.h"

namespace jv
{

class Platform
{
public:
    Platform(bn::fixed x, bn::fixed y);

    // Getters
    [[nodiscard]] bn::fixed x(){
        return _plat_position.x();
    }
    [[nodiscard]] bn::fixed y(){
        return _plat_position.y();
    }

    [[nodiscard]] int get_length(){
        return _length;
    }

    [[nodiscard]] bool is_magnetic(){
        return _is_magnetic;
    }

    [[nodiscard]] bn::rect get_rect(){
        return _plat_rect;
    }

    //Setters
    void set_position(const bn::fixed_point& position);
    void set_position(bn::fixed x, bn::fixed y);

    void set_x(bn::fixed x);
    void set_y(bn::fixed y);

    void set_visible(bool can_see);

    void reset_position();

    void grow();

    void shrink();

    void set_magnetic(bool magnetic);

    void magnet_decay();

private:
    bn::sprite_ptr _platform;
    bn::fixed_point _plat_position;
    int _length, _is_magnetic;
    bn::rect _plat_rect;
    bool _is_long;
};

} // End of jv namespace

#endif