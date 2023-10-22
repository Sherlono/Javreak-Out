#include "bn_core.h"
#include "bn_sprite_text_generator.h"

#include "jv_game.h"

int main()
{
    bn::core::init();

    jv::Platform platform(0, 65);
    platform.set_visible(false);

    bn::regular_bg_ptr game_bg = bn::regular_bg_items::level1_bg.create_bg(0, 0);
    game_bg.set_priority(3);
    game_bg.set_visible(false);
    
    bool gamestart = false;

    while(true)
    {
        if(!gamestart){
            jv::game::intro_scene();
            jv::game::start_scene();
            gamestart = true;
        }

        jv::game::game_scene(platform, game_bg);

        bn::core::update();
    }
}
