#ifndef JV_GAME_BALL
#define JV_GAME_BALL

#include "bn_sprite_ptr.h"
#include "bn_fixed_point.h"
#include "bn_rect.h"

#include "jv_constants.h"
#include "jv_game_platform.h"
#include "bn_sprite_items_ball.h"

namespace jv
{
class Ball
{
public:
    Ball(bn::fixed x, bn::fixed y, bn::fixed d_x, bn::fixed d_y);
    ~Ball();
    
    // Getters
    [[nodiscard]] bn::fixed x(){
        return _ball_position.x();
    }
    [[nodiscard]] bn::fixed y(){
        return _ball_position.y();
    }

    [[nodiscard]] bn::fixed d_x(){
        return _delta_x;
    }
    [[nodiscard]] bn::fixed d_y(){
        return _delta_y;
    }

    [[nodiscard]] bn::fixed stuck_x(){
        return _stuck_x;
    }    
    [[nodiscard]] bool is_stuck(){
        return _stuck;
    }
    
    [[nodiscard]] bool is_powerful(){
        return _powerful;
    }

    [[nodiscard]] bn::rect get_rect(){
        return _ball_rect;
    }

    // Setters
    void set_position(bn::fixed_point position);
    void set_position(bn::fixed x, bn::fixed y);

    void set_x(bn::fixed x);
    void set_y(bn::fixed y);

    void set_delta(bn::fixed d_x, bn::fixed d_y);

    void set_stuck_x(bn::fixed stuck_x);
    void set_stuck(bool stuck);
    void toggle_stuck();

    void set_powerful();

    void erase();

    void roll(jv::Platform& platform);

    void set_visible(bool can_see);

    void reset_position();

private:
    bn::sprite_ptr _ball;
    bn::fixed_point _ball_position;
    bn::fixed _delta_x, _delta_y, _stuck_x;
    bn::rect _ball_rect;
    unsigned int _powerful;
    bool _stuck;
};

}

#endif