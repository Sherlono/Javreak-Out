#include "bn_core.h"
#include "bn_sprite_text_generator.h"

#include "jv_game.h"

int main()
{
    bn::core::init();

    bool gamestart = false;

    while(true)
    {
        if(!gamestart){
            jv::game::intro_scene();
            jv::game::start_scene();
            gamestart = true;
        }

        jv::game::game_scene();

        bn::core::update();
    }
}
