#include "bn_core.h"
#include "bn_string.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h" 
#include "bn_blending_actions.h"
//#include "bn_blending_fade_alpha.h"
#include "bn_camera_actions.h"
#include "bn_sprite_text_generator.h"

//#include "common_info.h"
//#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "jv_constants.h"
#include "jv_game_manager.h"

#include "bn_regular_bg_items_level1_bg.h"
#include "bn_regular_bg_items_level2_bg.h"
#include "bn_regular_bg_items_level3_bg.h"
#include "bn_regular_bg_items_title_screen.h"
#include "bn_regular_bg_items_intro1.h"


namespace jv
{

const int mag_duration = 60 * 7;
const bn::sprite_palette_item   &red_brick_palette = bn::sprite_palette_items::red_brick, 
                                &blue_brick_palette = bn::sprite_palette_items::blue_brick, 
                                &green_brick_palette = bn::sprite_palette_items::green_brick, 
                                &yellow_brick_palette = bn::sprite_palette_items::yellow_brick, 
                                &purple_brick_palette = bn::sprite_palette_items::purple_brick;
enum Color {null, red, blue, green, yellow, purple};
enum Power {powerless, extra, large, multi, jackpot, magnet};

class Platform
{
public:
    Platform(bn::fixed x, bn::fixed y) :
        _platform(bn::sprite_items::platform_s.create_sprite(x, y)),
        _plat_position(bn::fixed_point(x, y)),
        _length(32),
        _is_magnetic(0),
        _plat_rect(x.round_integer(), y.round_integer() + 4, _length, 8),
        _is_long(false)
    {
        _platform.set_z_order(0);
    }

    // Getters
    bn::fixed x(){
        return _plat_position.x();
    }
    bn::fixed y(){
        return _plat_position.y();
    }

    int get_length(){
        return _length;
    }

    bool is_magnetic(){
        return _is_magnetic;
    }

    bn::rect get_rect(){
        return _plat_rect;
    }

    //Setters
    void set_position(bn::fixed_point position){
        _plat_position = position;
        _platform.set_position(position);
        _plat_rect.set_position(position.x().round_integer(), position.y().round_integer());
    }
    void set_position(bn::fixed x, bn::fixed y){
        _plat_position = bn::fixed_point(x, y);
        _platform.set_position(x, y);
        _plat_rect.set_position(x.round_integer(), y.round_integer() + 4);
    }

    void set_x(bn::fixed x){
        _plat_position = bn::fixed_point(x, _plat_position.y());
        _platform.set_x(x);
        _plat_rect.set_x(x.round_integer());
    }
    void set_y(bn::fixed y){
        _plat_position = bn::fixed_point(_plat_position.x(), y);
        _platform.set_y(y);
        _plat_rect.set_y(y.round_integer() + 4);
    }

    void set_visible(bool can_see){
        _platform.set_visible(can_see);
    }

    void reset_position(){
        _plat_position = bn::fixed_point(0, 65);
        _platform.set_position(_plat_position);
        _plat_rect.set_position(0, 65 + 4);
    }

    void grow(){
        if(!_is_long){
            _length = 48;
            _plat_rect.set_width(_length);
            _platform = bn::sprite_items::platform_l.create_sprite(_plat_position);
            _platform.set_tiles(bn::sprite_items::platform_l.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration)); 
        }
        _is_long = true;
    }

    void shrink(){
        if(_is_long){
            _length = 32;
            _plat_rect.set_width(_length);
            _platform = bn::sprite_items::platform_s.create_sprite(_plat_position);
            _platform.set_tiles(bn::sprite_items::platform_s.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration));
        }
        _is_long = false;
    }

    void set_magnetic(bool magnetic){
        _is_magnetic = magnetic * jv::mag_duration;
    }

    void magnet_decay(){
        if(_is_magnetic){
            if(!_is_long){
                _platform.set_tiles(bn::sprite_items::platform_s.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration));
            }else{
                _platform.set_tiles(bn::sprite_items::platform_l.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration));
            }
            _is_magnetic--;
        }else if(!_is_long){
            _platform.set_tiles(bn::sprite_items::platform_s.tiles_item().create_tiles(0));
        }else{
            _platform.set_tiles(bn::sprite_items::platform_l.tiles_item().create_tiles(0));
        }
    }

