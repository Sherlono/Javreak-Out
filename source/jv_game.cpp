#include "jv_game.h"

namespace jv::game
{
void intro_scene(){
    bn::regular_bg_ptr intro1_bg = bn::regular_bg_items::intro1.create_bg(0, 0);
    intro1_bg.set_priority(0);
    intro1_bg.set_blending_enabled(true);
    bn::blending::set_fade_alpha(0);
    
    for(int i = 0; i < 390; i++){
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

void attract_scene(bn::regular_bg_ptr& title_screen, bn::vector<jv::PowerUp, 10>& powerups){
    title_screen = bn::regular_bg_items::attract1_bg.create_bg(0, 0);
    title_screen.set_blending_enabled(true);
    title_screen.set_priority(3);
    int j = 0, idle = 0;

    bn::vector<bn::sprite_ptr, 64> v_text;

    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    text_generator.set_bg_priority(0);
    text_generator.generate(-80, -55, "Makes your platform longer.", v_text);
    text_generator.generate(-80, -33, "Puts an extra ball bouncing on screen.", v_text);
    text_generator.generate(-80, -11, "Makes you magnetic. Balls stick.", v_text);
    text_generator.generate(-80, 11, "The ball plows through bricks.", v_text);
    text_generator.generate(-80, 33, "Extra life. Get it!", v_text);
    text_generator.generate(-80, 55, "777 Jackpot. More points!", v_text);

    for(bn::sprite_ptr letter : v_text){
        letter.set_blending_enabled(true);
    }
    
    powerups.push_back(jv::PowerUp(-98, -55, jv::Power::large));
    powerups.push_back(jv::PowerUp(-98, -33, jv::Power::multi));
    powerups.push_back(jv::PowerUp(-98, -11, jv::Power::magnet));
    powerups.push_back(jv::PowerUp(-98, 11, jv::Power::powerful));
    powerups.push_back(jv::PowerUp(-98, 33, jv::Power::extra));
    powerups.push_back(jv::PowerUp(-98, 55, jv::Power::jackpot));

    // Fade in
    for(int i = 0; i < 60; i++){
        j++;
        bn::blending::set_fade_alpha(bn::min(1-bn::fixed(i)/60, bn::fixed(1)));
        if(bn::keypad::any_pressed()){
            break;
        }
        // Scroll
        title_screen.set_position(title_screen.x() + bn::fixed(0.5), title_screen.y() + bn::fixed(0.5));
        if(title_screen.x() == 32*2){
            title_screen.set_position(0,0);
        }
        resetcombo();
        bn::core::update();
    }
    // Body
    while(!bn::keypad::any_pressed()){
        idle++;
        if(idle >= 60*12){
            break;
        }
        // Scroll
        title_screen.set_position(title_screen.x() + bn::fixed(0.5), title_screen.y() + bn::fixed(0.5));
        if(title_screen.x() == 32*2){
            title_screen.set_position(0,0);
        }
        bn::core::update();
    }
    // Fade out
    for(int i = j; i > 0; i--){
        bn::blending::set_fade_alpha(bn::min(bn::fixed(1)-bn::fixed(i)/60, bn::fixed(1)));
        // Scroll
        title_screen.set_position(title_screen.x() + bn::fixed(0.5), title_screen.y() + bn::fixed(0.5));
        if(title_screen.x() == 32*2){
            title_screen.set_position(0,0);
        }
        resetcombo();
        bn::core::update();
    }

    powerups.clear();
    v_text.clear();
    bn::blending::set_fade_alpha(0);
    title_screen = bn::regular_bg_items::title_screen.create_bg(0, 0);
    title_screen.set_blending_enabled(true);
}

void start_scene(){
    start:
    bn::regular_bg_ptr title_screen = bn::regular_bg_items::title_screen.create_bg(0, 0);
    title_screen.set_priority(3);
    title_screen.set_blending_enabled(true);
    bn::blending::set_fade_alpha(0);
    int j = 0, idle = 0;

    // Fade in
    for(int i = 0; i < 60; i++){
        j++;
        bn::blending::set_fade_alpha(bn::min(1-bn::fixed(i)/60, bn::fixed(1)));
        if(bn::keypad::any_pressed()){
            break;
        }
        randomizer.update();
        resetcombo();
        bn::core::update();
    }
    // Body
    while(!bn::keypad::any_pressed()){
        idle++;
        if(idle == 60*12){
            for(int i = 60; i > 0; i--){
                bn::blending::set_fade_alpha(bn::min(bn::fixed(1)-bn::fixed(i)/60, bn::fixed(1)));
                randomizer.update();
                resetcombo();
                bn::core::update();
            }
            attract_scene(title_screen, powerups);
            goto start;
        }
        bn::core::update();
    }
    // Fade out
    for(int i = j; i > 0; i--){
        bn::blending::set_fade_alpha(bn::min(bn::fixed(1)-bn::fixed(i)/60, bn::fixed(1)));
        randomizer.update();
        resetcombo();
        bn::core::update();
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
    basket.push_back(jv::Ball(0, 16, bn::fixed(0.2), bn::fixed(0.5)));
    basket[0].set_visible(false);

    // Level creation
    hud(small_text_generator);
    level_manager(wall, game_bg);
    basket[0].set_visible(true);

    // Level start
    big_text_generator.generate(0, 0, "Ready?", v_scene_text);
    while(!bn::keypad::a_pressed() && !bn::keypad::b_pressed()){
        platform_manager(platform);
        brick_animation(wall);

        if(bn::keypad::start_pressed()){
            pause_screen(big_text_generator);
        }

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
        
        if(bn::keypad::start_pressed()){
            pause_screen(big_text_generator);
        }
        resetcombo();
        bn::core::update();
    }

    // Reset all for next level
    level++;
    platform.reset_position();
    platform.set_magnetic(false);
    basket.clear();
    powerups.clear();
}

}