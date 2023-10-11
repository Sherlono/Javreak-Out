#include "bn_core.h"
#include "bn_string.h"
#include "bn_rect.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sprite_palette_actions.h" 
#include "bn_blending_actions.h"
#include "bn_blending_fade_alpha.h"
#include "bn_camera_actions.h"
#include "bn_sprite_text_generator.h"
#include "bn_sprite_animate_actions.h"

#include "common_info.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "bn_sprite_items_platform_s.h"
#include "bn_sprite_items_platform_l.h"

#include "bn_sprite_items_ball.h"
#include "bn_sprite_items_power_up.h"
#include "bn_sprite_items_brick.h"

#include "bn_sprite_palette_items_red_brick.h"
#include "bn_sprite_palette_items_blue_brick.h"
#include "bn_sprite_palette_items_yellow_brick.h"
#include "bn_sprite_palette_items_green_brick.h"
#include "bn_sprite_palette_items_purple_brick.h"

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
// Game state data
int score = 0, level = 1, lives = 2, x_offset, y_offset;

// Constants
const int SCREEN_X = 120 - 8, SCREEN_Y = 80 - 8;

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

void ball_bounce(jv::Ball& ball, jv::Platform& platform){
    static bn::fixed BallPlat_diff, d_x;
    static int Plat_length;

    // Screen Boundaries
    if((ball.x() <= - SCREEN_X && ball.d_x() < 0) || (ball.x() >= SCREEN_X && ball.d_x() > 0)){
        ball.set_delta(-ball.d_x(), ball.d_y());
    }
    if(ball.y() <= - SCREEN_Y){
        ball.set_delta(ball.d_x(), -ball.d_y());
    }

    // Platform bounce
    if(platform.get_rect().intersects(ball.get_rect()) && ball.d_y() > 0){
        BallPlat_diff = ball.x() - platform.x();
        Plat_length = platform.get_length();
        d_x = BallPlat_diff/(4 + Plat_length/2);
        if(!platform.is_magnetic()){                        // Is not magnetic
            ball.set_stuck(false);
            if(d_x >= 0){                                   // + d_x +
                ball.set_delta(d_x, - bn::fixed(1.2));
            }else{                                          // - d_x -
                ball.set_delta(d_x, - bn::fixed(1.2));
            }
        }else{
            if(!ball.is_stuck()){                           // Is magnetic and not stuck
                ball.set_stuck(true);
                ball.set_stuck_x(platform.x() - ball.x());
            }else if(bn::keypad::a_pressed()){              // Is magnetic and stuck
                ball.set_stuck(false);
                ball.set_delta(d_x, -1);
            }
        }
    }
    
}

void ball_sink(int i, bn::vector<jv::Ball, 6>& basket, jv::Platform& platform, bn::sprite_text_generator& text_generator){
    int direction = -1 + 2*((lives+level) % 2);
    if(basket[i].y() >= 80 + 8){
        if(basket.size() > 0){                                              // There are extra lives left
            basket.erase(basket.begin() + i);
            if(lives > 0 && basket.size() == 0){                            //      Only 1 ball in basket
                lives--;
                platform.shrink();
                platform.set_magnetic(false);
                basket.push_back(jv::Ball(0, 16, bn::fixed(0.2) * direction, bn::fixed(0.5)));
            }
        }
    }
    if(lives == 0 && basket.size() == 0){
        game_over(text_generator);
    }
}

void brick_break(jv::Ball& ball, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 6>& powerups){
    for (int i = 0; i < wall.size(); i++) {
        if (ball.get_rect().intersects(wall[i].get_rect())) {
            // Ball is to the Sides
            if((ball.x() >= wall[i].x() + 16 && ball.d_x() < 0) || (ball.x() <= wall[i].x() - 16 && ball.d_x() > 0)){
                ball.set_delta(-ball.d_x(), ball.d_y());
            }
            // Ball is Above or Bellow
            if((ball.y() <= wall[i].y() - 6 && ball.d_y() > 0) || (ball.y() >= wall[i].y() + 6 && ball.d_y() < 0)){
                ball.set_delta(ball.d_x(), -ball.d_y());
            }
            score = score + 100;
            if(wall[i].is_powered()){
                powerups.push_back(jv::PowerUp(wall[i].x(), wall[i].y(), wall[i].power()));
            }
            wall.erase(wall.begin() + i);

            break;
        }
    }
}

