#include "Board.hpp"
#include "Direction.hpp"
#include "Game.hpp"
#include "Ship.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <ctime>
#include <thread>
#include <chrono>

#define MAX_SCORE 17

using namespace std;

/**
 * Constructor will create the ships vector and add ships to it.
 */
Game::Game(){

	Ship* cpu_carrier = new Ship(5, "Carrier", 'c');
	Ship* cpu_battleship = new Ship(4, "Battleship", 'b');
	Ship* cpu_destroyer = new Ship(3, "Destroyer", 'd');
	Ship* cpu_submarine = new Ship(3, "Submarine", 's');
	Ship* cpu_patrolboat = new Ship(2, "Patrol Boat", 'p');

	Ship* player_carrier = new Ship(5, "Carrier", 'c');
	Ship* player_battleship = new Ship(4, "Battleship", 'b');
	Ship* player_destroyer = new Ship(3, "Destroyer", 'd');
	Ship* player_submarine = new Ship(3, "Submarine", 's');
	Ship* player_patrolboat = new Ship(2, "Patrol Boat", 'p');

	this->cpu_ships = {cpu_carrier, cpu_battleship, cpu_destroyer, cpu_submarine, cpu_patrolboat};
    this->player_ships = {player_carrier, player_battleship, player_destroyer, player_submarine, player_patrolboat};
	
	auto player_board = new Board();
	auto cpu_board = new Board();
	player_score = 0;
	cpu_score = 0;
	
}

/**
 * Begin Game let's user and then computer setup boards then calls run()
 */
void Game::beginGame(){
	cout << "----------------BATTLESHIP----------------\n";
	cout << "The boats are:\n\n";
    
	for (Ship* ship : this->player_ships){
		cout << *ship << endl;
	}

	placeShips();
	placeShipsPC();
	run();
}

// quickly populate board with ships (for testing)
void Game::default_board(Board& b){
	int row = 0;
	for (Ship* ship : player_ships){
		place(row, 0, VERTICAL, *ship, b);
		row++;
	}
}
/*
Called when the game displays information the player
will want to know, pauses output stream so user can read
*/
void Game::sleep(){
	std::this_thread::sleep_for(std::chrono::seconds(1));
}
/**
 * Handle the human placing ships.
 */
void Game::placeShips(){

	std::cout << "Your turn to place your ships!\n";
	
	bool default_set = false; //for testing

	for (Ship* ship : this->player_ships){
		int x, y, direction;
		if (default_set){
			break;
		}
		
		bool valid = false;
		while(!valid){
			cout << endl << player_board << endl;
			std::cout << "What indices to place your " << ship->getName() << "?\n";
			std::cin >> x;

			// input 99 when asked for index to quickly populate board
			if (x == 99){
				default_board(player_board);
				default_set = true;
				valid = true;
				break;
			}
			std::cin >> y;
			std::cout << "What direction? (0 for horizontal, 1 for vertical)\n";
			std::cin >> direction;

			// convert int input to direction
			Direction d = num_to_direction(direction);

			if (place(x, y, d, *ship, player_board)){
				valid = true;
			}
			else{
				cout << "Invalid placement, try again\n";
			}
		}
	} 
}

/**
 * Helper method that converts user inputs into direction 
 */
Direction Game::num_to_direction(int num){
	if (num == 0) return VERTICAL;
	return HORIZONTAL;
}

/**
 * Handle the computer placing ships.
 */
void Game::placeShipsPC(){

	for (Ship* ship : cpu_ships){
		bool valid = false;
		while(!valid){
			// set the seed to get to as close to truly random as possible
			srand(time(0));
			int x = rand() % 9;
			int y = rand() % 9;
			int direction = rand() % 2;
			Direction d = num_to_direction(direction);
			
			if (place(x, y, d, *ship, cpu_board)){
				valid = true;
				cout << "CPU placed their " << ship->getName() << endl;
			}
		}
	}
	
}


/**
 * Helper method that checks if it is safe to put a ship
 * at a particular spot with a particular direction.
 */
