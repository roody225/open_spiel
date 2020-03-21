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
  for(auto& ac: moves_map[move]){
    rbg_state.apply_semimove(ac);
  }
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

void Rbg2OpenSState::gen_all_moves(int act, int nr, int plr, std::vector<Action>& legal_moves, std::vector<rbg_game::semimove>& acc) const{
  if(plr != rbg_state.get_current_player()){
    if(moves_map[act].empty()){
      for(auto& el: acc){
        moves_map[act].push_back(el);
      }
    }
    legal_moves.push_back(act);
    return;
  }
  if(nr == SEMIMOVES_IN_MOVE){
    std::vector<rbg_game::semimove> tmp;
    rbg_state.get_all_semimoves(rbg_cache, tmp, 999);
    if(!tmp.empty()){
      if(moves_map[act].empty()){
        moves_map[act] = acc;
      }
      legal_moves.push_back(act);
      return;
    }
  }
  rbg_state.get_all_semimoves(rbg_cache, semitmp[nr], 1);
  for(auto& el: semitmp[nr]){
    Action ac = rbg2opensAction(el.get_actions()[0]);
    ac = (ac<<(nr * MAX_ACTION_SIZE)) + act + 1;
    rbg_game::revert_information ri = rbg_state.apply_semimove_with_revert(el);
    acc.push_back(el);
    gen_all_moves(act+ac, nr+1, plr, legal_moves, acc);
    acc.pop_back();
    rbg_state.revert(ri);
  }
}
/*
void Rbg2OpenSState::calculate_legal_moves() const{
  rbg_state.get_all_semimoves(rbg_cache, tmp0, 1);
  for(auto& el: tmp0){
    rbg_game::revert_information ri = rbg_state.apply_semimove_with_revert(el);
    rbg_state.get_all_semimoves(rbg_cache, tmp1, 1);
      for(auto& e: tmp1){
        rbg_game::revert_information rii = rbg_state.apply_semimove_with_revert(e);
        Action act = (rbg2opensAction(e.get_actions()[0]) << MAX_ACTION_SIZE) + rbg2opensAction(el.get_actions()[0]) + 1;
        legal_moves.push_back(act);
        if(moves_map[act].empty()){
          moves_map[act].push_back(el);
          moves_map[act].push_back(e);
        }
        rbg_state.revert(rii);
        }
      rbg_state.revert(ri);
    }
  sort(legal_moves.begin(), legal_moves.end());
  isLegalMovesProcessed = true;
}
*/

std::vector<Action> Rbg2OpenSState::LegalActions() const {
  if(!isLegalMovesProcessed){
    legal_moves.clear();
    //calculate_legal_moves();
    gen_all_moves(0, 0, rbg_state.get_current_player(), legal_moves, acc);
    kx::radix_sort(legal_moves.begin(), legal_moves.end());
    isLegalMovesProcessed = true;
  }
  return legal_moves;
}

std::string Rbg2OpenSState::ActionToString(Player player,
                                           Action action_id) const {
  return std::to_string(player) + " " +std::to_string(action_id);
}

int calculate_action_size(){
  int x = rbg_game::BOARD_SIZE * rbg_game::NUMBER_OF_MODIFIERS + 1;
  int s = 0;
  while(x){
    s++;
    x = (x >> 1);
  }
  return s;
}
const int Rbg2OpenSState::MAX_ACTION_SIZE = calculate_action_size();
const int Rbg2OpenSState::SEMIMOVES_IN_MOVE = std::max(1, 22/MAX_ACTION_SIZE);
std::vector<std::vector<rbg_game::semimove>> Rbg2OpenSState::moves_map;
rbg_game::resettable_bitarray_stack Rbg2OpenSState::rbg_cache = rbg_game::resettable_bitarray_stack();
std::vector<std::vector<rbg_game::semimove>> Rbg2OpenSState::semitmp;

Rbg2OpenSState::Rbg2OpenSState(std::shared_ptr<const Game> game) : State(game) {
  if(semitmp.size()==0)
    semitmp.resize(SEMIMOVES_IN_MOVE);
  if(moves_map.size()==0){
    moves_map.resize(std::max(NUMBER_OF_POSSIBLE_ACTIONS+1, (NUMBER_OF_POSSIBLE_ACTIONS<<(MAX_ACTION_SIZE * (SEMIMOVES_IN_MOVE-1))) + NUMBER_OF_POSSIBLE_ACTIONS+1));
  }
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
  if(!isLegalMovesProcessed){
    legal_moves.clear();
    //calculate_legal_moves();
    std::vector<rbg_game::semimove> acc;
    gen_all_moves(0, 0, rbg_state.get_current_player(), legal_moves, acc);
    kx::radix_sort(legal_moves.begin(), legal_moves.end());
    isLegalMovesProcessed = true;
  }
  return legal_moves.empty();
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
