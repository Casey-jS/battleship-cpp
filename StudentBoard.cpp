#include "Board.hpp"
#include "BoardValues.hpp"

#include <algorithm>
#include <iostream>
#include <string>

// create board
Board::Board(){
    int *grid = new int[WIDTH*HEIGHT];
    this->grid = grid;
}

// Copy board
Board::Board(const Board& other){
    this->grid = new int[100];

    for (int i = 0; i < 100; i++){
        this->grid[i] = other.grid[i];
    } 
}

// Destructor used to free all memory
Board::~Board(){
    delete[]this->grid;
}

int& Board::Internal::operator[](int index){
    if (index >= WIDTH){
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid width of " + std::to_string(WIDTH));
    }
    return _grid[index];
}

Board::Internal Board::operator[](int index){
    if (index >= HEIGHT){
        throw std::out_of_range(std::to_string(index) + " is greater value than or equal to grid width of " + std::to_string(HEIGHT));
    }
    return Board::Internal(grid+(index * WIDTH));
}

std::ostream& operator<<(std::ostream& out, Board& b){

    out << '\t';
    // output top row of indices
    for (int i = 0; i < WIDTH; i++){
        out << i << '\t';
    }
    out << '\n';

    // output each value individually
    for (int row = 0; row < 10; row++){
        out << row << '\t';
        for (int col = 0; col < 10; col++){
            out << char(b[row][col]) << '\t';
        }
        out << "\n\n";
    }
    return out; 
}