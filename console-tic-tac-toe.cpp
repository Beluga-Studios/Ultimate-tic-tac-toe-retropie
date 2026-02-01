// normal tic tac toe in the console
#include <iostream>
using namespace std;

enum Player {EMPTY = 0, X, O};

Player board[9];
Player playerTurn

// Turn to printable char
char PlayerToChar(Player p) {
    if (p == X) {return 'X';}
    if (p == O) {return 'O';}
    return '.';
}

// function made by AI
void drawBoard() {
    for (int i = 0; i < 9; i++) {
        cout << PlayerToChar(board[i]) << " ";
        if (i % 3 == 2) cout << endl;
    }
    cout << endl;
}

int main() {
    drawBoard();
    cout << "Player " << PlayerToChar()
    return 0;
}