#include "jv_game_manager.h"
namespace
{
/*
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

*/
}
