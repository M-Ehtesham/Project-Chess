#include <iostream>
using namespace std;

// Function to initialize the board
void board_startup(char board[][8], int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			board[i][j] = ' ';
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {

			if (i == 1) board[i][j] = 'p';
			if (i == 6) board[i][j] = 'P';

			if (i == 0 && (j == 2 || j == 5)) board[i][j] = 'b';
			if (i == 7 && (j == 2 || j == 5)) board[i][j] = 'B';

			if (i == 0 && (j == 0 || j == 7)) board[i][j] = 'r';
			if (i == 7 && (j == 0 || j == 7)) board[i][j] = 'R';

			if (i == 0 && j == 3) board[i][j] = 'q';
			if (i == 7 && j == 3) board[i][j] = 'Q';

			if (i == 0 && j == 4) board[i][j] = 'k';
			if (i == 7 && j == 4) board[i][j] = 'K';

			if (i == 0 && (j == 1 || j == 6)) board[i][j] = 'n';
			if (i == 7 && (j == 1 || j == 6)) board[i][j] = 'N';

		}
	}
}

// Function to print the board
void printing(char board[][8], int size) {
	int num = 0;
	cout << "   a b c d e f g h\n";
	cout << "   ----------------\n";
	for (int i = 0; i < size; i++) {
		cout << num << "| ";
		for (int j = 0; j < size; j++) {
			cout << board[i][j] << " ";
		}cout << "|";
		cout << endl;
		num++;
	}cout << "   ----------------\n";
}

// Pawn move
bool isValidPawnMove(int sx, int sy, int dx, int dy, char board[8][8], bool whiteTurn) {

	int dir, startrow;
	if (whiteTurn) {
		dir = -1;
		startrow = 6;
	}
	else {
		dir = 1;
		startrow = 1;
	}

	if (dx == sx + dir && dy == sy && board[dx][dy] == ' ')
		return true;
	if (sx == startrow && dx == sx + 2 * dir && dy == sy && board[sx + dir][sy] == ' ' && board[dx][dy] == ' ')
		return true;


	if (dx == sx + dir && (dy == sy + 1 || dy == sy - 1) &&
		board[dx][dy] != ' ')
	{
		if (whiteTurn &&
			board[dx][dy] >= 'a' && board[dx][dy] <= 'z')
			return true;

		if (!whiteTurn &&
			board[dx][dy] >= 'A' && board[dx][dy] <= 'Z')
			return true;
	}

	return false;
}

// Bishop
bool isValidBishopMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	int rowDiff = dx - sx;
	int colDiff = dy - sy;

	// Bishop can only move diagonally
	if (!((rowDiff == colDiff) || (rowDiff == -colDiff))) {
		return false;
	}

	// Determine rowDir manually
	int rowDir;
	if (rowDiff > 0)
		rowDir = 1;
	else
		rowDir = -1;

	// Determine colDir manually
	int colDir;
	if (colDiff > 0)
		colDir = 1;
	else
		colDir = -1;

	// Check if anything is blocking the diagonal
	int x = sx + rowDir;
	int y = sy + colDir;

	while (x != dx && y != dy) {
		if (board[x][y] != ' ')
			return false;

		x += rowDir;
		y += colDir;
	}

	char dest = board[dx][dy];

	// Check capture rules
	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z') return false;  // can't capture own piece
	}
	else {
		if (dest >= 'a' && dest <= 'z') return false;  // can't capture own piece
	}

	return true;
}


// Rook
bool isValidRookMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {

	if (dx != sx && dy != sy)
		return false;

	int rowDir = 0, colDir = 0;

	if (dx > sx) rowDir = 1;
	if (dx < sx) rowDir = -1;
	if (dy > sy) colDir = 1;
	if (dy < sy) colDir = -1;

	int x = sx + rowDir;
	int y = sy + colDir;

	while (x != dx || y != dy) {
		if (board[x][y] != ' ')
			return false;

		x += rowDir;
		y += colDir;
	}

	char dest = board[dx][dy];

	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z')
			return false;
	}
	else {
		if (dest >= 'a' && dest <= 'z')
			return false;
	}

	return true;
}