void brick_layer(int rows, int columns, char shape, bn::vector<jv::Brick, 35>& wall){
    int brick_power = 0;
    x_offset = -32*(columns-1)/2;
    y_offset = -58;

    switch(shape % 4){
        case 0:     // Simple wall
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(j == 1 && i == 1){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 2 && i == 1){
                        brick_power = jv::Power::large;
                    }else if(j == 1 && i == columns - 2){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 2 && i == columns - 2){
                        brick_power = jv::Power::magnet;
                    }else if((j == rows/2 && i == columns - 2) ||  (j == rows/2 && i == 1)){
                        brick_power = jv::Power::multi;
                    }else{
                        brick_power = 0;
                    }
                    wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, brick_power));
                }
            }
        break;
        case 1:     // Two columns
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(j == 1 && i == 1){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 2 && i == 1){
                        brick_power = jv::Power::large;
                    }else if(j == 1 && i == columns - 2){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 2 && i == columns - 2){
                        brick_power = jv::Power::magnet;
                    }else if((j == rows/2 && i == columns - 2) ||  (j == rows/2 && i == 1)){
                        brick_power = jv::Power::multi;
                    }else{
                        brick_power = 0;
                    }
                    if(i < 2 || i >= columns - 2){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, brick_power));
                    }
                }
            }
        break;
        case 2:     // Horizontal lines
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(j == 0 && i == 1){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 1 && i == 1){
                        brick_power = jv::Power::large;
                    }else if(j == 1 && i == columns - 2){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 1 && i == columns - 2){
                        brick_power = jv::Power::magnet;
                    }else if((j == rows/2 && i == columns - 2) ||  (j == rows/2 && i == 1)){
                        brick_power = jv::Power::multi;
                    }else{
                        brick_power = 0;
                    }
                    if(j % 2 == 0){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, brick_power));
                    }
                }
            }
        break;
        case 3:     // Checkered
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(j == 1 && i == 1){
                        brick_power = jv::Power::extra;
                    }else if(j == rows - 2 && i == 1){
                        brick_power = jv::Power::large;
                    }else if(j == 1 && i == columns - 2){
                        brick_power = jv::Power::jackpot;
                    }else if(j == rows - 2 && i == columns - 2){
                        brick_power = jv::Power::magnet;
                    }else if((j == rows/2 && i == columns - 2) ||  (j == rows/2 && i == 1)){
                        brick_power = jv::Power::multi;
                    }else{
                        brick_power = 0;
                    }
                    if((i+j) % 2 == 0){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, brick_power));
                    }
                }
            }
        break;

        default:
        break;
    }
}

void brick_animation(bn::vector<jv::Brick, 35>& wall){
    static int a = 0, wait_frames = 60*4, duration_frames = 60*1, rows = 7, columns = 6;

    if(a < wait_frames + duration_frames){
        if(a >= wait_frames){
            for(int i = 0; i < wall.size(); i++){
                int x = (wall[i].x() - x_offset)/32, y = (wall[i].y() - y_offset)/12, window = (columns+rows) - x - (a - wait_frames)*5/(columns+rows);
                if(y == window){
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(1));
                }else if(y == window - 1){
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(2));
                }else if(y == window - 2){
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(3));
                }else if(y == window - 3){
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(4));
                }else if(y == window - 4){
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(5));
                }else{
                    wall[i].get_sprite().set_tiles(bn::sprite_items::brick.tiles_item().create_tiles(0));
                }
            }
        }
        a++;
    }else{
        a = 0;
    }
}

void ball_manager(bn::vector<jv::Ball, 6>& basket, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 6>& powerups, jv::Platform& platform, bn::sprite_text_generator& text_generator){
    for(int i = 0; i < basket.size(); i++){
        ball_bounce(basket[i], platform);
        basket[i].roll(platform);
        brick_break(basket[i], wall, powerups);
        ball_sink(i, basket, platform, text_generator);
    }
}

void platform_manager(jv::Platform& platform){
    if(bn::keypad::left_held() && platform.x() > -120){
        platform.set_x(platform.x() - 2);
    }else if(bn::keypad::right_held() && platform.x() < 120){
        platform.set_x(platform.x() + 2);
    }
    platform.magnet_decay();
}

void powerup_manager(bn::vector<jv::Ball, 6>& basket, bn::vector<jv::PowerUp, 6>& powerups, jv::Platform& platform){
    for(int i = 0; i < powerups.size(); i++){
        powerups[i].set_y(powerups[i].y() + bn::fixed(0.5));

        if(platform.get_rect().intersects(powerups[i].get_rect())){
            switch(powerups[i].power()){
                case jv::Power::extra:
                    score = score + 100;
                    lives++;
                break;
                case jv::Power::large:
                    score = score + 100;
                    platform.grow();
                break;
                case jv::Power::multi:
                    score = score + 100;
                    if(basket.size() < 6){
                        basket.push_back(jv::Ball(basket[0].x(), basket[0].y(),  basket[0].d_x(), basket[0].d_y()));
                    }
                break;
                case jv::Power::jackpot:
                    score = score + 2000;
                break;
                case jv::Power::magnet:
                    score = score + 100;
                    platform.set_magnetic(true);
                break;

                default:
                break;
            }
            powerups.erase(powerups.begin() + i);
        }else if(powerups[i].y() > 90){
            powerups.erase(powerups.begin() + i);
        }
    }
}

void level_manager(bn::vector<jv::Brick, 35>& wall){
    switch((level-1) % 4){
        case 0:
            brick_layer(5, 6, level-1, wall);
        break;
        case 1:
            brick_layer(7, 5, level-1, wall);
        break;
        case 2:
            brick_layer(5, 6, level-1, wall);
        break;
        case 3:
            brick_layer(5, 6, level-1, wall);
        break;

        default:
        break;
    }
}

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
