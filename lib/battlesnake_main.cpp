// C++ wrapper for http://play.battlesnake.com.
// Credit: https://github.com/DAChenScratch/Starter-Battlesnake-Cpp-with-replit

#include <iostream>
#include "../battlesnake.h"
#include "httplib.h"
#include "json.hpp"
using namespace std;
using namespace nlohmann;

int convert_x(int x) { return x + 1; }

int convert_y(int y, int height) { return height - y; }

BattleSnake parse_snake(const json& j, int height) {
  BattleSnake s;
  s.id = j["id"];
  s.health = j["health"];
  s.length = j["length"];
  for (const json& body : j["body"]) {
    Pos p = {.x = convert_x(body["x"]), .y = convert_y(body["y"], height)};
    s.body.push_back(p);
  }
  s.head = {.x = convert_x(j["head"]["x"]),
            .y = convert_y(j["head"]["y"], height)};
  return s;
}

int main() {
  httplib::Server svr;

  svr.Get("/", [](const auto&, auto& res) {
    res.set_content("{\"apiversion\":\"1\", \"head\":\"" + SNAKE_HEAD +
                        "\", \"tail\":\"" + SNAKE_TAIL + "\", \"color\":\"" +
                        SNAKE_COLOR + "\", " + "\"author\":\"" + USERNAME +
                        "\"}",
                    "application/json");
  });

  svr.Post("/end",
           [](const auto&, auto& res) { res.set_content("ok", "text/plain"); });

  svr.Post("/start",
           [](const auto&, auto& res) { res.set_content("ok", "text/plain"); });

  svr.Post("/move", [](auto& req, auto& res) {
    const json data = json::parse(req.body);

    GameState gs;
    gs.turn = data["turn"];
    gs.id = data["game"]["id"];
    gs.height = data["board"]["height"];
    gs.width = data["board"]["width"];
    for (const json& j : data["board"]["food"]) {
      Pos p = {.x = convert_x(j["x"]), .y = convert_y(j["y"], gs.height)};
      gs.food.push_back(p);
    }
    for (const json& j : data["board"]["snakes"]) {
      gs.snakes.push_back(parse_snake(j, gs.height));
    }
    BattleSnake you = parse_snake(data["you"], gs.height);

    Move move = bot_move(gs, you);
    string move_str;
    switch (move) {
      case NONE:
      case UP:
        move_str = "up";
        break;
      case DOWN:
        move_str = "down";
        break;
      case LEFT:
        move_str = "left";
        break;
      case RIGHT:
        move_str = "right";
        break;
    }

    res.set_content("{\"move\": \"" + move_str + "\"}", "text/plain");
  });

  svr.listen("0.0.0.0", 8080);
  cout << "Server started" << endl;
}
