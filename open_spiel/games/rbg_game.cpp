#include "rbg_game.hpp"

#include <vector>

namespace rbg_game{
    static const std::string cells_names[64] = {
        "rx0y0",
        "rx1y0",
        "rx2y0",
        "rx3y0",
        "rx4y0",
        "rx5y0",
        "rx6y0",
        "rx7y0",
        "rx0y1",
        "rx1y1",
        "rx2y1",
        "rx3y1",
        "rx4y1",
        "rx5y1",
        "rx6y1",
        "rx7y1",
        "rx0y2",
        "rx1y2",
        "rx2y2",
        "rx3y2",
        "rx4y2",
        "rx5y2",
        "rx6y2",
        "rx7y2",
        "rx0y3",
        "rx1y3",
        "rx2y3",
        "rx3y3",
        "rx4y3",
        "rx5y3",
        "rx6y3",
        "rx7y3",
        "rx0y4",
        "rx1y4",
        "rx2y4",
        "rx3y4",
        "rx4y4",
        "rx5y4",
        "rx6y4",
        "rx7y4",
        "rx0y5",
        "rx1y5",
        "rx2y5",
        "rx3y5",
        "rx4y5",
        "rx5y5",
        "rx6y5",
        "rx7y5",
        "rx0y6",
        "rx1y6",
        "rx2y6",
        "rx3y6",
        "rx4y6",
        "rx5y6",
        "rx6y6",
        "rx7y6",
        "rx0y7",
        "rx1y7",
        "rx2y7",
        "rx3y7",
        "rx4y7",
        "rx5y7",
        "rx6y7",
        "rx7y7",
    };

    std::string board_cell_to_string(int cell_id){
        return cells_names[cell_id];
    }

    static const std::string pieces_names[3] = {
        "b",
        "e",
        "w",
    };

    std::string piece_to_string(int piece_id){
        return pieces_names[piece_id];
    }

    static const std::string variables_names[2] = {
        "black",
        "white",
    };

    std::string variable_to_string(int variable_id){
        return variables_names[variable_id];
    }

    int action_to_modifier_index(int action_index){
        switch(action_index){
            case 1:
                return 0;
            case 7:
                return 1;
            case 13:
                return 2;
            case 14:
                return 3;
            case 15:
                return 4;
            case 17:
                return 5;
            case 19:
                return 6;
            case 21:
                return 7;
            case 27:
                return 8;
            case 33:
                return 9;
            case 34:
                return 10;
            case 35:
                return 11;
            case 37:
                return 12;
            case 39:
                return 13;
            default:
                return -1;
        }
    }

    int modifier_to_action_index(int modifier_index){
        switch(modifier_index){
            case 0:
                return 1;
            case 1:
                return 7;
            case 2:
                return 13;
            case 3:
                return 14;
            case 4:
                return 15;
            case 5:
                return 17;
            case 6:
                return 19;
            case 7:
                return 21;
            case 8:
                return 27;
            case 9:
                return 33;
            case 10:
                return 34;
            case 11:
                return 35;
            case 12:
                return 37;
            case 13:
                return 39;
            default:
                return -1;
        }
    }

    static const int cell_neighbors[65][4] = {
        {0,0,0,0},
        {9,0,2,0},
        {10,1,3,0},
        {11,2,4,0},
        {12,3,5,0},
        {13,4,6,0},
        {14,5,7,0},
        {15,6,8,0},
        {16,7,0,0},
        {17,0,10,1},
        {18,9,11,2},
        {19,10,12,3},
        {20,11,13,4},
        {21,12,14,5},
        {22,13,15,6},
        {23,14,16,7},
        {24,15,0,8},
        {25,0,18,9},
        {26,17,19,10},
        {27,18,20,11},
        {28,19,21,12},
        {29,20,22,13},
        {30,21,23,14},
        {31,22,24,15},
        {32,23,0,16},
        {33,0,26,17},
        {34,25,27,18},
        {35,26,28,19},
        {36,27,29,20},
        {37,28,30,21},
        {38,29,31,22},
        {39,30,32,23},
        {40,31,0,24},
        {41,0,34,25},
        {42,33,35,26},
        {43,34,36,27},
        {44,35,37,28},
        {45,36,38,29},
        {46,37,39,30},
        {47,38,40,31},
        {48,39,0,32},
        {49,0,42,33},
        {50,41,43,34},
        {51,42,44,35},
        {52,43,45,36},
        {53,44,46,37},
        {54,45,47,38},
        {55,46,48,39},
        {56,47,0,40},
        {57,0,50,41},
        {58,49,51,42},
        {59,50,52,43},
        {60,51,53,44},
        {61,52,54,45},
        {62,53,55,46},
        {63,54,56,47},
        {64,55,0,48},
        {0,0,58,49},
        {0,57,59,50},
        {0,58,60,51},
        {0,59,61,52},
        {0,60,62,53},
        {0,61,63,54},
        {0,62,64,55},
        {0,63,0,56},
    };

