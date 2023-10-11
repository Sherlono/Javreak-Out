#include "bn_core.h"
#include "bn_string.h"
#include "bn_fixed.h"
#include "bn_keypad.h"
#include "bn_display.h"
#include "bn_regular_bg_ptr.h" 
#include "bn_blending_actions.h"
#include "bn_blending_fade_alpha.h"
#include "bn_camera_actions.h"
#include "bn_sprite_text_generator.h"

#include "common_info.h"
#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "bn_regular_bg_items_level1_bg.h"
#include "bn_regular_bg_items_level2_bg.h"
#include "bn_regular_bg_items_level3_bg.h"
#include "bn_regular_bg_items_title_screen.h"
#include "bn_regular_bg_items_intro1.h"

#include "jv_constants.h"
#include "jv_game_manager.h"

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
    }
}