// Queen
bool isValidQueenMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	bool rookMove = isValidRookMove(sx, sy, dx, dy, board, whiteTurn);
	bool bishopMove = isValidBishopMove(sx, sy, dx, dy, board, whiteTurn);

	if (rookMove || bishopMove)
		return true;

	return false;
}

// King (fixed as per your request)
bool isValidKingMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {

	int row = dx - sx;
	if (row < 0) row = -row;

	int col = dy - sy;
	if (col < 0) col = -col;

	if (row > 1 || col > 1)
		return false;

	if (row == 0 && col == 0)
		return false;

	char dest = board[dx][dy];

	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z') return false;
	}
	else {
		if (dest >= 'a' && dest <= 'z') return false;
	}

	return true;
}

// Knight
bool isValidKnightMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	int row = dx - sx;
	if (row < 0) row = -row;

	int col = dy - sy;
	if (col < 0) col = -col;

	if (!((row == 2 && col == 1) || (row == 1 && col == 2)))
		return false;

	char dest = board[dx][dy];

	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z') return false;
	}
	else {
		if (dest >= 'a' && dest <= 'z') return false;
	}

	return true;
}

// Move piece
void movePiece(int sx, int sy, int dx, int dy, char board[8][8]) {
	board[dx][dy] = board[sx][sy];
	board[sx][sy] = ' ';
}

bool isValidMove(char piece, int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	switch (piece) {
	case 'P':case 'p':
		return isValidPawnMove(sx, sy, dx, dy, board, whiteTurn);
	case 'B':case 'b':
		return isValidBishopMove(sx, sy, dx, dy, board, whiteTurn);
	case 'R':case 'r':
		return isValidRookMove(sx, sy, dx, dy, board, whiteTurn);
	case 'N':case 'n':
		return isValidKnightMove(sx, sy, dx, dy, board, whiteTurn);
	case 'K':case 'k':
		return isValidKingMove(sx, sy, dx, dy, board, whiteTurn);
	case 'Q':case 'q':
		return isValidQueenMove(sx, sy, dx, dy, board, whiteTurn);
	}return false;
}
//pawn promotion
void checkPawnPromotion(char board[][8], bool whiteTurn) {
	int checkrow;
	if (whiteTurn) {
		checkrow = 0; // white pawns reach row 0
	}
	else {
		checkrow = 7; // black pawns reach row 7
	}

	for (int j = 0; j < 8; j++) {
		char piece = board[checkrow][j];

		if (whiteTurn && piece == 'P') { // white pawn promotion
			char promoteTo;
			while (true) {
				cout << "White pawn reached the end! Choose promotion piece (Q, R, B, N): ";
				cin >> promoteTo;

				// convert lowercase to uppercase
				if (promoteTo >= 'a' && promoteTo <= 'z') {
					promoteTo = promoteTo - 32;
				}

				if (promoteTo == 'Q' || promoteTo == 'R' || promoteTo == 'B' || promoteTo == 'N') {
					board[checkrow][j] = promoteTo;
					break;
				}
				else {
					cout << "Invalid input! Try again.\n";
				}
			}
		}

		if (!whiteTurn && piece == 'p') { // black pawn promotion
			char promoteTo;
			while (true) {
				cout << "Black pawn reached the end! Choose promotion piece (q, r, b, n): ";
				cin >> promoteTo;

				// convert uppercase to lowercase
				if (promoteTo >= 'A' && promoteTo <= 'Z') {
					promoteTo = promoteTo + 32;
				}

				if (promoteTo == 'q' || promoteTo == 'r' || promoteTo == 'b' || promoteTo == 'n') {
					board[checkrow][j] = promoteTo;
					break;
				}
				else {
					cout << "Invalid input! Try again.\n";
				}
			}
		}
	}
}