    int get_neighbor(int cell_id, int edge_id){
        return cell_neighbors[cell_id+1][edge_id]-1;
    }

    static const int bounds[2] = {
        100,
        100,
    };

    int get_bound(int variable_id){
        return bounds[variable_id];
    }

    static const std::vector<int> shift_table1[65] = {
        {},
        {2,},
        {1,3,},
        {2,4,},
        {3,5,},
        {4,6,},
        {5,7,},
        {6,8,},
        {7,},
        {10,},
        {9,11,},
        {10,12,},
        {11,13,},
        {12,14,},
        {13,15,},
        {14,16,},
        {15,},
        {18,},
        {17,19,},
        {18,20,},
        {19,21,},
        {20,22,},
        {21,23,},
        {22,24,},
        {23,},
        {26,},
        {25,27,},
        {26,28,},
        {27,29,},
        {28,30,},
        {29,31,},
        {30,32,},
        {31,},
        {34,},
        {33,35,},
        {34,36,},
        {35,37,},
        {36,38,},
        {37,39,},
        {38,40,},
        {39,},
        {42,},
        {41,43,},
        {42,44,},
        {43,45,},
        {44,46,},
        {45,47,},
        {46,48,},
        {47,},
        {50,},
        {49,51,},
        {50,52,},
        {51,53,},
        {52,54,},
        {53,55,},
        {54,56,},
        {55,},
        {58,},
        {57,59,},
        {58,60,},
        {59,61,},
        {60,62,},
        {61,63,},
        {62,64,},
        {63,},
    };

    bool action_representation::operator==(const action_representation& rhs) const{
        return index == rhs.index and cell == rhs.cell;
    }
    move::move(const move_representation& mr)
    : mr(mr){
    }
    bool move::operator==(const move& rhs) const{
        return mr == rhs.mr;
    }
    semimove::semimove(const move_representation& mr, int cell, int state)
    : mr(mr)
    , cell(cell)
    , state(state){
    }
    const move_representation& semimove::get_actions(void)const{
        return mr;
    }
    bool semimove::operator==(const semimove& rhs) const{
        return cell == rhs.cell and state == rhs.state and mr == rhs.mr;
    }
    int game_state::get_current_cell(void)const{
        return current_cell-1;
    }

    int game_state::get_piece(int cell_id)const{
        return pieces[cell_id+1];
    }

    int game_state::get_variable_value(int variable_id)const{
        return variables[variable_id];
    }

    int game_state::get_current_player(void)const{
        return current_player;
    }

    int game_state::get_player_score(int player_id)const{
        return variables[player_id-1];
    }

    bool game_state::is_nodal(void)const{
        switch(current_state){
            case 1:
            case 10:
            case 16:
            case 18:
            case 27:
            case 33:
            case 35:
                return true;
            default:
                return false;
        }
    }

    revert_information game_state::apply_semimove_with_revert(const semimove& m){
        revert_information ri;
        ri.previous_cell = current_cell;
        ri.previous_player = current_player;
        ri.previous_state = current_state;
        for(const auto& el: m.mr){
            apply_action_with_revert(el, ri);
        }
        current_cell = m.cell;
        current_state = m.state;
        return ri;
    }

    void game_state::apply_semimove(const semimove& m){
        for(const auto& el: m.mr){
            apply_action(el);
        }
        current_cell = m.cell;
        current_state = m.state;
    }
    void game_state::apply_move(const move& m){
        for(const auto& el: m.mr){
            apply_action(el);
        }
    }

