/** Contains tests for your code.
@file battlesnake_test.cpp
@author YOUR NAME(S) HERE
*/
// TODO: Fill in your name(s) above.

#include "battlesnake.h"
#include <gtest/gtest.h>
#include <set>
#include <vector>
#include "lib/snake.h"
#include "lib/snake_logic.h"
#include "lib/snake_presets.h"
using namespace std;

TEST(BattlesnakeTest, NewPosition) {
  Pos actual;
  Pos expected;

  actual = new_position({.x = 2, .y = 2}, UP);
  expected = {.x = 2, .y = 1};
  EXPECT_EQ(actual, expected);

  actual = new_position({.x = 2, .y = 2}, DOWN);
  expected = {.x = 2, .y = 3};
  EXPECT_EQ(actual, expected);

  actual = new_position({.x = 2, .y = 2}, LEFT);
  expected = {.x = 1, .y = 2};
  EXPECT_EQ(actual, expected);

  actual = new_position({.x = 2, .y = 2}, RIGHT);
  expected = {.x = 3, .y = 2};
  EXPECT_EQ(actual, expected);

  actual = new_position({.x = 2, .y = 2}, NONE);
  expected = {.x = 2, .y = 2};
  EXPECT_EQ(actual, expected);
}

TEST(BattlesnakeTest, OutOfBounds) {
  GameState gs;
  gs.width = 5;
  gs.height = 7;

  // Out of bounds on the left
  EXPECT_TRUE(out_of_bounds(gs, {.x = -1, .y = 5}));

  // Touches left walls
  EXPECT_TRUE(out_of_bounds(gs, {.x = 0, .y = 5}));

  // Just in bounds
  EXPECT_FALSE(out_of_bounds(gs, {.x = 1, .y = 5}));
  EXPECT_FALSE(out_of_bounds(gs, {.x = 5, .y = 5}));

  // Touches right walls
  EXPECT_TRUE(out_of_bounds(gs, {.x = 6, .y = 5}));

  // Out of bounds on the right
  EXPECT_TRUE(out_of_bounds(gs, {.x = 7, .y = 5}));

  // Out of bounds on the top
  EXPECT_TRUE(out_of_bounds(gs, {.x = 2, .y = -1}));

  // Touches top walls
  EXPECT_TRUE(out_of_bounds(gs, {.x = 2, .y = 0}));

  // Just in bounds
  EXPECT_FALSE(out_of_bounds(gs, {.x = 2, .y = 1}));
  EXPECT_FALSE(out_of_bounds(gs, {.x = 2, .y = 7}));

  // Touches bottom walls
  EXPECT_TRUE(out_of_bounds(gs, {.x = 2, .y = 8}));

  // Out of bounds on the bottom
  EXPECT_TRUE(out_of_bounds(gs, {.x = 2, .y = 9}));
}

// TODO (Optional): Add unit tests for your own helper functions here.