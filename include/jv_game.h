#ifndef JV_GAME
#define JV_GAME

#include "bn_core.h"
#include "bn_sprite_text_generator.h"
#include "bn_string.h"
#include "bn_display.h"
#include "bn_keypad.h"
#include "bn_regular_bg_ptr.h"
#include "bn_blending_actions.h"
#include "bn_camera_actions.h"
#include "common_info.h"

#include "bn_regular_bg_items_level1_bg.h"
#include "bn_regular_bg_items_level2_bg.h"
#include "bn_regular_bg_items_level3_bg.h"
#include "bn_regular_bg_items_title_screen.h"
#include "bn_regular_bg_items_intro1.h"

#include "common_variable_8x8_sprite_font.h"
#include "common_variable_8x16_sprite_font.h"
#include "fixed_32x64_sprite_font.h"

#include "jv_constants.h"
#include "jv_game_platform.h"
#include "jv_game_ball.h"
#include "jv_game_brick.h"
#include "jv_game_powerups.h"

namespace jv::game
{
void reset_combo();
void intro_scene();
void start_scene();
void hud(bn::sprite_text_generator& text_generator);

void game_over(bn::sprite_text_generator& text_generator);
void ball_bounce(jv::Ball& ball, jv::Platform& platform);
void ball_sink(int i, bn::vector<jv::Ball, 6>& basket, jv::Platform& platform, bn::sprite_text_generator& text_generator);
void brick_break(jv::Ball& ball, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 6>& powerups);
void brick_layer(int rows, int columns, char shape, bn::vector<jv::Brick, 35>& wall);
void brick_animation(bn::vector<jv::Brick, 35>& wall);
void ball_manager(bn::vector<jv::Ball, 6>& basket, bn::vector<jv::Brick, 35>& wall, bn::vector<jv::PowerUp, 6>& powerups, jv::Platform& platform, bn::sprite_text_generator& text_generator);

void platform_manager(jv::Platform& platform);

void powerup_manager(bn::vector<jv::Ball, 6>& basket, bn::vector<jv::PowerUp, 6>& powerups, jv::Platform& platform);

void level_manager(bn::vector<jv::Brick, 35>& wall);


void game_scene();
}

#endif