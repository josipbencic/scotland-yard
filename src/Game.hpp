#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include <optional>
#include <random>
#include <array>

using Position = int;

inline constexpr int TOTAL_POLICE = 1;
inline constexpr int MAX_TURNS = 22;

enum class Currency : int {
  Taxi = 0,
  Bus = 1,
  Underground = 2,
  SecretMove = 3,
  NoMove = 4
};

enum class GameStatus : int {
  Running,
  ThiefWon,
  PoliceWon
};

struct Move {
  std::optional<Position> position;
  Currency currency;
};

using CityMap = std::vector<std::vector<Move>>;

struct PoliceState {
  Move thiefMove;
};

struct ThiefState {
  std::array<Move, TOTAL_POLICE> policeMoves; 
};

class Game {

private:

  CityMap cityMap;

  Position thiefPosition;

  std::array<int, 4> thiefCurrency;

  std::array<Position, TOTAL_POLICE> policePositions;

  std::array<std::array<int, 4>, TOTAL_POLICE> policeCurrency;

  std::vector<Move> history;

  int turnNumber;

  GameStatus status;

public:

  Game(CityMap& graph, std::mt19937& generator);

  std::array<Position, TOTAL_POLICE> initPolice() const;

  Position initThief() const;

  void thiefMove(Move m);

  void policeMove(std::vector<Move>& moves);

  PoliceState policeState() const;

  ThiefState thiefState() const;

  GameStatus gameStatus() const;

private:

  void ThiefWon();

  void PoliceWon();

  bool isValidPoliceMove(Move m, int policeman);

  bool isValidThiefMove(Move m);

  std::optional<Position> thiefPositionIfVisible() const;
};

#endif
