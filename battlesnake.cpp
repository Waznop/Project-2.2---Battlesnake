/** Contains logic for your Battlesnake.
@file battlesnake.cpp
@author YOUR NAME(S) HERE
*/
// TODO: Fill in your name(s) above.

#include "battlesnake.h"
#include <iostream>
#include "lib/snake.h"
using namespace std;

// TODO (Optional): Set a custom head for your Battlesnake.
const string SNAKE_HEAD = "default";
// TODO (Optional): Set a custom tail for your Battlesnake.
const string SNAKE_TAIL = "default";
// TODO (Optional): Set a custom color for your Battlesnake.
const string SNAKE_COLOR = "#ffb9a3";
// TODO (Optional): Change to your battlesnake.com username.
const string USERNAME = "username";

Pos new_position(const Pos& current, Move move) {
  switch (move) {
    case UP:
      return {.x = current.x, .y = current.y - 1};
    case DOWN:
      return {.x = current.x, .y = current.y + 1};
    case LEFT:
      return {.x = current.x - 1, .y = current.y};
    case RIGHT:
      return {.x = current.x + 1, .y = current.y};
    default:
      return current;
  }
}

bool out_of_bounds(const GameState& gs, const Pos& position) {
  // TODO: Implement this function.
  return false;
}

// TODO (Optional): Write your own helper functions here.

Move bot_move(const GameState& gs, const BattleSnake& you) {
  // TODO: Implement this function.
  return UP;
}

// DO NOT MODIFY CODE BELOW.

std::ostream& operator<<(std::ostream& os, const BattleSnake& s) {
  os << "snake: " << s.id << "\n\thealth: " << s.health
     << "\n\tlength: " << s.length << "\n\tbody: ";
  for (const Pos& p : s.body) {
    os << p << " ";
  }
  return os << "\n\thead: " << s.head << std::endl;
}

std::ostream& operator<<(std::ostream& os, const GameState& gs) {
  os << "game: " << gs.id << "\n\tturn: " << gs.turn
     << "\n\theight: " << gs.height << "\n\twidth: " << gs.width
     << "\n\tfood: ";
  for (const Pos& p : gs.food) {
    os << p << " ";
  }
  os << "\n--- snakes ---\n";
  for (const BattleSnake& s : gs.snakes) {
    os << s;
  }
  return os;
}