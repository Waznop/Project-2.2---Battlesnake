BATTLE        := lib/battlesnake_main.cpp
DRIVER        := lib/snake_main.cpp
GTEST         := googletest
TEST          := battlesnake_test.cpp
HEADERS       := lib/snake.h battlesnake.h lib/snake_logic.h lib/snake_presets.h
IMPLEMS       := battlesnake.cpp lib/snake_logic.cpp
OBJECT_IMPLEM := lib/snake_presets.cpp
OBJECT        := lib/snake_presets.o
FILES         := $(DRIVER) $(HEADERS) $(IMPLEMS) $(TEST) $(BATTLE)
TEST_OUT      := test
BATTLE_OUT    := battle
OUT           := snake
FLAGS         := -Wall -Werror -Wno-unused-function

.PHONY: format build test all clean battle object

all: format build test

format:
	@echo "\n========================\nFormatting files\n========================\n"
	@clang-format $(FILES) -i -style=google -verbose
	@echo "\n========================\nFormatting complete\n========================\n"

build: $(DRIVER) $(HEADERS) $(IMPLEMS)
	@echo "\n========================\nCompiling Snake\n========================\n"
	@g++ -std=c++17 $(IMPLEMS) $(OBJECT) $(DRIVER) $(FLAGS) -o $(OUT) -lncurses
	@echo "========================\nCompilation complete\nRun your code with ./$(OUT)\n========================\n"

battle: $(BATTLE) $(HEADERS) $(IMPLEMS)
	@echo "\n========================\nCompiling Battlesnake\n========================\n"
	@g++ -std=c++17 $(IMPLEMS) $(OBJECT) $(BATTLE) $(FLAGS) -o $(BATTLE_OUT) -pthread
	@echo "========================\nCompilation complete\nRun your code with ./$(BATTLE_OUT)\n========================\n"

object:
	@echo "\n========================\nBuilding object\nYou shouldn't need to do this\n========================\n"
	@g++ -std=c++17 $(OBJECT_IMPLEM) $(FLAGS) -c -o $(OBJECT)
	@echo "\n========================\nBuilding complete\n========================\n"

test:
	@echo "\n========================\nRunning tests\n========================\n"
	@g++ -std=c++17 $(IMPLEMS) $(OBJECT) $(TEST) $(GTEST)/libgtest.a $(GTEST)/libgtest_main.a -isystem $(GTEST)/include -pthread -o $(TEST_OUT)
	@./$(TEST_OUT)
	@echo "\n========================\nTests complete\n========================\n"

clean:
	@rm -f $(OUT) $(TEST_OUT) $(BATTLE_OUT)
