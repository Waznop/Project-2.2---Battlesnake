/** Runs Snake through the terminal.
@file snake_main.cpp
@author Hong Tai Wei
*/

#include <ncurses.h>
#include <chrono>
#include <iostream>
#include <set>
#include <thread>
#include "snake.h"
#include "snake_logic.h"
#include "snake_presets.h"
using namespace std;

enum Input {
  IN_NONE,
  IN_UP,
  IN_DOWN,
  IN_LEFT,
  IN_RIGHT,
  IN_RESTART,
  IN_QUIT,
};

void setup() {
  // Init ncurses mode
  initscr();
  // Take one char at a time
  cbreak();
  // Suppress echo of typed chars
  noecho();
  // Capture special chars
  keypad(stdscr, true);
  // Don't block on user input
  nodelay(stdscr, true);
  // Hide cursor
  curs_set(0);
}

void tear_down() {
  // End ncurses mode
  endwin();
}

Move to_move(Input input) {
  switch (input) {
    case IN_UP:
      return UP;
    case IN_DOWN:
      return DOWN;
    case IN_LEFT:
      return LEFT;
    case IN_RIGHT:
      return RIGHT;
    default:
      return NONE;
  }
}

Input handle_input_turn(Game& game) {
  char ch;
  cin >> ch;
  switch (ch) {
    case 'w':
      return IN_UP;
    case 's':
      return IN_DOWN;
    case 'a':
      return IN_LEFT;
    case 'd':
      return IN_RIGHT;
    case 'c':
      return IN_NONE;
    case 'r':
      return IN_RESTART;
    default:
      return IN_QUIT;
  }
}

Input handle_input(Game& game) {
  int ch = getch();
  switch (ch) {
    case 'w':
    case 'W':
    case KEY_UP:
      return IN_UP;
    case 's':
    case 'S':
    case KEY_DOWN:
      return IN_DOWN;
    case 'a':
    case 'A':
    case KEY_LEFT:
      return IN_LEFT;
    case 'd':
    case 'D':
    case KEY_RIGHT:
      return IN_RIGHT;
    case 'r':
    case 'R':
      return IN_RESTART;
    case 'q':
    case 'Q':
      return IN_QUIT;
    default:
      return IN_NONE;
  }
}

void draw_board(char board[HEIGHT + 2][WIDTH + 2], const Game& game) {
  set<Pos> player_heads;
  set<Pos> bot_heads;
  set<Pos> snake_body;
  for (const Snake& s : game.snakes) {
    if (s.health <= 0) {
      continue;
    }
    for (const Pos& p : s.body) {
      if (p != s.head) {
        snake_body.insert(p);
      } else if (s.id == PLAYER) {
        player_heads.insert(p);
      } else {
        bot_heads.insert(p);
      }
    }
  }
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      if (i == 0 || i == HEIGHT + 1 || j == 0 || j == WIDTH + 1) {
        board[i][j] = '#';
        continue;
      }
      Pos p = {.x = j, .y = i};
      if (player_heads.count(p)) {
        board[i][j] = '@';
      } else if (bot_heads.count(p)) {
        board[i][j] = '0';
      } else if (snake_body.count(p)) {
        board[i][j] = 'O';
      } else if (game.food.count(p)) {
        board[i][j] = '+';
      } else {
        board[i][j] = ' ';
      }
    }
  }
}

void handle_output(const Game& game) {
  clear();
  char board[HEIGHT + 2][WIDTH + 2];
  draw_board(board, game);
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      mvprintw(i, j, "%c", board[i][j]);
    }
  }
  mvprintw(HEIGHT + 2, 0, "Snake - (R)estart (Q)uit - Turn: %i", game.turn);
  mvprintw(HEIGHT + 3, 0, "Alive: %i", game.snakes_alive);
  for (size_t i = 0; i < game.snakes.size(); i++) {
    int line = HEIGHT + 4 + i;
    move(line, 0);
    clrtoeol();
    const Snake& s = game.snakes[i];
    mvprintw(line, 0, "ID: %s - HP: %i - Length: %i", s.id.data(), s.health,
             s.length);
  }
  refresh();
}

void handle_output_turn(const Game& game) {
  char board[HEIGHT + 2][WIDTH + 2];
  draw_board(board, game);
  for (int i = 0; i < HEIGHT + 2; i++) {
    for (int j = 0; j < WIDTH + 2; j++) {
      cout << board[i][j];
    }
    cout << endl;
  }
  cout << "Snake - WASD (C)ontinue (R)estart (Q)uit - Turn: " << game.turn
       << endl;
  cout << "Alive: " << game.snakes_alive << endl;
  for (const Snake& s : game.snakes) {
    cout << "ID: " << s.id << " - HP: " << s.health << " - Length: " << s.length
         << endl;
  }
}

void real_time_main(bool human_player, const vector<Bot>& bots,
                    int spawn_timer) {
  setup();
  Game game = new_game(human_player, bots, spawn_timer);
  while (true) {
    Input input = handle_input(game);
    if (input == IN_RESTART) {
      game = new_game(human_player, bots, spawn_timer);
      continue;
    } else if (input == IN_QUIT) {
      break;
    }
    take_turn(game, to_move(input));
    handle_output(game);
    this_thread::sleep_for(chrono::milliseconds(SPEED));
  }
  tear_down();
}

void turn_based_main(bool human_player, const vector<Bot>& bots,
                     int spawn_timer) {
  Game game = new_game(human_player, bots, spawn_timer);
  while (true) {
    handle_output_turn(game);
    Input input = handle_input_turn(game);
    if (input == IN_RESTART) {
      game = new_game(human_player, bots, spawn_timer);
      continue;
    } else if (input == IN_QUIT) {
      break;
    }
    take_turn(game, to_move(input));
  }
}

int main() {
  cout << "Play Snake in real time? (1/0)" << endl;
  bool real_time;
  cin >> real_time;

  cout << "How many turns between food spawns? (5-24)" << endl;
  int spawn_timer;
  cin >> spawn_timer;

  cout << "Number of humans? (1/0)" << endl;
  bool human_player;
  cin >> human_player;

  cout << "Number of bots? (0-4)" << endl;
  int num_bots;
  cin >> num_bots;

  vector<Bot> bots;
  for (int i = 0; i < num_bots; i++) {
    cout << "Select logic for bot" << i << ":"
         << "\n\t0 - CUSTOM (Your own bot)"
         << "\n\t1 - HUNGRY (Likes to eat)"
         << "\n\t2 - CIRCLE (Circles around)"
         << "\n\t3 - GREEDY (Follows pattern)"
         << "\n\t4 - RANDOM (Unpredictable)"
         << "\n\t5 - HEALTH (Eats periodically)" << endl;
    int bot_choice;
    cin >> bot_choice;
    Bot bot;
    switch (bot_choice) {
      case 1:
        bot = HUNGRY;
        break;
      case 2:
        bot = CIRCLE;
        break;
      case 3:
        bot = GREEDY;
        break;
      case 4:
        bot = RANDOM;
        break;
      case 5:
        bot = HEALTH;
        break;
      default:
        bot = CUSTOM;
    }
    bots.push_back(bot);
  }

  srand(time(0));
  if (real_time) {
    real_time_main(human_player, bots, spawn_timer);
  } else {
    turn_based_main(human_player, bots, spawn_timer);
  }
}