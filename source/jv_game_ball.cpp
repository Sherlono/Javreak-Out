#include "jv_game_ball.h"

namespace jv
{Ball::Ball(bn::fixed x, bn::fixed y, bn::fixed d_x, bn::fixed d_y) :
    _ball(bn::sprite_items::ball.create_sprite(x, y)),
    _ball_position(bn::fixed_point(x, y)),
    _delta_x(d_x),
    _delta_y(d_y),
    _stuck_x(0),
    _ball_rect(x.round_integer(), y.round_integer(), 8, 8),
    _powerful(0),
    _stuck(false)
{
    _ball.set_z_order(1);
    _ball.set_blending_enabled(true);
}
Ball::~Ball() = default;

// Setters
void Ball::set_position(bn::fixed_point position){
    _ball_position = position;
    _ball.set_position(position);
    _ball_rect.set_position(position.x().round_integer(), position.y().round_integer());
}
void Ball::set_position(bn::fixed x, bn::fixed y){
    _ball_position = bn::fixed_point(x, y);
    _ball.set_position(x, y);
    _ball_rect.set_position(x.round_integer(), y.round_integer());
}

void Ball::set_x(bn::fixed x){
    _ball_position = bn::fixed_point(x, _ball_position.y());
    _ball.set_x(x);
    _ball_rect.set_x(x.round_integer());
}
void Ball::set_y(bn::fixed y){
    _ball_position = bn::fixed_point(_ball_position.x(), y);
    _ball.set_y(y);
    _ball_rect.set_y(y.round_integer());
}

void Ball::set_delta(bn::fixed d_x, bn::fixed d_y){
    _delta_x = d_x;
    _delta_y = d_y;
}

void Ball::set_stuck_x(bn::fixed stuck_x){
    _stuck_x = stuck_x;
}
void Ball::set_stuck(bool stuck){
    _stuck = stuck;
}
void Ball::toggle_stuck(){
    _stuck = !_stuck;
}

void Ball::set_powerful(){
    _ball.set_tiles(bn::sprite_items::ball.tiles_item().create_tiles(1));
    _powerful = 60 * 3;
}

void Ball::erase(){
    this->~Ball();
}

// **********************************************
// **************** roll be here ****************
void Ball::roll(jv::Platform& platform){
    if(!_stuck){
        this->set_x(_ball_position.x() + _delta_x * bn::fixed(1.5));
        this->set_y(_ball_position.y() + _delta_y * bn::fixed(1.5));
    }else{
        this->set_position(platform.x() - _stuck_x, _ball_position.y());
    }
    if(_powerful){
        _powerful--;
    }else{
        _ball.set_tiles(bn::sprite_items::ball.tiles_item().create_tiles(0));
    }
}
// **************** roll be here ****************
// **********************************************

void Ball::set_visible(bool can_see){
    _ball.set_visible(can_see);
}

void Ball::reset_position(){
    this->set_position(bn::fixed_point(0, 16));
}

} // End of jv namespace