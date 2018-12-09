
#include "Game.hpp"

#include <iostream>
#include <random>

int main() {
  std::random_device rd;
  std::mt19937 gen(rd());

  CityMap graph = {
    {},
    {
      {{2}, Currency::Taxi}, {{3}, Currency::Underground}
    },
    {
      {{1}, Currency::SecretMove}, {{3}, Currency::Underground}
    },
    {
      {{1}, Currency::Bus}, {{2}, Currency::Underground}
    }
  };

  Game game(graph, gen);
  //  TODO: init bots with initPolice, initThief

  auto thief = game.initThief();
  auto police = game.initPolice()[0];


  while (game.gameStatus() == GameStatus::Running) {
    if (thief == 1) {
      game.thiefMove({2, Currency::Taxi});
    } else if (thief == 2) {
      game.thiefMove({3, Currency::Underground});
    } else {
      game.thiefMove({1, Currency::Bus});
    }
    Move m;
    if (police == 1) {
      m = {3, Currency::Underground};
    } else if (police == 2) {
      m = {1, Currency::SecretMove};
    } else {
      m = {2, Currency::Taxi};
    }
    std::vector<Move> movesss;
    movesss.push_back(m);
    game.policeMove(movesss);
  }

  if (GameStatus::ThiefWon == game.gameStatus()) {
    std::cout << "Thief Won!" << std::endl;
  } else {
    std::cout << "Police Won!" << std::endl;
  }

  return 0;
}