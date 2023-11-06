#ifndef JV_GAME
#define JV_GAME

#include "bn_core.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_sound_items.h"
#include "bn_sound_actions.h"
#include "bn_blending_actions.h"
#include "bn_camera_actions.h"
#include "common_info.h"
#include "bn_random.h"

#include "bn_regular_bg_items_level1_bg.h"
#include "bn_regular_bg_items_level2_bg.h"
#include "bn_regular_bg_items_level3_bg.h"
#include "bn_regular_bg_items_title_screen.h"
#include "bn_regular_bg_items_intro1.h"
#include "bn_regular_bg_items_attract1_bg.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "jv_constants.h"
#include "jv_game_platform.h"
#include "jv_game_ball.h"
#include "jv_game_brick.h"
#include "jv_game_powerups.h"

namespace
{
int x_offset, y_offset;
unsigned int score = 0, level = 1, lives = 2, brick_frames = 0;
bn::random randomizer;

bn::vector<bn::sprite_ptr, 16> v_score;
bn::vector<bn::sprite_ptr, 8> v_level, v_lives;
bn::string<16> txt_string;
bn::vector<bn::sprite_ptr, 16> v_scene_text;

bn::vector<jv::Ball, 10> basket;
bn::vector<jv::Brick, 35> wall;
bn::vector<jv::PowerUp, 10> powerups;
}

inline void resetcombo(){
    if(bn::keypad::a_held() && bn::keypad::b_held() && bn::keypad::start_held() && bn::keypad::select_held()){
            bn::core::reset();
    }
}
inline void delay(unsigned int frames){
    for(unsigned int i = 0; i < frames; i++){
        bn::core::update();
    }
}

