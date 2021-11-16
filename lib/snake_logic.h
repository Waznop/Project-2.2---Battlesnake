/** Header file for snake_logic.cpp.
@file snake_logic.h
@author Hong Tai Wei
*/

#include <iostream>
#include <vector>
#include "snake.h"
#include "snake_presets.h"

#ifndef SNAKE_LOGIC_H
#define SNAKE_LOGIC_H

Game new_game(bool human_player, const std::vector<Bot>& bots, int spawn_timer);
void take_turn(Game& game, Move player_move);

#endif  // SNAKE_LOGIC_H