private:
    bn::sprite_ptr _platform;
    bn::fixed_point _plat_position;
    int _length, _is_magnetic;
    bn::rect _plat_rect;
    bool _is_long;
};

class Ball
{
public:
    Ball(bn::fixed x = 0, bn::fixed y = 16) :
        _ball(bn::sprite_items::ball.create_sprite(x, y)),
        _ball_position(bn::fixed_point(x, y)),
        _delta_x(0),
        _delta_y(0),
        _stuck_x(0),
        _ball_rect(x.round_integer(), y.round_integer(), 8, 8),
        _stuck(false)
    {
        _ball.set_z_order(1);
    }
    Ball(bn::fixed x, bn::fixed y, bn::fixed d_x, bn::fixed d_y) :
        _ball(bn::sprite_items::ball.create_sprite(x, y)),
        _ball_position(bn::fixed_point(x, y)),
        _delta_x(d_x),
        _delta_y(d_y),
        _stuck_x(0),
        _ball_rect(x.round_integer(), y.round_integer(), 8, 8),
        _stuck(false)
    {
        _ball.set_z_order(1);
    }
    ~Ball() = default;
    
    // Getters
    bn::fixed x(){
        return _ball_position.x();
    }
    bn::fixed y(){
        return _ball_position.y();
    }

    bn::fixed d_x(){
        return _delta_x;
    }
    bn::fixed d_y(){
        return _delta_y;
    }

    bn::fixed stuck_x(){
        return _stuck_x;
    }    
    bool is_stuck(){
        return _stuck;
    }

    bn::rect get_rect(){
        return _ball_rect;
    }

    // Setters
    void set_position(bn::fixed_point position){
        _ball_position = position;
        _ball.set_position(position);
        _ball_rect.set_position(position.x().round_integer(), position.y().round_integer());
    }
    void set_position(bn::fixed x, bn::fixed y){
        _ball_position = bn::fixed_point(x, y);
        _ball.set_position(x, y);
        _ball_rect.set_position(x.round_integer(), y.round_integer());
    }

    void set_x(bn::fixed x){
        _ball_position = bn::fixed_point(x, _ball_position.y());
        _ball.set_x(x);
        _ball_rect.set_x(x.round_integer());
    }
    void set_y(bn::fixed y){
        _ball_position = bn::fixed_point(_ball_position.x(), y);
        _ball.set_y(y);
        _ball_rect.set_y(y.round_integer());
    }

    void set_stuck_x(bn::fixed stuck_x){
        _stuck_x = stuck_x;
    }

    void set_delta(bn::fixed d_x, bn::fixed d_y){
        _delta_x = d_x;
        _delta_y = d_y;
    }

    void set_stuck(bool stuck){
        _stuck = stuck;
    }

    void toggle_stuck(){
        _stuck = !_stuck;
    }

    void erase(){
        this->~Ball();
    }

    // **********************************************
    // **************** roll be here ****************
    void roll(jv::Platform& platform){
        if(!_stuck){
            this->set_x(_ball_position.x() + _delta_x * bn::fixed(1.5));
            this->set_y(_ball_position.y() + _delta_y * bn::fixed(1.5));
        }else{
            this->set_position(platform.x() - _stuck_x, _ball_position.y());
        }
    }
    // **************** roll be here ****************
    // **********************************************

    void set_visible(bool can_see){
        _ball.set_visible(can_see);
    }

    void reset_position(){
        this->set_position(bn::fixed_point(0, 16));
    }

private:
    bn::sprite_ptr _ball;
    bn::fixed_point _ball_position;
    bn::fixed _delta_x, _delta_y, _stuck_x;
    bn::rect _ball_rect;
    bool _stuck;
};

