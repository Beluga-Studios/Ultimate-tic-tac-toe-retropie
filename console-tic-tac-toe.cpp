// normal tic tac toe in the console
#include <iostream>
using namespace std;

enum Player {EMPTY = 0, X, O};

Player board[9];

// Turn to printable char
char cellToChar(Player p) {
    if (p == X) {return 'X';}
    if (p == O) {return 'O';}
    return '.';
}

// function made by AI
void drawBoard() {
    for (int i = 0; i < 9; i++) {
        cout << cellToChar(board[i]) << " ";
        if (i % 3 == 2) cout << endl;
    }
    cout << endl;
}

int main() {
    drawBoard();
    return 0;
}