bool Game::place(const int& x, const int& y, Direction d, Ship& ship, Board& b){
	
	// first, make sure the x and y values are valid
	if (y < 0 || y > 9 || x < 0 || x > 9){ return false; }

	switch(d){
		case VERTICAL:
			// return false if the y value + the size of the ship is greater than 9
			if (y + ship.getSpaces() > 9){ return false; }

			// return false if any value between y and y + size of ship is already occupied
			for (int row = y; row < y + ship.getSpaces(); row++){
				if (b[x][row] != 0){ return false; }
			}

			// if nothing is caught, modify the board
			for (int row = y; row <  y + ship.getSpaces(); row++){
				b[x][row] = ship.getChr();
			}
			return true;

		case HORIZONTAL:
			if (x + ship.getSpaces() > 9){ return false; }

			for (int col = x; col < (x + ship.getSpaces()); col++){
				if (b[col][y] != 0){ return false; }
			}

			for (int col = x; col < x + ship.getSpaces(); col++){
				b[col][y] = ship.getChr();
			}
			return true;
	}
	return true;
}

/**
 * Call human turn/computer turn until someone wins.
 */
void Game::run(){

	bool game_over = false;

	while(!game_over){

		humanTurn();
		if (player_score == MAX_SCORE){
			cout << "You win!\n";
			game_over = true;
		}
		computerTurn();
		if (cpu_score == MAX_SCORE){
			cout << "CPU wins!\n";
			game_over = true;
		}
	}
}

void Game::humanTurn(){
	// what the user will see
	Board display;

	cout << "Your board:\n" << player_board << endl;

	// occupy the 2D array, only showing past hits
	for (int row = 0; row < HEIGHT; row++){
		for (int col = 0; col < WIDTH; col++){
			if (cpu_board[row][col] == '-'){
				display[row][col] = '-';
			}
			else if(cpu_board[row][col] == '*'){
				display[row][col] = '*';
			}
			else{
				display[row][col] = 0;
			}	
		}
	}
	cout << "CPU Board:\n";
	cout << display << endl;

	cout << "SCORE:\n";
	cout << "You: " << this->player_score << endl;
	cout << "CPU: " << this->cpu_score << endl;

	bool valid = false;
	int x, y;

	while(!valid){
		cout << "Where would you like to guess?\n";
		cin >> x;
		cin >> y;
		if (cpu_board[x][y] == 0){
			cout << "Miss!\n";
			sleep();
			cin.get();
			cpu_board[x][y] = '-';
			valid = true;
		}
		// if user hits anything except empty or previously hit spots, count as hit
		else if (cpu_board[x][y] != '-' && cpu_board[x][y] != '*'){
			valid = true;
			for (Ship* s : cpu_ships){
				if (s->getChr() == cpu_board[x][y]){
					cout << "You hit a " << s->getName() << "!\n";
					sleep();
					valid = true;
					s->addHit();
					cpu_board[x][y] = '*';
					player_score++;
				}
			}
		}
		else{
			cout << "Invalid selection, try again\n";
			sleep();
		}
	}
}

void Game::computerTurn(){
	
	// setting seed to generate random numbers based on the current time
	srand(time(0));

	bool valid = false;
	while (!valid){
		int x = rand() % 9;
		int y = rand() % 9;
		cout << "CPU guessed [" << x << "][" << y << "]!\n"; 
		sleep();

		if (player_board[x][y] == 0){
			valid = true;
			cout << "CPU Missed!\n";
			sleep();
			player_board[x][y] = '-';
		}
		for (Ship* s : player_ships){
			if (s->getChr() == player_board[x][y]){
				cout << "CPU hit your " << s->getName() << "!\n";
				sleep();
				s->addHit();
				player_board[x][y] = '*';
				cpu_score += 1;
				valid = true;
			}
		}
	}

}

/**
 * Create a game instance and start.
 */
int main(int argc, char** argv){
	(void)argc;
	(void)argv;
	Game g;
	g.beginGame();

	return 0;
}