class Brick
{
public:
    Brick(int x, int y, int z = 0, int color = 0, int power = jv::Power::powerless):
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
    ~Brick() = default;
    
    // Getters
    int x(){
        return _brick_position.x();
    }
    int y(){
        return _brick_position.y();
    }
    
    int power(){
        return _power;
    }

    bool is_powered(){
        if(!_power){
            return false;
        }else{
            return true;
        }
    }

    bn::sprite_ptr get_sprite(){
        return _brick;
    }

    bn::rect get_rect(){
        return _brick_rect;
    }

private:
    bn::sprite_ptr _brick;
    bn::point _brick_position;
    bn::rect _brick_rect;
    int _power;
};

class PowerUp
{
public:
    PowerUp(bn::fixed x, bn::fixed y, int power):
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
    PowerUp(int x, int y, int power):
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

    // Getters
    bn::fixed x(){
        return _pu_position.x();
    }
    bn::fixed y(){
        return _pu_position.y();
    }

    int power(){
        return _power;
    }
    
    bn::rect get_rect(){
        return _pu_rect;
    }

    // Setters
    void set_y(bn::fixed y){
        _pu_position = bn::fixed_point(_pu_position.x(), y);
        _pu.set_y(y);
        _pu_rect.set_y(y.round_integer());
    }

private:
    bn::sprite_ptr _pu;
    bn::fixed_point _pu_position;
    bn::rect _pu_rect;
    int _power;
};

} // End of jv Namespace

namespace
{

void reset_combo(){
    if(bn::keypad::a_held() && bn::keypad::b_held() && bn::keypad::start_held() && bn::keypad::select_held()){
            bn::core::reset();
    }
}

void intro_scene(){
    bn::regular_bg_ptr intro1_bg = bn::regular_bg_items::intro1.create_bg(0, 0);
    intro1_bg.set_priority(0);
    intro1_bg.set_blending_enabled(true);
    bn::blending::set_fade_alpha(0);
    
    for(int i = 0; i < 390; i=i+1){
        if(i < 60){
            bn::blending::set_fade_alpha(bn::min(1-bn::fixed(i)/60, bn::fixed(1)));
        }else if(i >= 240 && i <= 300){
            bn::blending::set_fade_alpha(bn::max((bn::fixed(i)-240)/60, bn::fixed(0)));
        }
        bn::core::update();
        reset_combo();
    }
    bn::blending::set_fade_alpha(0);
    intro1_bg.set_blending_enabled(false);
}

void start_scene(){
    bn::regular_bg_ptr title_screen = bn::regular_bg_items::title_screen.create_bg(0, 0);
    title_screen.set_priority(0);
    title_screen.set_blending_enabled(true);
    bn::blending::set_fade_alpha(0);
    int j = 0;

    for(int i = 0; i < 60; i = i+1){
        j++;
        bn::blending::set_fade_alpha(bn::min(1-bn::fixed(i)/60, bn::fixed(1)));
        bn::core::update();
        if(bn::keypad::any_pressed()){
            break;
        }
        reset_combo();
    }
    while(!bn::keypad::any_pressed()){
        bn::core::update();
    }

    for(int i = j; i > 0; i = i-1){
        bn::blending::set_fade_alpha(bn::min(bn::fixed(1)-bn::fixed(i)/60, bn::fixed(1)));
        bn::core::update();
        reset_combo();
    }

    bn::blending::set_fade_alpha(0);
}

void game_over(bn::sprite_text_generator& text_generator){
    static int i = -32 - 120;
    static bn::vector<bn::sprite_ptr, 4> v_game, v_over;

    while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed()){
        if(i < 0){
            i++;
        }

        v_game.clear();
        v_over.clear();
        
        text_generator.generate(0, i - 16, "GAME", v_game);
        text_generator.generate(0, i + 16, "OVER", v_over);

        bn::core::update();
    }
    bn::core::reset();
}