    std::vector<move> game_state::get_all_moves(resettable_bitarray_stack& cache){
        std::vector<move> result;
        result.reserve(100);
        get_all_moves(cache, result);
        return result;
    }

    void game_state::get_all_moves(resettable_bitarray_stack& cache, std::vector<move>& moves){
        std::vector<semimove> semimoves;
        get_all_semimoves(cache, semimoves, 1000);
        moves.clear();
        for(const auto& el: semimoves){
            moves.emplace_back(el.mr);
        }
    }

    void game_state::get_all_semimoves(resettable_bitarray_stack&, std::vector<semimove>& moves, unsigned int move_length_limit){
        moves.clear();
        move_representation mr;
        switch(current_state){
            case 0:
                get_all_moves_0_1(current_cell, mr, moves, move_length_limit);
                break;
            case 1:
                get_all_moves_1_2(current_cell, mr, moves, move_length_limit);
                break;
            case 2:
                get_all_moves_2_3(current_cell, mr, moves, move_length_limit);
                break;
            case 3:
                get_all_moves_3_5(current_cell, mr, moves, move_length_limit);
                get_all_moves_3_7(current_cell, mr, moves, move_length_limit);
                break;
            case 4:
                get_all_moves_4_10(current_cell, mr, moves, move_length_limit);
                break;
            case 5:
                get_all_moves_5_6(current_cell, mr, moves, move_length_limit);
                break;
            case 6:
                get_all_moves_6_4(current_cell, mr, moves, move_length_limit);
                break;
            case 7:
                get_all_moves_7_8(current_cell, mr, moves, move_length_limit);
                break;
            case 8:
                get_all_moves_8_9(current_cell, mr, moves, move_length_limit);
                break;
            case 9:
                get_all_moves_9_4(current_cell, mr, moves, move_length_limit);
                break;
            case 10:
                get_all_moves_10_11(current_cell, mr, moves, move_length_limit);
                break;
            case 11:
                get_all_moves_11_13(current_cell, mr, moves, move_length_limit);
                get_all_moves_11_15(current_cell, mr, moves, move_length_limit);
                break;
            case 12:
                get_all_moves_12_18(current_cell, mr, moves, move_length_limit);
                break;
            case 13:
                get_all_moves_13_14(current_cell, mr, moves, move_length_limit);
                break;
            case 14:
                get_all_moves_14_12(current_cell, mr, moves, move_length_limit);
                break;
            case 15:
                get_all_moves_15_16(current_cell, mr, moves, move_length_limit);
                break;
            case 16:
                get_all_moves_16_17(current_cell, mr, moves, move_length_limit);
                break;
            case 17:
                get_all_moves_17_12(current_cell, mr, moves, move_length_limit);
                break;
            case 18:
                get_all_moves_18_19(current_cell, mr, moves, move_length_limit);
                break;
            case 19:
                get_all_moves_19_20(current_cell, mr, moves, move_length_limit);
                break;
            case 20:
                get_all_moves_20_22(current_cell, mr, moves, move_length_limit);
                get_all_moves_20_24(current_cell, mr, moves, move_length_limit);
                break;
            case 21:
                get_all_moves_21_27(current_cell, mr, moves, move_length_limit);
                break;
            case 22:
                get_all_moves_22_23(current_cell, mr, moves, move_length_limit);
                break;
            case 23:
                get_all_moves_23_21(current_cell, mr, moves, move_length_limit);
                break;
            case 24:
                get_all_moves_24_25(current_cell, mr, moves, move_length_limit);
                break;
            case 25:
                get_all_moves_25_26(current_cell, mr, moves, move_length_limit);
                break;
            case 26:
                get_all_moves_26_21(current_cell, mr, moves, move_length_limit);
                break;
            case 27:
                get_all_moves_27_28(current_cell, mr, moves, move_length_limit);
                break;
            case 28:
                get_all_moves_28_30(current_cell, mr, moves, move_length_limit);
                get_all_moves_28_32(current_cell, mr, moves, move_length_limit);
                break;
            case 29:
                get_all_moves_29_36(current_cell, mr, moves, move_length_limit);
                get_all_moves_29_0(current_cell, mr, moves, move_length_limit);
                break;
            case 30:
                get_all_moves_30_31(current_cell, mr, moves, move_length_limit);
                break;
            case 31:
                get_all_moves_31_29(current_cell, mr, moves, move_length_limit);
                break;
            case 32:
                get_all_moves_32_33(current_cell, mr, moves, move_length_limit);
                break;
            case 33:
                get_all_moves_33_34(current_cell, mr, moves, move_length_limit);
                break;
            case 34:
                get_all_moves_34_29(current_cell, mr, moves, move_length_limit);
                break;
            case 35:
                get_all_moves_35_0(current_cell, mr, moves, move_length_limit);
                get_all_moves_35_36(current_cell, mr, moves, move_length_limit);
                break;
            case 36:
                break;
            default:
                break;
        }
    }