//finding king
bool FindKing(char board[][8], int& kx, int& ky, bool whiteTurn)
{
	char kingchar;

	//  searching the OPPONENT king
	if (whiteTurn) {
		kingchar = 'k';
	}
	else {
		kingchar = 'K';
	}
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {

			if (board[i][j] == kingchar) {
				kx = i;
				ky = j;
				return true;
			}

		}
	}
	cout << "King Not Found\n";
	return false;
}
//square attacking funciton
bool isSquareAttack(char board[][8], int& kx, int& ky, bool whiteTurn) {
	if (whiteTurn) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				// Black pawn attacks
				if (board[i][j] == 'p') {
					if ((i + 1 == kx && j - 1 == ky) || (i + 1 == kx && j + 1 == ky))
						return true;
				}

				// Black knight attacks
				if (board[i][j] == 'n') {
					if ((i - 2 == kx && j - 1 == ky) || (i - 2 == kx && j + 1 == ky) ||
						(i - 1 == kx && j - 2 == ky) || (i - 1 == kx && j + 2 == ky) ||
						(i + 1 == kx && j - 2 == ky) || (i + 1 == kx && j + 2 == ky) ||
						(i + 2 == kx && j - 1 == ky) || (i + 2 == kx && j + 1 == ky))
						return true;
				}
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				// White pawn attacks
				if (board[i][j] == 'P') {
					if ((i - 1 == kx && j - 1 == ky) || (i - 1 == kx && j + 1 == ky))
						return true;
				}

				// White knight attacks
				if (board[i][j] == 'N') {
					if ((i - 2 == kx && j - 1 == ky) || (i - 2 == kx && j + 1 == ky) ||
						(i - 1 == kx && j - 2 == ky) || (i - 1 == kx && j + 2 == ky) ||
						(i + 1 == kx && j - 2 == ky) || (i + 1 == kx && j + 2 == ky) ||
						(i + 2 == kx && j - 1 == ky) || (i + 2 == kx && j + 1 == ky))
						return true;
				}
			}
		}
	}

	// Rook/Queen straight lines
// Up
	for (int r = kx - 1; r >= 0; r--) {
		if (board[r][ky] == ' ') continue;
		if ((whiteTurn && (board[r][ky] == 'r' || board[r][ky] == 'q')) ||
			(!whiteTurn && (board[r][ky] == 'R' || board[r][ky] == 'Q')))
			return true;
		break;
	}
	// Down
	for (int r = kx + 1; r < 8; r++) {
		if (board[r][ky] == ' ') continue;
		if ((whiteTurn && (board[r][ky] == 'r' || board[r][ky] == 'q')) ||
			(!whiteTurn && (board[r][ky] == 'R' || board[r][ky] == 'Q')))
			return true;
		break;
	}
	// Left
	for (int r = ky - 1; r >= 0; r--) {
		if (board[kx][r] == ' ') continue;
		if ((whiteTurn && (board[kx][r] == 'r' || board[kx][r] == 'q')) ||
			(!whiteTurn && (board[kx][r] == 'R' || board[kx][r] == 'Q')))
			return true;
		break;
	}
	// Right
	for (int r = ky + 1; r < 8; r++) {
		if (board[kx][r] == ' ') continue;
		if ((whiteTurn && (board[kx][r] == 'r' || board[kx][r] == 'q')) ||
			(!whiteTurn && (board[kx][r] == 'R' || board[kx][r] == 'Q')))
			return true;
		break;
	}

	// Bishop/Queen diagonals
	int dr[4] = { -1,-1,1,1 };
	int dc[4] = { -1,1,-1,1 };
	for (int d = 0; d < 4; d++) {
		int r = kx + dr[d];
		int c = ky + dc[d];
		while (r >= 0 && r < 8 && c >= 0 && c < 8) {
			if (board[r][c] == ' ') { r += dr[d]; c += dc[d]; continue; }
			if ((whiteTurn && (board[r][c] == 'b' || board[r][c] == 'q')) ||
				(!whiteTurn && (board[r][c] == 'B' || board[r][c] == 'Q')))
				return true;
			break;
		}
	}


	// Opponent king adjacent
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue;
			int nx = kx + dx, ny = ky + dy;
			if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
				if ((whiteTurn && board[nx][ny] == 'k') || (!whiteTurn && board[nx][ny] == 'K'))
					return true;
			}
		}
	}

	return false;
}

bool isCheck(char board[][8], int kx, int ky, bool whiteTurn) {
	return isSquareAttack(board, kx, ky, whiteTurn);
}

