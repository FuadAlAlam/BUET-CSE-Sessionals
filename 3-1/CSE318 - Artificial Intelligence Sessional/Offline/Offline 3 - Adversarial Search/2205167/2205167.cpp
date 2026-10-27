#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <sstream>

using namespace std;

const int P1_STORE = 6;
const int P2_STORE = 13;
const double INF = 1e9;

class MancalaBoard {
public:
    int board[14];

    MancalaBoard() {
        for (int i = 0; i < 14; ++i) {
            board[i] = 4;
        }
        board[P1_STORE] = 0;
        board[P2_STORE] = 0;
    }

    bool is_valid_move(int player, int pit) const {
        if (player == 1) return pit >= 0 && pit <= 5 && board[pit] > 0;
        return pit >= 7 && pit <= 12 && board[pit] > 0;
    }

    vector<int> get_valid_moves(int player) const {
        vector<int> moves;
        int start = (player == 1) ? 0 : 7;
        int end = (player == 1) ? 5 : 12;
        for (int i = start; i <= end; ++i) {
            if (board[i] > 0) moves.push_back(i);
        }
        return moves;
    }

    bool is_terminal() const {
        bool p1_empty = true, p2_empty = true;
        for (int i = 0; i <= 5; ++i) {
            if (board[i] > 0) {
                p1_empty = false;
                break;
            }
        }
        for (int i = 7; i <= 12; ++i) {
            if (board[i] > 0) {
                p2_empty = false;
                break;
            }
        }
        return p1_empty || p2_empty;
    }

    MancalaBoard make_move(int player, int pit, bool& extra_turn, int& captured) const {
        MancalaBoard next_state = *this;
        int stones = next_state.board[pit];
        next_state.board[pit] = 0;
        int curr = pit;

        int my_store = (player == 1) ? P1_STORE : P2_STORE;
        int opp_store = (player == 1) ? P2_STORE : P1_STORE;

        while (stones > 0) {
            curr = (curr + 1) % 14;
            if (curr == opp_store) continue;
            next_state.board[curr]++;
            stones--;
        }

        extra_turn = (curr == my_store);
        captured = 0;

        bool in_my_pits = (player == 1 && curr >= 0 && curr <= 5) || (player == 2 && curr >= 7 && curr <= 12);
        if (!extra_turn && in_my_pits && next_state.board[curr] == 1) {
            int opp_pit = 12 - curr;
            if (next_state.board[opp_pit] > 0) {
                captured = next_state.board[curr] + next_state.board[opp_pit];
                next_state.board[curr] = 0;
                next_state.board[opp_pit] = 0;
                next_state.board[my_store] += captured;
            }
        }

        if (next_state.is_terminal()) {
            for (int i = 0; i <= 5; ++i) { 
                next_state.board[P1_STORE] += next_state.board[i]; 
                next_state.board[i] = 0; 
            }
            for (int i = 7; i <= 12; ++i) { 
                next_state.board[P2_STORE] += next_state.board[i]; 
                next_state.board[i] = 0; 
            }
        }

        return next_state;
    }
};

class MancalaHeuristics {
public:
    static double evaluate(const MancalaBoard& state, int player, int heuristic_id, bool extra_turn = false, int captured = 0) {
        if (state.is_terminal()) {
            int my_store = (player == 1) ? state.board[P1_STORE] : state.board[P2_STORE];
            int opp_store = (player == 1) ? state.board[P2_STORE] : state.board[P1_STORE];
            if (my_store > opp_store) return 10000.0 + (my_store - opp_store);
            if (my_store < opp_store) return -10000.0 + (my_store - opp_store);
            return 0.0;
        }

        int my_store = (player == 1) ? state.board[P1_STORE] : state.board[P2_STORE];
        int opp_store = (player == 1) ? state.board[P2_STORE] : state.board[P1_STORE];

        int my_side = 0, opp_side = 0;
        int my_start = (player == 1) ? 0 : 7;
        int opp_start = (player == 1) ? 7 : 0;
        for (int i = 0; i <= 5; ++i) {
            my_side += state.board[my_start + i];
            opp_side += state.board[opp_start + i];
        }

        double w1 = 2.0, w2 = 1.0, w3 = 5.0, w4 = 3.0;
        double e_turn = extra_turn ? 1.0 : 0.0;

        switch (heuristic_id) {
            case 1:
                return (double)(my_store - opp_store);
            case 2:
                return w1 * (my_store - opp_store) + w2 * (my_side - opp_side);
            case 3:
                return w1 * (my_store - opp_store) + w2 * (my_side - opp_side) + w3 * e_turn;
            case 4:
                return w1 * (my_store - opp_store) + w2 * (my_side - opp_side) + w3 * e_turn + w4 * captured;
            default:
                return (double)(my_store - opp_store);
        }
    }
};