    bool game_state::apply_any_move(resettable_bitarray_stack&){
        switch(current_state){
            case 1:
                if(apply_any_move_1_2(current_cell)){
                    return true;
                }
                return false;
            case 2:
                if(apply_any_move_2_3(current_cell)){
                    return true;
                }
                return false;
            case 3:
                if(apply_any_move_3_5(current_cell)){
                    return true;
                }
                if(apply_any_move_3_7(current_cell)){
                    return true;
                }
                return false;
            case 8:
                if(apply_any_move_8_9(current_cell)){
                    return true;
                }
                return false;
            case 10:
                if(apply_any_move_10_11(current_cell)){
                    return true;
                }
                return false;
            case 11:
                if(apply_any_move_11_13(current_cell)){
                    return true;
                }
                if(apply_any_move_11_15(current_cell)){
                    return true;
                }
                return false;
            case 16:
                if(apply_any_move_16_17(current_cell)){
                    return true;
                }
                return false;
            case 18:
                if(apply_any_move_18_19(current_cell)){
                    return true;
                }
                return false;
            case 19:
                if(apply_any_move_19_20(current_cell)){
                    return true;
                }
                return false;
            case 20:
                if(apply_any_move_20_22(current_cell)){
                    return true;
                }
                if(apply_any_move_20_24(current_cell)){
                    return true;
                }
                return false;
            case 25:
                if(apply_any_move_25_26(current_cell)){
                    return true;
                }
                return false;
            case 27:
                if(apply_any_move_27_28(current_cell)){
                    return true;
                }
                return false;
            case 28:
                if(apply_any_move_28_30(current_cell)){
                    return true;
                }
                if(apply_any_move_28_32(current_cell)){
                    return true;
                }
                return false;
            case 29:
                if(apply_any_move_29_36(current_cell)){
                    return true;
                }
                if(apply_any_move_29_0(current_cell)){
                    return true;
                }
                return false;
            case 33:
                if(apply_any_move_33_34(current_cell)){
                    return true;
                }
                return false;
            case 35:
                if(apply_any_move_35_0(current_cell)){
                    return true;
                }
                if(apply_any_move_35_36(current_cell)){
                    return true;
                }
                return false;
            default:
                return false;
        }
    }

    void game_state::revert(const revert_information& ri){
        current_cell = ri.previous_cell;
        current_player = ri.previous_player;
        current_state = ri.previous_state;
        for(unsigned int i=ri.brr.size();i>0;--i){
            pieces[ri.brr[i-1].cell] = ri.brr[i-1].previous_piece;
        }
        for(unsigned int i=ri.vrr.size();i>0;--i){
            variables[ri.vrr[i-1].variable] = ri.vrr[i-1].previous_value;
        }
    }

