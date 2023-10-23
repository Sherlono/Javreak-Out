#include "jv_game.h"

namespace jv::game
{
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
        resetcombo();
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
        resetcombo();
    }
    while(!bn::keypad::any_pressed()){
        bn::core::update();
    }

    for(int i = j; i > 0; i = i-1){
        bn::blending::set_fade_alpha(bn::min(bn::fixed(1)-bn::fixed(i)/60, bn::fixed(1)));
        bn::core::update();
        resetcombo();
    }

    bn::blending::set_fade_alpha(0);
}
void game_scene(jv::Platform& platform, bn::regular_bg_ptr& game_bg){
    // Text stuff
    bn::sprite_text_generator   small_text_generator(common::variable_8x8_sprite_font),
                                big_text_generator(common::variable_8x16_sprite_font);
    small_text_generator.set_bg_priority(0);
    big_text_generator.set_bg_priority(0);
    big_text_generator.set_center_alignment();

    // Game items
    game_bg.set_visible(true);
    platform.set_visible(true);
    basket.push_back(jv::Ball(0, 16, -bn::fixed(0.2), bn::fixed(0.5)));
    basket[0].set_visible(false);

    // Level creation
    hud(small_text_generator);
    level_manager(wall, game_bg);
    basket[0].set_visible(true);

    // Level start
    while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed()){
        v_scene_text.clear();
        big_text_generator.generate(0, 0, "Ready?", v_scene_text);
        hud(small_text_generator);
        //pause_screen(big_text_generator);
        platform_manager(platform);
        brick_animation(wall);

        resetcombo();
        bn::core::update();
    }
    v_scene_text.clear();
    
    // Game in progress
    while(!wall.empty()){
        hud(small_text_generator);

        platform_manager(platform);
        ball_manager(basket, wall, powerups, platform);
        powerup_manager(basket, powerups, platform);

        brick_animation(wall);

        resetcombo();
        bn::core::update();
    }

    // Reset all for next level
    reset_items(platform, basket, powerups);
}

}