class AlphaBetaPlayer {
private:
    int player_id;
    int heuristic_id;
    int max_depth;

    vector<int> order_moves(const MancalaBoard& state, const vector<int>& moves, int curr_player) {
        vector<pair<int, int>> scored;
        for (int move : moves) {
            bool extra = false;
            int capt = 0;
            state.make_move(curr_player, move, extra, capt);
            int score = (extra ? 100 : 0) + capt * 10;
            scored.push_back({score, move});
        }
        sort(scored.rbegin(), scored.rend());
        vector<int> ordered;
        for (auto& sm : scored) ordered.push_back(sm.second);
        return ordered;
    }

    double minimax(const MancalaBoard& state, int depth, double alpha, double beta, int curr_player, long long& nodes, long long& cutoffs) {
        nodes++;
        if (depth == 0 || state.is_terminal()) {
            return MancalaHeuristics::evaluate(state, player_id, heuristic_id);
        }

        vector<int> valid_moves = state.get_valid_moves(curr_player);
        if (valid_moves.empty()) return MancalaHeuristics::evaluate(state, player_id, heuristic_id);

        vector<int> ordered_moves = order_moves(state, valid_moves, curr_player);
        bool is_max = (curr_player == player_id);
        double best_val = is_max ? -INF : INF;

        for (int move : ordered_moves) {
            bool extra = false;
            int capt = 0;
            MancalaBoard next_state = state.make_move(curr_player, move, extra, capt);

            int next_player = extra ? curr_player : ((curr_player == 1) ? 2 : 1);
            int next_depth = extra ? depth : (depth - 1);
            double val = minimax(next_state, next_depth, alpha, beta, next_player, nodes, cutoffs);

            if (is_max) {
                best_val = max(best_val, val);
                alpha = max(alpha, best_val);
            } else {
                best_val = min(best_val, val);
                beta = min(beta, best_val);
            }

            if (beta <= alpha) {
                cutoffs++;
                break;
            }
        }
        return best_val;
    }

public:
    AlphaBetaPlayer(int pid, int hid, int max_d) : player_id(pid), heuristic_id(hid), max_depth(max_d) {}

    int select_move(const MancalaBoard& state, long long& total_nodes, long long& total_cutoffs) {
        int best_move = -1;
        for (int d = 1; d <= max_depth; ++d) {
            vector<int> valid_moves = state.get_valid_moves(player_id);
            if (valid_moves.empty()) break;

            vector<int> ordered_moves = order_moves(state, valid_moves, player_id);
            double best_val = -INF;
            int curr_best = ordered_moves[0];

            for (int move : ordered_moves) {
                bool extra = false;
                int capt = 0;
                MancalaBoard next_state = state.make_move(player_id, move, extra, capt);

                int next_player = extra ? player_id : ((player_id == 1) ? 2 : 1);
                double val;
                if (next_state.is_terminal()) {
                    val = MancalaHeuristics::evaluate(next_state, player_id, heuristic_id, extra, capt);
                } else {
                    int next_depth = extra ? (d) : (d - 1);
                    val = minimax(next_state, next_depth, -INF, INF, next_player, total_nodes, total_cutoffs);
                }

                if (val > best_val) {
                    best_val = val;
                    curr_best = move;
                }
            }
            best_move = curr_best;
        }
        return best_move;
    }
};

struct GameResult {
    int winner;
    int p1_score;
    int p2_score;
    long long p1_nodes;
    long long p2_nodes;
    long long p1_cutoffs;
    long long p2_cutoffs;
};

GameResult run_deterministic_game(int h1, int h2, int depth) {
    MancalaBoard board;
    int current_player = 1;
    AlphaBetaPlayer p1(1, h1, depth);
    AlphaBetaPlayer p2(2, h2, depth);
    long long n1 = 0, n2 = 0, c1 = 0, c2 = 0;

    while (!board.is_terminal()) {
        int move = (current_player == 1) ? p1.select_move(board, n1, c1) : p2.select_move(board, n2, c2);
        if (move == -1) break;

        bool extra_turn = false;
        int captured = 0;
        board = board.make_move(current_player, move, extra_turn, captured);

        if (!extra_turn) {
            current_player = (current_player == 1) ? 2 : 1;
        }
    }

    GameResult res;
    res.p1_score = board.board[P1_STORE];
    res.p2_score = board.board[P2_STORE];
    res.p1_nodes = n1;
    res.p2_nodes = n2;
    res.p1_cutoffs = c1;
    res.p2_cutoffs = c2;
    if (res.p1_score > res.p2_score) res.winner = 1;
    else if (res.p2_score > res.p1_score) res.winner = 2;
    else res.winner = 0;

    return res;
}

