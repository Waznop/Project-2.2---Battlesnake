/** Contains pre-written Battlesnakes.
@file snake_presets.h
@author Hong Tai Wei
*/

#include <iostream>
#include <vector>
#include "../battlesnake.h"
#include "snake.h"

#ifndef SNAKE_PRESETS_H
#define SNAKE_PRESETS_H

enum Bot {
  CUSTOM,
  HUNGRY,
  CIRCLE,
  GREEDY,
  RANDOM,
  HEALTH,
};

std::string get_name(Bot bot, int idx);
Bot get_bot(const std::string& name);
Move bot_move(const GameState& gs, const BattleSnake& you, Bot bot);
Move bot_move(const Game& g, const std::string& id, Bot bot);

#endif  // SNAKE_PRESETS_H
