#ifndef RBG_GAME
#define RBG_GAME

#include <boost/container/static_vector.hpp>
#include <string>
#include <vector>

namespace rbg_game{
    constexpr int BOARD_SIZE = 64;
    constexpr int NUMBER_OF_PLAYERS = 3;
    constexpr int NUMBER_OF_PIECES = 3;
    constexpr int NUMBER_OF_VARIABLES = 2;
    constexpr int BOARD_DEGREE = 4;

    std::string board_cell_to_string(int cell_id);
    std::string piece_to_string(int piece_id);
    std::string variable_to_string(int variable_id);
    constexpr int NUMBER_OF_MODIFIERS = 14;
    int action_to_modifier_index(int action_index);
    int modifier_to_action_index(int modifier_index);

    int get_neighbor(int cell_id, int edge_id);
    int get_bound(int variable_id);

    class resettable_bitarray_stack{};
    struct action_representation{
        int index;
        int cell;
        action_representation(void)=default;
        action_representation(int index,int cell)
        : index(index),
          cell(cell){
        }
        bool operator==(const action_representation& rhs) const;
    };

    struct board_revert_information{
        int previous_piece;
        int cell;
        board_revert_information(void)=default;
        board_revert_information(int previous_piece, int cell)
        : previous_piece(previous_piece),
          cell(cell){
        }
    };
    struct variable_revert_information{
        int previous_value;
        int variable;
        variable_revert_information(void)=default;
        variable_revert_information(int previous_value, int variable)
        : previous_value(previous_value),
          variable(variable){
        }
    };

    typedef boost::container::static_vector<action_representation, 4> move_representation;
    typedef boost::container::static_vector<board_revert_information, 4> board_revert_representation;
    typedef boost::container::static_vector<variable_revert_information, 4> variables_revert_representation;
    struct move{
        move_representation mr;
        move(void) = default;
        move(const move_representation& mr);
        bool operator==(const move& rhs) const;
    };
    class semimove{
        friend class game_state;
        private:
            move_representation mr;
            int cell;
            int state;
            semimove(void)=default;
        public:
            semimove(const move_representation& mr, int cell, int state);
            const move_representation& get_actions(void)const;
            bool operator==(const semimove& rhs) const;
    };
    class revert_information{
        int previous_cell = 1;
        int previous_player = 0;
        int previous_state = 35;
        board_revert_representation brr = {};
        variables_revert_representation vrr = {};
        friend class game_state;
    };

    class game_state{
        public:
            int get_current_cell(void)const;
            int get_piece(int cell_id)const;
            int get_variable_value(int variable_id)const;
            int get_current_player(void)const;
            int get_player_score(int player_id)const;
            bool is_nodal(void)const;
            revert_information apply_semimove_with_revert(const semimove& m);
            void apply_semimove(const semimove& m);
            void apply_move(const move& m);
            std::vector<move> get_all_moves(resettable_bitarray_stack& cache);
            void get_all_moves(resettable_bitarray_stack& cache, std::vector<move>& moves);
            void get_all_semimoves(resettable_bitarray_stack&, std::vector<semimove>& moves, unsigned int move_length_limit);
            bool apply_any_move(resettable_bitarray_stack&);
            void revert(const revert_information& ri);
        private:
            void apply_action_with_revert(const action_representation& action, revert_information& ri);
            void apply_action(const action_representation& action);
            bool apply_any_move_1_2(int cell);
            bool apply_any_move_2_3(int cell);
            bool apply_any_move_3_5(int cell);
            bool apply_any_move_3_7(int cell);
            bool apply_any_move_8_9(int cell);
            bool apply_any_move_10_11(int cell);
            bool apply_any_move_11_13(int cell);
            bool apply_any_move_11_15(int cell);
            bool apply_any_move_16_17(int cell);
            bool apply_any_move_18_19(int cell);
            bool apply_any_move_19_20(int cell);
            bool apply_any_move_20_22(int cell);
            bool apply_any_move_20_24(int cell);
            bool apply_any_move_25_26(int cell);
            bool apply_any_move_27_28(int cell);
            bool apply_any_move_28_30(int cell);
            bool apply_any_move_28_32(int cell);
            bool apply_any_move_29_36(int cell);
            bool apply_any_move_29_0(int cell);
            bool apply_any_move_33_34(int cell);
            bool apply_any_move_35_0(int cell);
            bool apply_any_move_35_36(int cell);
            void get_all_moves_0_1(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_1_2(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_2_3(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_3_5(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_3_7(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_4_10(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_5_6(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_6_4(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_7_8(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_8_9(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_9_4(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_10_11(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_11_13(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_11_15(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_12_18(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_13_14(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_14_12(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_15_16(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_16_17(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_17_12(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_18_19(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_19_20(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_20_22(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_20_24(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_21_27(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_22_23(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_23_21(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_24_25(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_25_26(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_26_21(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_27_28(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_28_30(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_28_32(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_29_36(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_29_0(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_30_31(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_31_29(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_32_33(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_33_34(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_34_29(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_35_0(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);
            void get_all_moves_35_36(int cell, move_representation& mr, std::vector<semimove>& moves, unsigned int move_length_limit);

            int current_cell = 1;
            int current_player = 0;
            int current_state = 35;
            int pieces[65] = {
                -1,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                1,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
                2,
            };
            int variables[2] = {};
    };

}

#endif
