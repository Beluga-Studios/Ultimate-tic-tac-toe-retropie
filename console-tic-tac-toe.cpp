// normal tic tac toe in the console
#include <iostream>
using namespace std;

enum Player {EMPTY = 0, X, O};

Player board[9];
Player playerTurn = X;

// Turn to printable char
char PlayerToChar(Player p) {
    if (p == X) {return 'X';}
    if (p == O) {return 'O';}
    return '.';
}

void drawBoard() {
    for (int i = 0; i < 9; i++) {
        cout << PlayerToChar(board[i]) << " ";
        if (i % 3 == 2) cout << endl;
    }
    cout << endl;
}

Player checkWin() {
    int wins[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // rows
        {0,3,6}, {1,4,7}, {2,5,8}, // cols
        {0,4,8}, {2,4,6}           // diagonals
    };

}

int main() {
    while (true) {
    drawBoard();
    cout << "Player " << PlayerToChar(playerTurn) << " Turn. \nchoose square(0-8)";
    int move;
    cin >> move;
    board[move] = playerTurn;
    }
    return 0;
}