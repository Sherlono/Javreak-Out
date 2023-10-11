#include "jv_game_platform.h"

namespace jv
{
const int mag_duration = 60 * 7;

Platform::Platform(bn::fixed x, bn::fixed y) :
    _platform(bn::sprite_items::platform_s.create_sprite(x, y)),
    _plat_position(x, y),
    _length(32),
    _is_magnetic(0),
    _plat_rect(x.round_integer(), y.round_integer() + 4, _length, 8),
    _is_long(false)
{
    _platform.set_z_order(0);
}

void Platform::set_position(const bn::fixed_point& position){
    _plat_position = position;
    _platform.set_position(position);
    _plat_rect.set_position(position.x().round_integer(), position.y().round_integer());
}
void Platform::set_position(bn::fixed x, bn::fixed y){
    _plat_position = bn::fixed_point(x, y);
    _platform.set_position(x, y);
    _plat_rect.set_position(x.round_integer(), y.round_integer() + 4);
}

void Platform::set_x(bn::fixed x){
    _plat_position = bn::fixed_point(x, _plat_position.y());
    _platform.set_x(x);
    _plat_rect.set_x(x.round_integer());
}
void Platform::set_y(bn::fixed y){
    _plat_position = bn::fixed_point(_plat_position.x(), y);
    _platform.set_y(y);
    _plat_rect.set_y(y.round_integer() + 4);
}

void Platform::set_visible(bool can_see){
    _platform.set_visible(can_see);
}

void Platform::reset_position(){
    _plat_position = bn::fixed_point(0, 65);
    _platform.set_position(_plat_position);
    _plat_rect.set_position(0, 65 + 4);
}

void Platform::grow(){
    if(!_is_long){
        _length = 48;
        _plat_rect.set_width(_length);
        _platform = bn::sprite_items::platform_l.create_sprite(_plat_position);
        _platform.set_tiles(bn::sprite_items::platform_l.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration)); 
    }
    _is_long = true;
}

void Platform::shrink(){
    if(_is_long){
        _length = 32;
        _plat_rect.set_width(_length);
        _platform = bn::sprite_items::platform_s.create_sprite(_plat_position);
        _platform.set_tiles(bn::sprite_items::platform_s.tiles_item().create_tiles((_is_magnetic + 59)*7/jv::mag_duration));
    }
    _is_long = false;
}

void Platform::set_magnetic(bool magnetic){
    _is_magnetic = magnetic * jv::mag_duration;
}

void Platform::magnet_decay(){
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

} // End of jv namespace