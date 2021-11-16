/** Header file for battlesnake.cpp.
@file battlesnake.h
@author YOUR NAME(S) HERE
*/
// TODO: Fill in your name(s) above.

#include <iostream>
#include <vector>
#include "lib/snake.h"

#ifndef BATTLESNAKE_H
#define BATTLESNAKE_H

// Optional constants for battlesnake.com.
extern const std::string SNAKE_HEAD;
extern const std::string SNAKE_TAIL;
extern const std::string SNAKE_COLOR;
extern const std::string USERNAME;

// Represents a snake, dead or alive.
struct BattleSnake {
  std::string id;
  int health;
  int length;
  std::vector<Pos> body;
  Pos head;
};

// Represents the state of the game.
struct GameState {
  std::string id;
  int turn;
  int height;
  int width;
  std::vector<Pos> food;
  std::vector<BattleSnake> snakes;
};

// DO NOT MODIFY CODE ABOVE

// Calculates a new position given a current position and a pending move.
Pos new_position(const Pos& current, Move move);

// Returns whether a given position is out of bounds.
bool out_of_bounds(const GameState& gs, const Pos& position);

// TODO (Optional): Declare your own helper functions here.

// DO NOT MODIFY CODE BELOW

// Chooses a move for the CUSTOM snake based on the game state.
Move bot_move(const GameState& gs, const BattleSnake& you);

// Specifies how to print out a BattleSnake for debugging purposes.
std::ostream& operator<<(std::ostream& os, const BattleSnake& s);

// Specifies how to print out a GameState for debugging purposes.
std::ostream& operator<<(std::ostream& os, const GameState& gs);

#endif  // BATTLESNAKE_H