bool isCheckMate(char board[][8], int kx, int ky, bool whiteTurn) {
	if (!isSquareAttack(board, kx, ky, whiteTurn))
		return false;

	//checking for all 8 direction 
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if ((dx == 0) && (dy == 0))
				continue;

			int nx = kx + dx;//nx and ny are the new positions of king
			int ny = ky + dy;

			if (nx < 0 || nx>7 || ny < 0 || ny>7)
				continue;

			if (whiteTurn) {
				if (board[nx][ny] >= 'A' && board[nx][ny] <= 'Z') // friendly piece
					continue;
			}
			else {
				if (board[nx][ny] >= 'a' && board[nx][ny] <= 'z') // friendly piece
					continue;
			}

			char temp = board[nx][ny]; //moving king to imaginary space in 8 blocks around
			if (whiteTurn)
				board[nx][ny] = 'K';
			else
				board[nx][ny] = 'k';
			board[kx][ky] = ' ';
			//checking if the king is safe in new position
			bool safe;
			if (!isSquareAttack(board, nx, ny, whiteTurn))
				safe = true;
			else
				safe = false;
			//restoring the king original coordinates
			board[nx][ny] = temp;
			if (whiteTurn)
				board[kx][ky] = 'K';
			else
				board[kx][ky] = 'k';
			if (safe)
				return false;
		}
	}
	for (int i = 0;i < 8;i++) {
		for (int j = 0;j < 8;j++) {
			char piece = board[i][j];
			if (piece == ' ')
				continue;	//empty square

			if (whiteTurn) {        //enemy piece detection
				if (piece >= 'a' && piece <= 'z') continue;
			}
			else {
				if (piece >= 'A' && piece <= 'Z') continue;
			}

			//now we will move this pice to all possible square at which the piece can  legally move
			for (int tx = 0;tx < 8;tx++) {
				for (int ty = 0;ty < 8;ty++) {
					if (isValidMove(piece, i, j, tx, ty, board, whiteTurn)) {
						char temp = board[tx][ty];//agr valid move h to piece ko udr move krky check wala scn check krna h
						board[tx][ty] = board[i][j];
						board[i][j] = ' ';
						if (!isSquareAttack(board, kx, ky, whiteTurn)) {//if check go removed
							board[i][j] = board[tx][ty]; //restoring
							board[tx][ty] = temp;
							return false;  // not checkmate, piece can save king
						}
						board[i][j] = board[tx][ty];//restoring if upper condition is false
						board[tx][ty] = temp;
					}
				}
			}
		}
	}

	return true;
}
int main() {
	const int size = 8;
	char board[size][size];
	int sx, sy, dx, dy, kx, ky;
	bool result, whiteTurn = true;

	board_startup(board, size);

	while (true) {

		printing(board, size);

		if (whiteTurn)
			cout << "White's turn.\n";
		else
			cout << "Black's turn.\n";

		cout << "Enter source row and column: ";
		cin >> sx >> sy;

		char piece = board[sx][sy];

		if (piece == ' ') {
			cout << "There is no piece here!\n";
			continue;
		}

		if (whiteTurn) {
			if (!(piece >= 'A' && piece <= 'Z')) {
				cout << "Not a white piece!\n";
				continue;
			}
		}
		else {
			if (!(piece >= 'a' && piece <= 'z')) {
				cout << "Not a black piece!\n";
				continue;
			}
		}

		cout << "Enter destination row and column: ";
		cin >> dx >> dy;

		bool valid = isValidMove(piece, sx, sy, dx, dy, board, whiteTurn);

		if (valid) {
			movePiece(sx, sy, dx, dy, board);
			cout << "Move done.\n";
			checkPawnPromotion(board, whiteTurn);
			printing(board, size);
			FindKing(board, kx, ky, whiteTurn); // find the king of the player who just moved

			if (isCheck(board, kx, ky, !whiteTurn)) {
				cout << "Check!\n";

				if (isCheckMate(board, kx, ky, !whiteTurn)) {
					cout << "Checkmate!\n";

					if (whiteTurn) {
						cout << "White wins!" << endl;
					}
					else {
						cout << "Black wins!" << endl;
					}

					break; // stop the game
				}
			}

			// switch turn
			whiteTurn = !whiteTurn;

		}
		else {
			cout << "Invalid move!\n";
		}
	}
	return 0;
}
