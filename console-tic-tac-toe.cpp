// normal tic tac toe in the console
#include <iostream>

enum Cell {EMPTY = 0, X, O};

Cell board[9];

int main() {
    std::cout << board;
    return 0;
}