    void game_state::apply_action_with_revert(const action_representation& action, revert_information& ri){
        switch(action.index){
            case 1:
                current_player = 2;
                break;
            case 7:
                ri.brr.emplace_back(pieces[action.cell], action.cell);
                pieces[action.cell] = 1;
                break;
            case 13:
                current_player = 0;
                break;
            case 14:
                ri.vrr.emplace_back(variables[1], 1);
                variables[1] = 100;
                break;
            case 15:
                ri.vrr.emplace_back(variables[0], 0);
                variables[0] = 0;
                break;
            case 21:
                current_player = 1;
                break;
            case 17:
                ri.brr.emplace_back(pieces[action.cell], action.cell);
                pieces[action.cell] = 2;
                break;
            case 19:
                current_player = 0;
                break;
            case 27:
                ri.brr.emplace_back(pieces[action.cell], action.cell);
                pieces[action.cell] = 1;
                break;
            case 33:
                current_player = 0;
                break;
            case 34:
                ri.vrr.emplace_back(variables[0], 0);
                variables[0] = 100;
                break;
            case 35:
                ri.vrr.emplace_back(variables[1], 1);
                variables[1] = 0;
                break;
            case 37:
                ri.brr.emplace_back(pieces[action.cell], action.cell);
                pieces[action.cell] = 0;
                break;
            case 39:
                current_player = 0;
                break;
            default:
                break;
        }
    }

    void game_state::apply_action(const action_representation& action){
        switch(action.index){
            case 1:
                current_player = 2;
                current_cell = action.cell;
                current_state = 1;
                break;
            case 7:
                pieces[action.cell] = 1;
                break;
            case 13:
                current_player = 0;
                current_cell = action.cell;
                current_state = 10;
                break;
            case 14:
                variables[1] = 100;
                break;
            case 15:
                variables[0] = 0;
                break;
            case 21:
                current_player = 1;
                current_cell = action.cell;
                current_state = 18;
                break;
            case 17:
                pieces[action.cell] = 2;
                break;
            case 19:
                current_player = 0;
                current_cell = action.cell;
                current_state = 16;
                break;
            case 27:
                pieces[action.cell] = 1;
                break;
            case 33:
                current_player = 0;
                current_cell = action.cell;
                current_state = 27;
                break;
            case 34:
                variables[0] = 100;
                break;
            case 35:
                variables[1] = 0;
                break;
            case 37:
                pieces[action.cell] = 0;
                break;
            case 39:
                current_player = 0;
                current_cell = action.cell;
                current_state = 33;
                break;
            default:
                break;
        }
    }

