#ifndef JV_CONSTANTS
#define JV_CONSTANTS

#include "bn_sprite_palette_actions.h"
#include "bn_sprite_palette_items_red_brick.h"
#include "bn_sprite_palette_items_blue_brick.h"
#include "bn_sprite_palette_items_yellow_brick.h"
#include "bn_sprite_palette_items_green_brick.h"
#include "bn_sprite_palette_items_purple_brick.h"

namespace
{
constexpr const int SCREEN_X = 120 - 8, SCREEN_Y = 80 - 8;
}

namespace jv
{
static const bn::sprite_palette_item    &red_brick_palette = bn::sprite_palette_items::red_brick, 
                                        &blue_brick_palette = bn::sprite_palette_items::blue_brick, 
                                        &green_brick_palette = bn::sprite_palette_items::green_brick, 
                                        &yellow_brick_palette = bn::sprite_palette_items::yellow_brick, 
                                        &purple_brick_palette = bn::sprite_palette_items::purple_brick;

enum Color {null, red, blue, green, yellow, purple};
enum Power {powerless, extra, large, multi, jackpot, magnet};
}

#endif