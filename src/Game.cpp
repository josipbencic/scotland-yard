#include "Game.hpp"

#include <algorithm>

Game::Game(CityMap& graph, std::mt19937& generator)
  : thiefCurrency{4, 3, 3, 4}
  , turnNumber{1}
  , status{GameStatus::Running}
  , cityMap(graph) {

  policeCurrency.fill({10, 8, 4, 0});

  const auto maxPosOnMap = static_cast<int>(cityMap.size()) - 1;
  std::uniform_int_distribution<> uniform(1, maxPosOnMap);

  std::vector<int> positions;
  positions.reserve(TOTAL_POLICE + 1);
  while (positions.size() < (TOTAL_POLICE + 1)) {
    auto newPosition = uniform(generator);
    if (std::find(begin(positions), end(positions), newPosition) == end(positions)) {
      positions.push_back(newPosition);
    }
  }

  thiefPosition = positions[0];
  std::copy(begin(positions) + 1, end(positions), begin(policePositions));
}

void Game::thiefMove(Move m) {
  if (!isValidThiefMove(m)) {
    PoliceWon();
    return;
  }

  thiefCurrency[static_cast<size_t>(m.currency)]--;
  thiefPosition = m.position.value();
  history.push_back(m);
}

bool Game::isValidThiefMove(Move m) {
  if (!m.position.has_value()) {
    return false;
  }
  if (m.currency == Currency::NoMove) {
    return false;
  }
  if (thiefCurrency[static_cast<int>(m.currency)] == 0) {
    return false;
  }

  if (std::none_of(
    begin(cityMap[thiefPosition]),
    end(cityMap[thiefPosition]),
    [m](Move edge) {
      return edge.position.value() == m.position.value() && edge.currency == m.currency;
    }
  )) {
    return false;
  }

  if (std::find(begin(policePositions), end(policePositions), m.position.value())) {
    return false;
  }

  return true;
}

void Game::policeMove(std::vector<Move>& moves) {
  if (moves.size() != static_cast<size_t>(TOTAL_POLICE)) {
    ThiefWon();
    return;
  }
  for (int i = 0; i < TOTAL_POLICE; ++i) {
    if (!isValidPoliceMove(moves[i], i)) {
      ThiefWon();
    }
    policePositions[i] = moves[i].position.value();
    policeCurrency[i][static_cast<size_t>(moves[i].currency)]--;
    history.push_back(moves[i]);
  }
  for (auto& position : policePositions) {
    if (position == thiefPosition) {
      PoliceWon();
    }
  }
  ++turnNumber;
}

bool Game::isValidPoliceMove(Move m, int policeman) {
  if (m.currency == Currency::NoMove) {
    for (auto& edge : cityMap[policePositions[policeman]]) {
      if (isValidPoliceMove(edge, policeman)) {
        return false;
      }
    }
    return true;
  }

  if (!m.position.has_value()) {
    return false;
  }
  if (policeCurrency[policeman][static_cast<size_t>(m.currency)] == 0) {
    return false;
  }
  if (std::find(begin(policePositions), end(policePositions), m.position.value())) {
    return false;
  }
  const auto position = policePositions[policeman];
  if (std::none_of(
    begin(cityMap[position]),
    end(cityMap[position]),
    [m](Move edge) {
      return edge.position.value() == m.position.value() && edge.currency == m.currency;
    }
  )) {
    return false;
  }

  return true;
}

void Game::ThiefWon() {
  status = GameStatus::ThiefWon;
}

void Game::PoliceWon() {
  status = GameStatus::PoliceWon;
}

GameStatus Game::gameStatus() const {
  return status;
}

ThiefState Game::thiefState() const {
  std::array<Move, TOTAL_POLICE> policeMoves;
  if (history.size() < static_cast<size_t>(TOTAL_POLICE)) {
    copy(end(history) - TOTAL_POLICE, end(history), begin(policeMoves));
  }
  return {policeMoves};
}

PoliceState Game::policeState() const {
  return PoliceState{Move{ thiefPositionIfVisible(), history.back().currency }};
}

std::optional<Position> Game::thiefPositionIfVisible() const {
  static const std::array<int, 5> revealingPositions = {3, 8, 13, 18, 22};
  if (std::find(begin(revealingPositions), end(revealingPositions), turnNumber) == end(revealingPositions)) {
    return std::nullopt;
  }
  return {thiefPosition};
}

std::array<Position, TOTAL_POLICE> Game::initPolice() const {
  return policePositions;
}

Position Game::initThief() const {
  return thiefPosition;
}
