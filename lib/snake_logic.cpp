/** Contains logic for the Snake game.
@file snake_logic.cpp
@author Hong Tai Wei
*/

#include "snake_logic.h"
#include <iostream>
#include <set>
#include <vector>
#include "../battlesnake.h"
#include "snake.h"
#include "snake_presets.h"
using namespace std;

const int START_LEN = 3;
const int START_HP = 100;
const int START_SPAWN = 3;

bool place_food(Game& game) {
  if (game.free.empty()) {
    return false;
  }

  int idx = rand() % game.free.size();
  Pos p = *next(game.free.begin(), idx);
  game.free.erase(p);
  game.food.insert(p);
  return true;
}

bool place_snake(Game& game, const string& id) {
  if (game.free.empty()) {
    return false;
  }

  // Check for id duplication
  for (const Snake& s : game.snakes) {
    if (s.id == id) {
      return false;
    }
  }

  int idx = rand() % game.free.size();
  Pos p = *next(game.free.begin(), idx);
  game.free.erase(p);

  Snake snake;
  snake.id = id;
  snake.health = START_HP;
  snake.length = START_LEN;
  snake.head = p;
  snake.move = p.x > (WIDTH + 1) / 2.0 ? LEFT : RIGHT;
  for (int i = 0; i < START_LEN; i++) {
    snake.body.push_front(p);
  }

  game.snakes.push_back(snake);
  return true;
}

void move_head(Snake& s) {
  switch (s.move) {
    case NONE:
    case UP:
      s.head.y--;
      break;
    case DOWN:
      s.head.y++;
      break;
    case LEFT:
      s.head.x--;
      break;
    case RIGHT:
      s.head.x++;
      break;
  }
}

bool check_collision(Game& game, Snake& s) {
  if (s.head.x == 0 || s.head.x == WIDTH + 1 || s.head.y == 0 ||
      s.head.y == HEIGHT + 1) {
    return true;
  }

  for (Snake& other : game.snakes) {
    if (other.health <= 0) {
      continue;
    }
    // Check for head collision (other than self)
    if (s.head == other.head && s.id != other.id) {
      return true;
    }

    // Check for body collision
    for (size_t i = 1; i < other.body.size(); i++) {
      if (s.head == other.body[i]) {
        return true;
      }
    }
  }

  return false;
}

Game new_game(bool human_player, const vector<Bot>& bots, int spawn_timer) {
  Game game;
  game.snakes_alive = 0;
  game.turn = 0;
  game.spawn_timer = spawn_timer;

  for (int i = 1; i < HEIGHT + 1; i++) {
    for (int j = 1; j < WIDTH + 1; j++) {
      game.free.insert({.x = j, .y = i});
    }
  }

  if (human_player && place_snake(game, PLAYER)) {
    game.snakes_alive++;
  }

  size_t i = 0;
  while (i < bots.size() && place_snake(game, get_name(bots[i], i))) {
    game.snakes_alive++;
    i++;
  }

  for (int i = 0; i < START_SPAWN; i++) {
    place_food(game);
  }

  return game;
}

void take_turn(Game& game, Move player_move) {
  if (game.snakes_alive <= 0) {
    return;
  }

  set<string> snakes_to_kill;
  set<Pos> food_to_remove;

  for (Snake& s : game.snakes) {
    if (s.health <= 0) {
      continue;
    }

    // Determine move
    if (s.id != PLAYER) {
      Bot bot = get_bot(s.id);
      s.move = bot_move(game, s.id, bot);
    } else if (player_move != NONE) {
      s.move = player_move;
    }
    move_head(s);
  }

  for (Snake& s : game.snakes) {
    if (s.health <= 0) {
      continue;
    }

    // Check for food and apply move
    if (game.food.count(s.head)) {
      food_to_remove.insert(s.head);
      s.health = START_HP;
      s.length++;
    } else {
      game.free.insert(s.body.back());
      s.body.pop_back();
    }
    game.free.erase(s.head);
    s.body.push_front(s.head);
  }

  for (Snake& s : game.snakes) {
    if (s.health <= 0) {
      continue;
    }
    // Check collision
    if (check_collision(game, s)) {
      snakes_to_kill.insert(s.id);
    }
  }

  for (Snake& s : game.snakes) {
    if (s.health <= 0) {
      continue;
    }
    // Apply hunger
    s.health--;
    if (s.health <= 0) {
      snakes_to_kill.insert(s.id);
    }
  }

  // Remove eaten food
  for (const Pos& p : food_to_remove) {
    game.food.erase(p);
  }

  // Remove killed snakes
  for (Snake& s : game.snakes) {
    if (snakes_to_kill.count(s.id) > 0) {
      s.health = 0;
      game.snakes_alive--;
      for (const Pos& p : s.body) {
        game.free.insert(p);
      }
    }
  }

  game.turn++;
  if (game.turn % game.spawn_timer == 0) {
    place_food(game);
  }
}
