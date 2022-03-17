#ifndef				__HPP_GAME__
#define				__HPP_GAME__

#include "Board.hpp"
#include "Direction.hpp"
#include "Ship.hpp"
#include <vector>

class Game {
	public:
		Game();
		void hello();
		void placeShips();
		void default_board(Board& b);
		bool is_hidden(int cell);
		void placeShipsPC();
		void beginGame();
		void run();
		void humanTurn();
		void computerTurn();
		Direction num_to_direction(int num);
		void sleep();
		bool is_hit_or_miss(Board& b, int row, int col);
		bool place(const int& x, const int& y, Direction d, Ship& ship, Board& b);

	private:
		Board player_board;
		Board cpu_board;

		Ship* cpu_carrier;
		Ship* cpu_battleship;
		Ship* cpu_destroyer;
		Ship* cpu_submarine;
		Ship* cpu_patrolboat;

		Ship* player_carrier;
		Ship* player_battleship;
		Ship* player_destroyer;
		Ship* player_submarine;
		Ship* player_patrolboat;

		std::vector<Ship*> cpu_ships;
		std::vector<Ship*> player_ships;
		int player_score;
		int cpu_score;
};

#endif