void hud(bn::sprite_text_generator& text_generator){
    static bn::vector<bn::sprite_ptr, 16> v_score;
    static bn::vector<bn::sprite_ptr, 8> v_level, v_lives;
    static bn::string<32> txt_string;

    v_score.clear();
    v_level.clear();
    v_lives.clear();

    txt_string = "Score: " + bn::to_string<16>(score);
    text_generator.generate(-7 * 16, -68, txt_string, v_score);
    
    text_generator.set_center_alignment();
    txt_string = "Level " + bn::to_string<10>(level);
    text_generator.generate(0, -68, txt_string, v_level);

    text_generator.set_left_alignment();
    txt_string = "Lives: " + bn::to_string<10>(lives);
    text_generator.generate(4 * 16, -68, txt_string, v_lives);
}

/*void pause_screen(bn::sprite_text_generator& text_generator){
    bn::regular_bg_ptr intro1_bg = bn::regular_bg_items::intro1.create_bg(0, 0);
    intro1_bg.set_priority(0);
    intro1_bg.set_blending_enabled(true);
    bn::blending::set_fade_alpha(bn::fixed(0.5));

    static bn::vector<bn::sprite_ptr, 8> v_pause;

    while(!bn::keypad::start_pressed()){
        text_generator.generate(0, -80, "Paused", v_pause);
        v_pause.clear();
    }
    bn::blending::set_fade_alpha(bn::fixed(0));
    intro1_bg.set_blending_enabled(false);
}*/
    
void game_scene(){
    // Background stuff
    static bn::regular_bg_ptr game_bg = bn::regular_bg_items::level1_bg.create_bg(0, 0);
    game_bg.set_priority(3);

    // Text stuff
    static bn::sprite_text_generator    small_text_generator(common::variable_8x8_sprite_font),
                                        big_text_generator(common::variable_8x16_sprite_font),
                                        huge_text_generator(fixed_32x64_sprite_font);
    small_text_generator.set_bg_priority(0);
    big_text_generator.set_bg_priority(0);
    big_text_generator.set_center_alignment();
    huge_text_generator.set_bg_priority(0);
    huge_text_generator.set_center_alignment();
    static bn::vector<bn::sprite_ptr, 16> v_scene_text;

    // Game items
    static bn::vector<jv::Ball, 6> basket;
    static bn::vector<jv::Brick, 35> wall;
    static bn::vector<jv::PowerUp, 6> powerups;

    static jv::Platform platform(0, 65);
    basket.push_back(jv::Ball(0, 16, -bn::fixed(0.2), bn::fixed(0.5)));

    // Level creation
    level_manager(wall);
    if(((level/4)%3) == 0){
        game_bg = bn::regular_bg_items::level1_bg.create_bg(0, 0);
    }else if(((level/4)%3) == 1){
        game_bg = bn::regular_bg_items::level2_bg.create_bg(0, 0);
    }else{
        game_bg = bn::regular_bg_items::level3_bg.create_bg(0, 0);
    }

    // Level start
    while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed()){
        big_text_generator.generate(0, 0, "Ready?", v_scene_text);
        hud(small_text_generator);
        //pause_screen(big_text_generator);
        platform_manager(platform);
        brick_animation(wall);
        bn::core::update();
        v_scene_text.clear();

        reset_combo();
    }
    
    // Game in progress
    while(!wall.empty()){
        hud(small_text_generator);

        platform_manager(platform);
        ball_manager(basket, wall, powerups, platform, huge_text_generator);
        powerup_manager(basket, powerups, platform);

        brick_animation(wall);

        bn::core::update();

        reset_combo();
    }

    level++;
    platform.reset_position();
    platform.set_magnetic(false);
    basket.clear();
    powerups.clear();
}
} // End of main Namespace

int main()
{
    bn::core::init();
    
    bool gamestart = false;

    while(true)
    {
        if(!gamestart){
            intro_scene();
            start_scene();
            gamestart = true;
        }

        game_scene();

        bn::core::update();
    }
}
