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

#include "open_spiel/games/rbg2openS.h"

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "open_spiel/spiel_utils.h"
#include "open_spiel/utils/tensor_view.h"

namespace open_spiel {
namespace rbg2opens {
namespace {

// Facts about the game.
const GameType kGameType{
    /*short_name=*/"rbg2opens",
    /*long_name=*/"Rbg To Open Spiel",
    GameType::Dynamics::kSequential,
    GameType::ChanceMode::kDeterministic,
    GameType::Information::kPerfectInformation,
    GameType::Utility::kGeneralSum,
    GameType::RewardModel::kTerminal,
    /*max_num_players=*/rbg_game::NUMBER_OF_PLAYERS - 1,
    /*min_num_players=*/rbg_game::NUMBER_OF_PLAYERS - 1,
    /*provides_information_state_string=*/true,
    /*provides_information_state_tensor=*/false,
    /*provides_observation_string=*/true,
    /*provides_observation_tensor=*/false,
    /*parameter_specification=*/{}  // no parameters
};

std::shared_ptr<const Game> Factory(const GameParameters& params) {
  return std::shared_ptr<const Game>(new Rbg2OpenSGame(params));
}

REGISTER_SPIEL_GAME(kGameType, Factory);

}  // namespace

Action rbg2opensAction(rbg_game::action_representation action) {
  Action result = 0;  
  result += (long long)rbg_game::action_to_modifier_index(action.index) * rbg_game::BOARD_SIZE;
  result += action.cell-1;
  return result;
}

rbg_game::action_representation opens2rbgAction(Action action_id) {
  int cell = action_id % rbg_game::BOARD_SIZE;
  int index = (action_id - cell) / rbg_game::BOARD_SIZE;
  cell += 1;
  index = rbg_game::modifier_to_action_index(index);
  return rbg_game::action_representation(index, cell);
}

void Rbg2OpenSState::DoApplyAction(Action move) {
  
  rbg_state.apply_semimove(moves_map[move]);
  num_moves_ += 1;
  
  isLegalMovesProcessed = false;
  while(rbg_state.get_current_player() == KEEPER){
    if(!rbg_state.apply_any_move(rbg_cache)){
      legal_moves.clear();
      isLegalMovesProcessed = true;
      break;
    }
  }
}

std::vector<Action> Rbg2OpenSState::LegalActions() const {
  if(isLegalMovesProcessed){
    return legal_moves;
  }
  unsigned int big_number = 999;
  legal_moves.clear();
  std::vector<rbg_game::semimove> tmp;
  rbg_state.get_all_semimoves(rbg_cache, rbg_moves, 1);
  for(auto& el: rbg_moves){
    Action act = rbg2opensAction(el.get_actions()[0]);
    moves_map[act] = rbg_game::semimove(el);
    rbg_game::revert_information ri = rbg_state.apply_semimove_with_revert(el);
    rbg_state.get_all_semimoves(rbg_cache, tmp, big_number);
    if(!tmp.empty()){
      legal_moves.push_back(act);
    }
    rbg_state.revert(ri);
  }
  sort(legal_moves.begin(), legal_moves.end());
  isLegalMovesProcessed = true;
  return legal_moves;
}

std::string Rbg2OpenSState::ActionToString(Player player,
                                           Action action_id) const {
  return std::to_string(player) + " " +std::to_string(action_id);
}

Rbg2OpenSState::Rbg2OpenSState(std::shared_ptr<const Game> game) : State(game) {
  moves_map.resize(NUMBER_OF_POSSIBLE_MOVES, rbg_game::semimove({}, 0, 0));
  
  while(rbg_state.get_current_player() == KEEPER){
    if(!rbg_state.apply_any_move(rbg_cache)){
      break;
    }
  }
}

Player Rbg2OpenSState::CurrentPlayer() const {
  return (IsTerminal() ? kTerminalPlayerId : rbg_state.get_current_player());
}

std::string Rbg2OpenSState::ToString() const {
  std::string res="";
  res += "BOARD:\n";
  for(int i=0; i<rbg_game::BOARD_SIZE; i++){
    res += rbg_game::piece_to_string(rbg_state.get_piece(i));
    res += ", ";
  }
  res += "\n";
  res += "VARIABLES:\n";
  for(int i=0; i<rbg_game::NUMBER_OF_VARIABLES; i++){
    res += rbg_game::variable_to_string(i);
    res += " = ";
    res += std::to_string(rbg_state.get_variable_value(i));
    res += ", ";
  }
  res+="\n";
  return res;
}

bool Rbg2OpenSState::IsTerminal() const {
  return LegalActions().empty();
}

std::vector<double> Rbg2OpenSState::Returns() const {
  std::vector<double>result;
  for(int i=1; i<rbg_game::NUMBER_OF_PLAYERS; i++){
    result.push_back(rbg_state.get_player_score(i));
  }
  return result;
}

std::string Rbg2OpenSState::InformationStateString(Player player) const {
  return HistoryString();
}

std::string Rbg2OpenSState::ObservationString(Player player) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, num_players_);
  return ToString();
}
/*
void Rbg2OpenSState::ObservationTensor(Player player,
                                       std::vector<double>* values) const {
  SPIEL_CHECK_GE(player, 0);
  SPIEL_CHECK_LT(player, num_players_);
  // Treat `values` as a 2-d tensor.
  TensorView<2> view(values, {kCellStates, kNumCells}, true);
  for (int cell = 0; cell < kNumCells; ++cell) {
    view[{static_cast<int>(board_[cell]), cell}] = 1.0;
  }
}
*/
void Rbg2OpenSState::UndoAction(Player player, Action move) {
  if(history_.size()>0){
    rbg_state = rbg_game::game_state();
    rbg_cache = rbg_game::resettable_bitarray_stack();
    isLegalMovesProcessed = false;
    while(CurrentPlayer() == KEEPER){
      if(!rbg_state.apply_any_move(rbg_cache)){
        break;
      }
    }
    history_.pop_back();
    num_moves_ -= 1;
    for(auto& ac : history_){
      ApplyAction(ac);
    }
  }
}

std::unique_ptr<State> Rbg2OpenSState::Clone() const {
  return std::unique_ptr<State>(new Rbg2OpenSState(*this));
}

Rbg2OpenSGame::Rbg2OpenSGame(const GameParameters& params)
    : Game(kGameType, params) {}

}  // namespace rbg2opens
}  // namespace open_spiel