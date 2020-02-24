// Copyright 2019 DeepMind Technologies Ltd. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef THIRD_PARTY_OPEN_SPIEL_GAMES_RBG2OPENS_H_
#define THIRD_PARTY_OPEN_SPIEL_GAMES_RBG2OPENS_H_

#include <array>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "open_spiel/spiel.h"
#include "open_spiel/games/rbg_game.hpp"

// Simple game of Noughts and Crosses:
// https://en.wikipedia.org/wiki/Tic-tac-toe
//
// Parameters: none

namespace open_spiel {
namespace rbg2opens {

/*
// Constants.
inline constexpr int kNumPlayers = 2;
inline constexpr int kNumRows = 3;
inline constexpr int kNumCols = 3;
inline constexpr int kNumCells = kNumRows * kNumCols;
inline constexpr int kCellStates = 1 + kNumPlayers;  // empty, 'x', and 'o'.

// https://math.stackexchange.com/questions/485752/tictactoe-state-space-choose-calculation/485852
inline constexpr int kNumberStates = 5478;

// State of a cell.
enum class CellState {
  kEmpty,
  kNought,
  kCross,
};
*/
// State of an in-play game.
Action rbg2opensAction(rbg_game::action_representation action);
rbg_game::action_representation opens2rbgAction(Action action_id);

class Rbg2OpenSState : public State {
 public:
  Rbg2OpenSState(std::shared_ptr<const Game> game);

  Rbg2OpenSState(const Rbg2OpenSState&) = default;
  Rbg2OpenSState& operator=(const Rbg2OpenSState&) = default;

  Player CurrentPlayer() const override;
  std::string ActionToString(Player player, Action action_id) const override;
  std::string ToString() const override;
  bool IsTerminal() const override;
  std::vector<double> Returns() const override;
  std::string InformationStateString(Player player) const override;
  std::string ObservationString(Player player) const override;
  //void ObservationTensor(Player player,
  //                       std::vector<double>* values) const override;
  std::unique_ptr<State> Clone() const override;
  void UndoAction(Player player, Action move) override;
  std::vector<Action> LegalActions() const override;

 protected:
  void DoApplyAction(Action move) override;

 private:
  mutable rbg_game::game_state rbg_state = rbg_game::game_state();
  mutable rbg_game::resettable_bitarray_stack rbg_cache = rbg_game::resettable_bitarray_stack();
  mutable std::vector<rbg_game::semimove> rbg_moves;
  mutable std::vector<Action> legal_moves;
  mutable bool isLegalMovesProcessed = false;
  const int KEEPER = 0;
  int num_moves_ = 0;
};

// Game object.
class Rbg2OpenSGame : public Game {
 public:
  explicit Rbg2OpenSGame(const GameParameters& params);
  int NumDistinctActions() const override { return rbg_game::NUMBER_OF_MODIFIERS * rbg_game::BOARD_SIZE; }
  std::unique_ptr<State> NewInitialState() const override {
    return std::unique_ptr<State>(new Rbg2OpenSState(shared_from_this()));
  }
  int NumPlayers() const override { return rbg_game::NUMBER_OF_PLAYERS - 1; }
  double MinUtility() const override { return 0; }
  double MaxUtility() const override { return 100; }
  std::shared_ptr<const Game> Clone() const override {
    return std::shared_ptr<const Game>(new Rbg2OpenSGame(*this));
  }
  int MaxGameLength() const override { return INT_MAX; }
};

}  // namespace rbg2opens
}  // namespace open_spiel

#endif  // THIRD_PARTY_OPEN_SPIEL_GAMES_RBG2OPENS_H_