    bool game_state::apply_any_move_1_2([[maybe_unused]] int cell){
        for(int el=1;el<65;++el){
            if(apply_any_move_2_3(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_2_3([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return false;
        }
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 1;
        cell = cell_neighbors[cell][3];
        if(cell == 0){
            pieces[board_change0_cell] = board_change0_piece;
            return false;
        }
        if(apply_any_move_3_5(cell)){
            return true;
        }
        if(apply_any_move_3_7(cell)){
            return true;
        }
        pieces[board_change0_cell] = board_change0_piece;
        return false;
    }

    bool game_state::apply_any_move_3_5([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 10;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_3_7([[maybe_unused]] int cell){
        for(const auto el: shift_table1[cell]){
            if(apply_any_move_8_9(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_8_9([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 2:
            case -1:
                return false;
            default:
                break;
        }
        current_player = 0;
        current_state = 10;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_10_11([[maybe_unused]] int cell){
        [[maybe_unused]] int variable_change0 = variables[1];
        variables[1] = 100;
        [[maybe_unused]] int variable_change1 = variables[0];
        variables[0] = 0;
        if(apply_any_move_11_13(cell)){
            return true;
        }
        if(apply_any_move_11_15(cell)){
            return true;
        }
        variables[0] = variable_change1;
        variables[1] = variable_change0;
        return false;
    }

    bool game_state::apply_any_move_11_13([[maybe_unused]] int cell){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                return false;
            default:
                break;
        }
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 2;
        current_player = 1;
        current_state = 18;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_11_15([[maybe_unused]] int cell){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 16;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_16_17([[maybe_unused]] int cell){
        return false;
        current_player = 1;
        current_state = 18;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_18_19([[maybe_unused]] int cell){
        for(int el=1;el<65;++el){
            if(apply_any_move_19_20(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_19_20([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return false;
        }
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 1;
        cell = cell_neighbors[cell][0];
        if(cell == 0){
            pieces[board_change0_cell] = board_change0_piece;
            return false;
        }
        if(apply_any_move_20_22(cell)){
            return true;
        }
        if(apply_any_move_20_24(cell)){
            return true;
        }
        pieces[board_change0_cell] = board_change0_piece;
        return false;
    }

    bool game_state::apply_any_move_20_22([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 27;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_20_24([[maybe_unused]] int cell){
        for(const auto el: shift_table1[cell]){
            if(apply_any_move_25_26(el)){
                return true;
            }
        }
        return false;
    }

    bool game_state::apply_any_move_25_26([[maybe_unused]] int cell){
        switch(pieces[cell]){
            case 0:
            case -1:
                return false;
            default:
                break;
        }
        current_player = 0;
        current_state = 27;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_27_28([[maybe_unused]] int cell){
        [[maybe_unused]] int variable_change0 = variables[0];
        variables[0] = 100;
        [[maybe_unused]] int variable_change1 = variables[1];
        variables[1] = 0;
        if(apply_any_move_28_30(cell)){
            return true;
        }
        if(apply_any_move_28_32(cell)){
            return true;
        }
        variables[1] = variable_change1;
        variables[0] = variable_change0;
        return false;
    }

    bool game_state::apply_any_move_28_30([[maybe_unused]] int cell){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                return false;
            default:
                break;
        }
        [[maybe_unused]] int board_change0_cell = cell;
        [[maybe_unused]] int board_change0_piece = pieces[cell];
        pieces[cell] = 0;
        if(apply_any_move_29_36(cell)){
            return true;
        }
        if(apply_any_move_29_0(cell)){
            return true;
        }
        pieces[board_change0_cell] = board_change0_piece;
        return false;
    }

    bool game_state::apply_any_move_28_32([[maybe_unused]] int cell){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                break;
            default:
                return false;
        }
        current_player = 0;
        current_state = 33;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_29_36([[maybe_unused]] int cell){
        return false;
    }

    bool game_state::apply_any_move_29_0([[maybe_unused]] int cell){
        current_player = 2;
        current_state = 1;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_33_34([[maybe_unused]] int cell){
        return false;
        if(apply_any_move_29_36(cell)){
            return true;
        }
        if(apply_any_move_29_0(cell)){
            return true;
        }
        return false;
    }

    bool game_state::apply_any_move_35_0([[maybe_unused]] int cell){
        current_player = 2;
        current_state = 1;
        current_cell = cell;
        return true;
    }

    bool game_state::apply_any_move_35_36([[maybe_unused]] int cell){
        return false;
    }

    void game_state::get_all_moves_0_1([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(1,cell);
        moves.emplace_back(mr, cell, 1);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_1_2([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(int el=1;el<65;++el){
            get_all_moves_2_3(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_2_3([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 2:
                break;
            default:
                return;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(7,cell);
        pieces[cell] = 1;
        cell = cell_neighbors[cell][3];
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 3);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(cell == 0){
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        get_all_moves_3_5(cell, mr, moves, move_length_limit);
        get_all_moves_3_7(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_3_5([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_3_7([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(const auto el: shift_table1[cell]){
            get_all_moves_8_9(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_4_10([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_5_6([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_6_4([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_7_8([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(const auto el: shift_table1[cell]){
            get_all_moves_8_9(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_8_9([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 2:
            case -1:
                return;
            default:
                break;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_9_4([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(13,cell);
        moves.emplace_back(mr, cell, 10);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_10_11([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        int variable_change0 = variables[1];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(14,cell);
        variables[1] = 100;
        int variable_change1 = variables[0];
        mr.emplace_back(15,cell);
        variables[0] = 0;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 11);
            mr.resize(previous_changes_list);
            variables[0] = variable_change1;
            variables[1] = variable_change0;
            return;
        }
        get_all_moves_11_13(cell, mr, moves, move_length_limit);
        get_all_moves_11_15(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        variables[0] = variable_change1;
        variables[1] = variable_change0;
        return;
    }

    void game_state::get_all_moves_11_13([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                return;
            default:
                break;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(17,cell);
        pieces[cell] = 2;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 14);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 12);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_11_15([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(19,cell);
        moves.emplace_back(mr, cell, 16);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_12_18([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_13_14([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                return;
            default:
                break;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(17,cell);
        pieces[cell] = 2;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 14);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 12);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_14_12([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_15_16([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(19,cell);
        moves.emplace_back(mr, cell, 16);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_16_17([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        return;
        const auto previous_changes_list = mr.size();
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_17_12([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(21,cell);
        moves.emplace_back(mr, cell, 18);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_18_19([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(int el=1;el<65;++el){
            get_all_moves_19_20(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_19_20([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 0:
                break;
            default:
                return;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(27,cell);
        pieces[cell] = 1;
        cell = cell_neighbors[cell][0];
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 20);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(cell == 0){
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        get_all_moves_20_22(cell, mr, moves, move_length_limit);
        get_all_moves_20_24(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_20_22([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_20_24([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(const auto el: shift_table1[cell]){
            get_all_moves_25_26(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_21_27([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_22_23([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 1:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_23_21([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_24_25([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        for(const auto el: shift_table1[cell]){
            get_all_moves_25_26(el, mr, moves, move_length_limit);
        }
        return;
    }

    void game_state::get_all_moves_25_26([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(pieces[cell]){
            case 0:
            case -1:
                return;
            default:
                break;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_26_21([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(33,cell);
        moves.emplace_back(mr, cell, 27);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_27_28([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        int variable_change0 = variables[0];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(34,cell);
        variables[0] = 100;
        int variable_change1 = variables[1];
        mr.emplace_back(35,cell);
        variables[1] = 0;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 28);
            mr.resize(previous_changes_list);
            variables[1] = variable_change1;
            variables[0] = variable_change0;
            return;
        }
        get_all_moves_28_30(cell, mr, moves, move_length_limit);
        get_all_moves_28_32(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        variables[1] = variable_change1;
        variables[0] = variable_change0;
        return;
    }

    void game_state::get_all_moves_28_30([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                return;
            default:
                break;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(37,cell);
        pieces[cell] = 0;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 31);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 29);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        get_all_moves_29_36(cell, mr, moves, move_length_limit);
        get_all_moves_29_0(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_28_32([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(39,cell);
        moves.emplace_back(mr, cell, 33);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_29_36([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        return;
    }

    void game_state::get_all_moves_29_0([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(1,cell);
        moves.emplace_back(mr, cell, 1);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_30_31([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                return;
            default:
                break;
        }
        int board_change0_cell = cell;
        int board_change0_piece = pieces[cell];
        const auto previous_changes_list = mr.size();
        mr.emplace_back(37,cell);
        pieces[cell] = 0;
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 31);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        if(mr.size()>=move_length_limit){
            moves.emplace_back(mr, cell, 29);
            mr.resize(previous_changes_list);
            pieces[board_change0_cell] = board_change0_piece;
            return;
        }
        get_all_moves_29_36(cell, mr, moves, move_length_limit);
        get_all_moves_29_0(cell, mr, moves, move_length_limit);
        mr.resize(previous_changes_list);
        pieces[board_change0_cell] = board_change0_piece;
        return;
    }

    void game_state::get_all_moves_31_29([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        get_all_moves_29_36(cell, mr, moves, move_length_limit);
        get_all_moves_29_0(cell, mr, moves, move_length_limit);
        return;
    }

    void game_state::get_all_moves_32_33([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        switch(cell){
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
            case 63:
            case 64:
                break;
            default:
                return;
        }
        const auto previous_changes_list = mr.size();
        mr.emplace_back(39,cell);
        moves.emplace_back(mr, cell, 33);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_33_34([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        return;
        get_all_moves_29_36(cell, mr, moves, move_length_limit);
        get_all_moves_29_0(cell, mr, moves, move_length_limit);
        return;
    }

    void game_state::get_all_moves_34_29([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        get_all_moves_29_36(cell, mr, moves, move_length_limit);
        get_all_moves_29_0(cell, mr, moves, move_length_limit);
        return;
    }

    void game_state::get_all_moves_35_0([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        const auto previous_changes_list = mr.size();
        mr.emplace_back(1,cell);
        moves.emplace_back(mr, cell, 1);
        mr.resize(previous_changes_list);
        return;
    }

    void game_state::get_all_moves_35_36([[maybe_unused]] int cell, [[maybe_unused]] move_representation& mr, [[maybe_unused]] std::vector<semimove>& moves, [[maybe_unused]] unsigned int move_length_limit){
        return;
    }

}