void run_all_experiments() {
    vector<int> depths = {4, 6, 8};
    vector<int> heuristics = {1, 2, 3, 4};

    cout << "========================================================================================================\n";
    cout << "                            MANCALA ADVERSARIAL SEARCH EXPERIMENT                            \n";
    cout << "========================================================================================================\n";

    for (int depth : depths) {
        cout << "\n========================================================================================================\n";
        cout << "                       EXPERIMENT RESULTS AT SEARCH DEPTH = " << depth << "\n";
        cout << "========================================================================================================\n";
        cout << left << setw(14) << "Matchup" 
             << setw(14) << "P1 (First)" 
             << setw(14) << "P2 (Second)" 
             << setw(12) << "Score" 
             << setw(12) << "Winner" 
             << setw(24) << "Nodes (P1/P2)" 
             << setw(24) << "Cutoffs (P1/P2)" << "\n";
        cout << string(114, '-') << "\n";

        int wins[5] = {0}, losses[5] = {0}, draws[5] = {0}, total_stones[5] = {0};

        for (size_t i = 0; i < heuristics.size(); ++i) {
            for (size_t j = i + 1; j < heuristics.size(); ++j) {
                int hA = heuristics[i];
                int hB = heuristics[j];

                GameResult g1 = run_deterministic_game(hA, hB, depth);
                string score1 = to_string(g1.p1_score) + " - " + to_string(g1.p2_score);
                string winner1 = (g1.winner == 1) ? ("H" + to_string(hA)) : (g1.winner == 2 ? ("H" + to_string(hB)) : "Draw");
                string nodes1 = to_string(g1.p1_nodes) + " / " + to_string(g1.p2_nodes);
                string cutoffs1 = to_string(g1.p1_cutoffs) + " / " + to_string(g1.p2_cutoffs);

                cout << left << setw(14) << ("H" + to_string(hA) + " vs H" + to_string(hB))
                     << setw(14) << ("H" + to_string(hA))
                     << setw(14) << ("H" + to_string(hB))
                     << setw(12) << score1
                     << setw(12) << winner1
                     << setw(24) << nodes1
                     << setw(24) << cutoffs1 << "\n";

                total_stones[hA] += g1.p1_score;
                total_stones[hB] += g1.p2_score;
                if (g1.winner == 1) { wins[hA]++; losses[hB]++; }
                else if (g1.winner == 2) { wins[hB]++; losses[hA]++; }
                else { draws[hA]++; draws[hB]++; }

                GameResult g2 = run_deterministic_game(hB, hA, depth);
                string score2 = to_string(g2.p1_score) + " - " + to_string(g2.p2_score);
                string winner2 = (g2.winner == 1) ? ("H" + to_string(hB)) : (g2.winner == 2 ? ("H" + to_string(hA)) : "Draw");
                string nodes2 = to_string(g2.p1_nodes) + " / " + to_string(g2.p2_nodes);
                string cutoffs2 = to_string(g2.p1_cutoffs) + " / " + to_string(g2.p2_cutoffs);

                cout << left << setw(14) << ("H" + to_string(hB) + " vs H" + to_string(hA))
                     << setw(14) << ("H" + to_string(hB))
                     << setw(14) << ("H" + to_string(hA))
                     << setw(12) << score2
                     << setw(12) << winner2
                     << setw(24) << nodes2
                     << setw(24) << cutoffs2 << "\n";

                total_stones[hB] += g2.p1_score;
                total_stones[hA] += g2.p2_score;
                if (g2.winner == 1) { wins[hB]++; losses[hA]++; }
                else if (g2.winner == 2) { wins[hA]++; losses[hB]++; }
                else { draws[hB]++; draws[hA]++; }
            }
        }

        cout << "\n--- Heuristic Performance Summary (Depth " << depth << ") ---\n";
        cout << left << setw(12) << "Heuristic" 
             << setw(10) << "Wins" 
             << setw(10) << "Losses" 
             << setw(10) << "Draws" 
             << setw(16) << "Win Rate %" 
             << setw(14) << "Total Stones" << "\n";
        cout << string(66, '-') << "\n";

        for (int h = 1; h <= 4; ++h) {
            int total_games = wins[h] + losses[h] + draws[h];
            double win_rate = (total_games > 0) ? (100.0 * wins[h] / total_games) : 0.0;
            stringstream ss;
            ss << fixed << setprecision(1) << win_rate << "%";
            cout << left << setw(12) << ("H" + to_string(h))
                 << setw(10) << wins[h]
                 << setw(10) << losses[h]
                 << setw(10) << draws[h]
                 << setw(16) << ss.str()
                 << setw(14) << total_stones[h] << "\n";
        }
    }

    cout << "\n========================================================================================================\n";
    cout << "                                                DONE                                      \n";
    cout << "========================================================================================================\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    run_all_experiments();

    return 0;
}