inline void hud(bn::sprite_text_generator& text_generator){
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
inline void pause_screen(bn::sprite_text_generator& text_generator){
    bn::blending::set_fade_alpha(bn::fixed(0.6));
    bn::sound_items::pause.play(1.0);
    v_scene_text.clear();
    text_generator.generate(0, 0, "Pause", v_scene_text);
    bn::core::update();
    while(!bn::keypad::start_pressed()){
        resetcombo();
        bn::core::update();
    }
    bn::blending::set_fade_alpha(bn::fixed(0));
    bn::sound_items::unpause.play(1.0);
    v_scene_text.clear();
}
inline char rng(){
    int random = randomizer.get_int(100);
    if(random >= 100 - ODDS){
        return jv::Power::large;
    }else if(random >= 100 - (ODDS * 2)){
        return jv::Power::multi;
    }else if(random >= 100 - (ODDS * 3)){
        return jv::Power::magnet;
    }else if(random >= 100 - (ODDS * 4)){
        return jv::Power::jackpot;
    }else if(random >= 100 - (ODDS * 5)){
        return jv::Power::powerful;
    }else if(random >= 100 - 1 - (ODDS * 5)){
        return jv::Power::extra;
    }else{
        return jv::Power::powerless;
    }
}

inline void gameover(){
    bn::sprite_text_generator huge_text_generator(fixed_32x64_sprite_font);
    huge_text_generator.set_bg_priority(0);
    huge_text_generator.set_center_alignment();
    int i = -32 - 120;
    bn::vector<bn::sprite_ptr, 4> v_game, v_over;

    while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed()){
        if(i < 0){
            i++;
        }

        v_game.clear();
        v_over.clear();
        
        huge_text_generator.generate(0, i - 16, "GAME", v_game);
        huge_text_generator.generate(0, i + 16, "OVER", v_over);

        bn::core::update();
    }
    bn::core::reset();
}
inline void ball_bounce(jv::Ball& ball, jv::Platform& platform){
    bn::fixed BP_diff, d_x;

    // Screen Boundaries
    if((ball.x() <= - SCREEN_X && ball.d_x() < 0) || (ball.x() >= SCREEN_X && ball.d_x() > 0)){
        ball.set_delta(-ball.d_x(), ball.d_y());
        bn::sound_items::ball_bounce.play(0.5);
    }
    if(ball.y() <= - SCREEN_Y){
        ball.set_delta(ball.d_x(), -ball.d_y());
        bn::sound_items::ball_bounce.play(0.5);
    }

    // Platform bounce
    if(platform.get_rect().intersects(ball.get_rect()) && ball.d_y() > 0){
        BP_diff = ball.x() - platform.x();
        d_x = BP_diff/(4 + platform.get_length()/2);
        if(!platform.is_magnetic()){                        // Is not magnetic
            ball.set_stuck(false);
            if(d_x >= 0){                                   // + d_x +
                ball.set_delta(d_x, - bn::fixed(1.2));
                bn::sound_items::ball_bounce.play(0.5);
            }else{                                          // - d_x -
                ball.set_delta(d_x, - bn::fixed(1.2));
                bn::sound_items::ball_bounce.play(0.5);
            }
        }else{
            if(!ball.is_stuck()){                           // Is magnetic and not stuck
                ball.set_stuck(true);
                ball.set_stuck_x(platform.x() - ball.x());
            }else if(bn::keypad::a_pressed()){              // Is magnetic and stuck
                ball.set_stuck(false);
                ball.set_delta(d_x, -1);
                bn::sound_items::ball_bounce.play(0.5);
            }
        }
    }
    
}
inline void ball_sink(int i, bn::vector<jv::Ball, 10>& basket, jv::Platform& platform){
    int direction = -1 + 2*((lives + level) % 2);
    if(basket[i].y() >= 80 + 8 && basket.size() > 0){      // There are extra lives left
        basket.erase(basket.begin() + i);
        if(lives > 0 && basket.size() == 0){               // Only 1 ball in basket
            lives--;
            platform.shrink();
            platform.set_magnetic(false);
            basket.push_back(jv::Ball(0, 16, bn::fixed(0.2) * direction, bn::fixed(0.5)));
        }
    }
    if(lives == 0 && basket.size() == 0){       // There are no extra lives or balls
        gameover();
    }
}
inline void brick_break(jv::Ball& ball, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 10>& powerups){
    char b_x = 0, b_y = 0;
    for (int i = 0; i < wall.size(); i++) {
        if(ball.get_rect().intersects(wall[i].get_rect())) {
            if(!ball.is_powerful()){
                // Ball is to the Sides
                if((ball.x() >= wall[i].x() + 15 && ball.d_x() < 0) || (ball.x() <= wall[i].x() - 15 && ball.d_x() > 0)){
                    b_x++;
                    ball.set_delta(-ball.d_x(), ball.d_y());
                    bn::sound_items::ball_bounce.play(0.5);
                }
                // Ball is Above or Bellow
                if((ball.y() >= wall[i].y() + 7 && ball.d_y() < 0) || (ball.y() <= wall[i].y() - 7 && ball.d_y() > 0)){
                    b_y++;
                    ball.set_delta(ball.d_x(), -ball.d_y());
                    bn::sound_items::ball_bounce.play(0.5);
                }
            }

            // Brick breaks
            if(wall.size() > 0 && wall[i].is_powered() && powerups.size() < 10){
                powerups.push_back(jv::PowerUp(wall[i].x(), wall[i].y(), wall[i].power()));
            }
            wall.erase(wall.begin() + i);
            score = score + 100;
            i--;

            //break;
        }
    }
}
inline void brick_layer(int rows, int columns, char shape, bn::vector<jv::Brick, 35>& wall){
    int x_offset = -32*(columns-1)/2, y_offset = -58;
    switch(shape % 5){
        case jv::Shape::brickwall:
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, rng()));
                    bn::sound_items::lay_brick.play(1.0);
                    delay(10);
                }
            }
        break;
        case jv::Shape::columns:
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(i < 2 || i >= columns - 2){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, rng()));
                        bn::sound_items::lay_brick.play(1.0);
                        delay(10);
                    }
                }
            }
        break;
        case jv::Shape::rows:
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if(j % 2 == 0){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, rng()));
                        bn::sound_items::lay_brick.play(1.0);
                        delay(10);
                    }
                }
            }
        break;
        case jv::Shape::checkered:
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    if((i+j) % 2 == 0){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, rng()));
                        bn::sound_items::lay_brick.play(1.0);
                        delay(10);
                    }
                }
            }
        break;
        case jv::Shape::diamond:
            for(int i = 0; i < columns; i++){
                for(int j = 0; j < rows; j++){
                    int x = j - rows/2, y = i - columns/2;
                    if(x+y <= columns/2 && x+y >= -columns/2 && x-y >= -columns/2 && x-y <= columns/2){
                        wall.push_back(jv::Brick(x_offset + 32*i, y_offset + 12*j, j, j+1, rng()));
                        bn::sound_items::lay_brick.play(1.0);
                        delay(10);
                    }
                }
            }
        break;

        default:
        break;
    }
}
inline void brick_animation(bn::vector<jv::Brick, 35>& wall){
    if(brick_frames < WAIT_FRAMES + DURATION_FRAMES){
        if(brick_frames >= WAIT_FRAMES){
            for(int i = 0; i < wall.size(); i++){
                int x = (wall[i].x() - x_offset)/32, y = (wall[i].y() - y_offset)/12, window = (columns+rows) - x - (brick_frames - WAIT_FRAMES)*5/(columns+rows);
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
        brick_frames++;
    }else{
        brick_frames = 0;
    }
}

inline void ball_manager(bn::vector<jv::Ball, 10>& basket, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 10>& powerups, jv::Platform& platform){
    for(int i = 0; i < basket.size(); i++){
        ball_bounce(basket[i], platform);
        basket[i].roll(platform);
        brick_break(basket[i], wall, powerups);
        ball_sink(i, basket, platform);
    }
}
inline void platform_manager(jv::Platform& platform){
    if(bn::keypad::left_held() && platform.x() > -120){
        platform.set_x(platform.x() - 2);
    }else if(bn::keypad::right_held() && platform.x() < 120){
        platform.set_x(platform.x() + 2);
    }
    platform.magnet_decay();
}
inline void powerup_manager(bn::vector<jv::Ball, 10>& basket, bn::vector<jv::PowerUp, 10>& powerups, jv::Platform& platform){
    for(int i = 0; i < powerups.size(); i++){
        powerups[i].set_y(powerups[i].y() + bn::fixed(0.5));

        if(platform.get_rect().intersects(powerups[i].get_rect())){
            switch(powerups[i].power()){
                case jv::Power::large:
                    score = score + 100;
                    platform.grow();
                break;
                case jv::Power::multi:
                    score = score + 100;
                    if(basket.size() < 10){
                        basket.push_back(jv::Ball(basket[0].x(), basket[0].y(),  basket[0].d_x(), basket[0].d_y()));
                    }
                break;
                case jv::Power::magnet:
                    score = score + 100;
                    platform.set_magnetic(true);
                break;
                case jv::Power::jackpot:
                    score = score + 3000;
                break;
                case jv::Power::extra:
                    score = score + 100;
                    lives++;
                break;
                case jv::Power::powerful:
                    score = score + 100;
                    for(unsigned char j = 0; j < basket.size(); j++){
                        basket[j].set_powerful();
                    }
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
inline void level_manager(bn::vector<jv::Brick, 35>& wall, bn::regular_bg_ptr& game_bg){
    // Background Picture
    if((level-1)%3 == 0){
        game_bg = bn::regular_bg_items::level1_bg.create_bg(0, 0);
    }else if((level-1)%3 == 1){
        game_bg = bn::regular_bg_items::level2_bg.create_bg(0, 0);
    }else{
        game_bg = bn::regular_bg_items::level3_bg.create_bg(0, 0);
    }

    // Brick Layout
    switch((level-1) % 5){
        case jv::Shape::brickwall:
            brick_layer(5, 6, level-1, wall);
        break;
        case jv::Shape::columns:
            brick_layer(7, 5, level-1, wall);
        break;
        case jv::Shape::rows:
            brick_layer(5, 6, level-1, wall);
        break;
        case jv::Shape::checkered:
            brick_layer(5, 6, level-1, wall);
        break;
        case jv::Shape::diamond:
            brick_layer(5, 5, level, wall);
        break;

        default:
        break;
    }
}    

namespace jv::game
{
extern void intro_scene();
extern void start_scene();
extern void game_scene(jv::Platform& platform, bn::regular_bg_ptr& game_bg);
}

#endif