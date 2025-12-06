
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
using namespace sf;

int whiteScore = 0, blackScore = 0;
char whiteCaptured[16], blackCaptured[16];
int whiteCount = 0, blackCount = 0;

char board[8][8] = {
	{ 'r','n','b','q','k','b','n','r' },
	{ 'p','p','p','p','p','p','p','p' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ' },
	{ ' ',' ',' ',' ',' ',' ',' ',' ' },
	{ 'P','P','P','P','P','P','P','P' },
	{ 'R','N','B','Q','K','B','N','R' }
};

// Function to print the board (ranks 8..1)
void printing(char board[][8], int size) {
	cout << "   a b c d e f g h\n";
	cout << "  -----------------\n";
	for (int i = 0; i < size; i++) {
		cout << 8 - i << "| ";
		for (int j = 0; j < size; j++) {
			cout << board[i][j] << " ";
		}
		cout << "| " << 8 - i << "\n";
	}
	cout << "  -----------------\n";
	cout << "   a b c d e f g h\n";
}

// --- Movement validators  ---
// Pawn
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

	// single step forward
	if (dx == sx + dir && dy == sy && board[dx][dy] == ' ')
		return true;

	// double step from start row
	if (sx == startrow && dx == sx + 2 * dir && dy == sy && board[sx + dir][sy] == ' ' && board[dx][dy] == ' ')
		return true;

	// capture diagonally
	if (dx == sx + dir && (dy == sy + 1 || dy == sy - 1) && board[dx][dy] != ' ') {
		if (whiteTurn && board[dx][dy] >= 'a' && board[dx][dy] <= 'z')
			return true;
		if (!whiteTurn && board[dx][dy] >= 'A' && board[dx][dy] <= 'Z')
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

	int rowDir = (rowDiff > 0) ? 1 : -1;
	int colDir = (colDiff > 0) ? 1 : -1;

	int x = sx + rowDir;
	int y = sy + colDir;

	while (x != dx && y != dy) {
		if (board[x][y] != ' ')
			return false;
		x += rowDir;
		y += colDir;
	}

	char dest = board[dx][dy];

	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z') return false;
	}
	else {
		if (dest >= 'a' && dest <= 'z') return false;
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
	return isValidRookMove(sx, sy, dx, dy, board, whiteTurn) || isValidBishopMove(sx, sy, dx, dy, board, whiteTurn);
}

// King
bool isValidKingMove(int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	int row = dx - sx; if (row < 0) row = -row;
	int col = dy - sy; if (col < 0) col = -col;

	if (row > 1 || col > 1) return false;
	if (row == 0 && col == 0) return false;

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
	int row = dx - sx; if (row < 0) row = -row;
	int col = dy - sy; if (col < 0) col = -col;

	if (!((row == 2 && col == 1) || (row == 1 && col == 2))) return false;

	char dest = board[dx][dy];

	if (whiteTurn) {
		if (dest >= 'A' && dest <= 'Z') return false;
	}
	else {
		if (dest >= 'a' && dest <= 'z') return false;
	}

	return true;
}
//prototypes
int pieceValue(char piece);
void updateScore(char capturedPiece, bool attackerWhite);

// Move piece (board logic)
void movePiece(int sx, int sy, int dx, int dy, char board[8][8]) {

	char source = board[sy][sx];
	char target = board[dy][dx];

	// MOVE THE PIECE 
	board[dy][dx] = board[sy][sx];
	board[sy][sx] = ' ';
}

bool isValidMove(char piece, int sx, int sy, int dx, int dy, char board[][8], bool whiteTurn) {
	switch (piece) {
	case 'P': case 'p': return isValidPawnMove(sx, sy, dx, dy, board, whiteTurn);
	case 'B': case 'b': return isValidBishopMove(sx, sy, dx, dy, board, whiteTurn);
	case 'R': case 'r': return isValidRookMove(sx, sy, dx, dy, board, whiteTurn);
	case 'N': case 'n': return isValidKnightMove(sx, sy, dx, dy, board, whiteTurn);
	case 'K': case 'k': return isValidKingMove(sx, sy, dx, dy, board, whiteTurn);
	case 'Q': case 'q': return isValidQueenMove(sx, sy, dx, dy, board, whiteTurn);
	}
	return false;
}

// Pawn promotion (asks user via console)
void checkPawnPromotion(char board[][8], bool whiteTurn) {
	int checkrow = whiteTurn ? 0 : 7;

	for (int j = 0; j < 8; j++) {
		char piece = board[checkrow][j];

		if (whiteTurn && piece == 'P') {
			char promoteTo;
			while (true) {
				cout << "White pawn reached the end! Choose promotion piece (Q, R, B, N): ";
				cin >> promoteTo;
				if (promoteTo >= 'a' && promoteTo <= 'z') promoteTo = promoteTo - 32;
				if (promoteTo == 'Q' || promoteTo == 'R' || promoteTo == 'B' || promoteTo == 'N') {
					board[checkrow][j] = promoteTo;
					break;
				}
				else {
					cout << "Invalid input! Try again.\n";
				}
			}
		}

		if (!whiteTurn && piece == 'p') {
			char promoteTo;
			while (true) {
				cout << "Black pawn reached the end! Choose promotion piece (q, r, b, n): ";
				cin >> promoteTo;
				if (promoteTo >= 'A' && promoteTo <= 'Z') promoteTo = promoteTo + 32;
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

// Finding king (search opponent king) 
bool FindKing(char board[][8], int& kx, int& ky, bool whiteTurn) {
	char kingchar = whiteTurn ? 'k' : 'K';
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] == kingchar) {
				kx = i; ky = j;
				return true;
			}
		}
	}
	// Not found
	return false;
}

// Helper: find own king (whiteKing==true -> find 'K', else find 'k')
bool findOwnKing(char board[][8], bool whiteKing, int& kx, int& ky) {
	char kingchar = whiteKing ? 'K' : 'k';
	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 8; ++j)
			if (board[i][j] == kingchar) { kx = i; ky = j; return true; }
	return false;
}

// Is square under attack? (kx,ky are coordinates to test against)
// NOTE: parameter whiteTurn: when true, function checks attacks from BLACK pieces (as in original implementation)
// kept identical to original implementation
bool isSquareAttack(char board[][8], int& kx, int& ky, bool whiteTurn) {
	if (whiteTurn) {
		// check for black pawn and knight attacks
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == 'p') {
					if ((i + 1 == kx && j - 1 == ky) || (i + 1 == kx && j + 1 == ky))
						return true;
				}
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
		// check for white pawn and knight attacks
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (board[i][j] == 'P') {
					if ((i - 1 == kx && j - 1 == ky) || (i - 1 == kx && j + 1 == ky))
						return true;
				}
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
	for (int c = ky - 1; c >= 0; c--) {
		if (board[kx][c] == ' ') continue;
		if ((whiteTurn && (board[kx][c] == 'r' || board[kx][c] == 'q')) ||
			(!whiteTurn && (board[kx][c] == 'R' || board[kx][c] == 'Q')))
			return true;
		break;
	}
	// Right
	for (int c = ky + 1; c < 8; c++) {
		if (board[kx][c] == ' ') continue;
		if ((whiteTurn && (board[kx][c] == 'r' || board[kx][c] == 'q')) ||
			(!whiteTurn && (board[kx][c] == 'R' || board[kx][c] == 'Q')))
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

	// Try all 8 king moves
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) continue;
			int nx = kx + dx;
			int ny = ky + dy;
			if (nx < 0 || nx > 7 || ny < 0 || ny > 7) continue;

			if (whiteTurn) {
				if (board[nx][ny] >= 'A' && board[nx][ny] <= 'Z') continue;
			}
			else {
				if (board[nx][ny] >= 'a' && board[nx][ny] <= 'z') continue;
			}

			char temp = board[nx][ny];
			if (whiteTurn) board[nx][ny] = 'K'; else board[nx][ny] = 'k';
			board[kx][ky] = ' ';
			bool safe = !isSquareAttack(board, nx, ny, whiteTurn);
			board[nx][ny] = temp;
			if (whiteTurn) board[kx][ky] = 'K'; else board[kx][ky] = 'k';
			if (safe) return false;
		}
	}

	// Try to block or capture check by other pieces
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			char piece = board[i][j];
			if (piece == ' ') continue;

			// skip opponent pieces
			if (whiteTurn) {
				if (piece >= 'a' && piece <= 'z') continue;
			}
			else {
				if (piece >= 'A' && piece <= 'Z') continue;
			}

			for (int tx = 0; tx < 8; tx++) {
				for (int ty = 0; ty < 8; ty++) {
					if (isValidMove(piece, i, j, tx, ty, board, whiteTurn)) {
						char temp = board[tx][ty];
						board[tx][ty] = board[i][j];
						board[i][j] = ' ';
						if (!isSquareAttack(board, kx, ky, whiteTurn)) {
							board[i][j] = board[tx][ty];
							board[tx][ty] = temp;
							return false;
						}
						board[i][j] = board[tx][ty];
						board[tx][ty] = temp;
					}
				}
			}
		}
	}

	return true;
}
int pieceValue(char piece) {
	switch (piece) {
	case 'P': return 1;
	case 'N': return 3;
	case 'B': return 3;
	case 'R': return 5;
	case 'Q': return 9;
	case 'K': return 100;
	}
	return 0;
}

void updateScore(char capturedPiece, bool whiteTurn) {
	int val = pieceValue(toupper(capturedPiece));

	if (whiteTurn)
		whiteScore += val;
	else
		blackScore += val;
}


// Helper: map piece char to texture pointer (we'll set after textures are loaded)
Texture* getTextureForPiece(char piece,
	Texture& wPawn, Texture& bPawn,
	Texture& wRook, Texture& bRook,
	Texture& wKnight, Texture& bKnight,
	Texture& wBishop, Texture& bBishop,
	Texture& wQueen, Texture& bQueen,
	Texture& wKing, Texture& bKing)
{
	switch (piece) {
	case 'P': return &wPawn;
	case 'p': return &bPawn;
	case 'R': return &wRook;
	case 'r': return &bRook;
	case 'N': return &wKnight;
	case 'n': return &bKnight;
	case 'B': return &wBishop;
	case 'b': return &bBishop;
	case 'Q': return &wQueen;
	case 'q': return &bQueen;
	case 'K': return &wKing;
	case 'k': return &bKing;
	default: return nullptr;
	}
}

int main() {
	bool isDragging = false;
	bool whiteTurn = true;
	const int size = 8;
	int draggedRow = -1, draggedCol = -1;
	int originalRow = -1, originalCol = -1;
	float squareSize = 98.25f;

	// valid moves matrix (no vector)
	bool validMoves[8][8] = { {false} };

	// temporary dragged sprite visual
	Sprite draggedVisual;
	char draggedChar = ' ';

	// Print console board initially
	printing(board, size);

	RenderWindow window(VideoMode(786, 786), "Chess");

	Font font;
	if (!font.loadFromFile("ARIAL.ttf")) {
		cerr << "Font loading failed!\n";
		return -1;
	}
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(26);
	scoreText.setFillColor(sf::Color::Blue);
	scoreText.setPosition(600, 10);


	Text turnText("", font, 22);
	turnText.setPosition(10, 6);

	Text statusText("", font, 24);
	statusText.setFillColor(Color::Red);
	statusText.setPosition(300, 6);

	// --- Load textures ---
	Texture whitePawnTex, blackPawnTex;
	Texture whiteRookTex, blackRookTex;
	Texture whiteKnightTex, blackKnightTex;
	Texture whiteBishopTex, blackBishopTex;
	Texture whiteQueenTex, blackQueenTex;
	Texture whiteKingTex, blackKingTex;

	if (!whitePawnTex.loadFromFile("images/white-pawn.png") ||
		!blackPawnTex.loadFromFile("images/black-pawn.png") ||
		!whiteRookTex.loadFromFile("images/white-rook.png") ||
		!blackRookTex.loadFromFile("images/black-rook.png") ||
		!whiteKnightTex.loadFromFile("images/white-knight.png") ||
		!blackKnightTex.loadFromFile("images/black-knight.png") ||
		!whiteBishopTex.loadFromFile("images/white-bishop.png") ||
		!blackBishopTex.loadFromFile("images/black-bishop.png") ||
		!whiteQueenTex.loadFromFile("images/white-queen.png") ||
		!blackQueenTex.loadFromFile("images/black-queen.png") ||
		!whiteKingTex.loadFromFile("images/white-king.png") ||
		!blackKingTex.loadFromFile("images/black-king.png")) {
		cerr << "Failed to load textures!" << endl;
		return -1;
	}

	// Sprites pool (we will place them according to board each frame)
	Sprite whitePawns[8], blackPawns[8];
	Sprite whiteRooks[2], blackRooks[2];
	Sprite whiteKnights[2], blackKnights[2];
	Sprite whiteBishops[2], blackBishops[2];
	Sprite whiteQueen, blackQueen;
	Sprite whiteKing, blackKing;

	// initial setup (textures & scale; positions will be synced from board each frame)
	for (int i = 0; i < 8; ++i) {
		whitePawns[i].setTexture(whitePawnTex);
		whitePawns[i].setScale(0.48f, 0.48f);
		blackPawns[i].setTexture(blackPawnTex);
		blackPawns[i].setScale(0.48f, 0.48f);
	}
	for (int i = 0; i < 2; ++i) {
		whiteRooks[i].setTexture(whiteRookTex); whiteRooks[i].setScale(0.48f, 0.48f);
		blackRooks[i].setTexture(blackRookTex); blackRooks[i].setScale(0.48f, 0.48f);
		whiteKnights[i].setTexture(whiteKnightTex); whiteKnights[i].setScale(0.48f, 0.48f);
		blackKnights[i].setTexture(blackKnightTex); blackKnights[i].setScale(0.48f, 0.48f);
		whiteBishops[i].setTexture(whiteBishopTex); whiteBishops[i].setScale(0.48f, 0.48f);
		blackBishops[i].setTexture(blackBishopTex); blackBishops[i].setScale(0.48f, 0.48f);
	}
	whiteQueen.setTexture(whiteQueenTex); whiteQueen.setScale(0.48f, 0.48f);
	blackQueen.setTexture(blackQueenTex); blackQueen.setScale(0.48f, 0.48f);
	whiteKing.setTexture(whiteKingTex); whiteKing.setScale(0.48f, 0.48f);
	blackKing.setTexture(blackKingTex); blackKing.setScale(0.48f, 0.48f);

	// small highlight for valid moves
	RectangleShape greenHighlight(Vector2f(squareSize, squareSize));
	greenHighlight.setFillColor(Color(0, 255, 0, 110)); // translucent green

	// letters for top labels
	const char letters[] = "abcdefgh";

	// main loop
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) window.close();

			// Mouse pressed: pick a piece (only pick pieces that belong to current turn)
			if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
				int mouseX = event.mouseButton.x;
				int mouseY = event.mouseButton.y;
				int col = static_cast<int>(mouseX / squareSize);
				int row = static_cast<int>(mouseY / squareSize);
				if (col >= 0 && col < 8 && row >= 0 && row < 8) {
					char piece = board[row][col];
					if (piece != ' ') {
						bool isWhitePiece = (piece >= 'A' && piece <= 'Z');
						if ((whiteTurn && isWhitePiece) || (!whiteTurn && !isWhitePiece)) {
							// compute valid moves (populate validMoves[][])
							for (int r = 0; r < 8; ++r)
								for (int c = 0; c < 8; ++c)
									validMoves[r][c] = false;

							// For each target square test isValidMove and king-safety
							for (int tx = 0; tx < 8; ++tx) {
								for (int ty = 0; ty < 8; ++ty) {
									if (isValidMove(piece, row, col, tx, ty, board, whiteTurn)) {
										// simulate move and check king safety
										char savedSrc = board[row][col];
										char savedDst = board[tx][ty];
										board[tx][ty] = board[row][col];
										board[row][col] = ' ';

										// find own king pos (if we moved the king, its pos changed)
										int kx = -1, ky = -1;
										bool found;
										if ((savedSrc == 'K') || (savedSrc == 'k')) {
											// we moved the king; its new position is tx,ty
											kx = tx; ky = ty; found = true;
										}
										else {
											// find own king normally
											found = findOwnKing(board, (whiteTurn ? true : false), kx, ky);
										}

										bool kingSafe = true;
										if (found) {
											// isSquareAttack expects whiteTurn to indicate checking for BLACK attacks when true
											// We want to know if our king (white if whiteTurn) is attacked by opponent:
											// isSquareAttack(board, kx, ky, whiteTurn) -> checks opponent attacks as in original semantics
											if (isSquareAttack(board, kx, ky, whiteTurn))
												kingSafe = false;
										}

										// restore
										board[row][col] = savedSrc;
										board[tx][ty] = savedDst;

										if (kingSafe) validMoves[tx][ty] = true;
									}
								}
							}

							// if there are any valid moves, start dragging
							bool any = false;
							for (int r = 0; r < 8 && !any; ++r)
								for (int c = 0; c < 8 && !any; ++c)
									if (validMoves[r][c]) any = true;

							if (any) {
								isDragging = true;
								draggedRow = row; draggedCol = col;
								originalRow = row; originalCol = col;
								draggedChar = piece;

								// prepare draggedVisual texture
								Texture* t = getTextureForPiece(piece,
									whitePawnTex, blackPawnTex,
									whiteRookTex, blackRookTex,
									whiteKnightTex, blackKnightTex,
									whiteBishopTex, blackBishopTex,
									whiteQueenTex, blackQueenTex,
									whiteKingTex, blackKingTex);
								if (t) {
									draggedVisual.setTexture(*t);
									draggedVisual.setScale(0.48f, 0.48f);
									draggedVisual.setOrigin(0, 0);
									draggedVisual.setPosition(mouseX - squareSize / 2, mouseY - squareSize / 2);
								}

								// temporarily remove piece from board for visual (we will place it back on drop)
								board[row][col] = ' ';
							} // if any
						} // if piece belongs to side
					} // if piece != ' '
				} // if valid cell
			} // MousePressed

			// Mouse moved: move dragged visual
			if (event.type == Event::MouseMoved && isDragging) {
				draggedVisual.setPosition(event.mouseMove.x - squareSize / 2, event.mouseMove.y - squareSize / 2);
			}

			// Mouse released: attempt to drop
			if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
				if (isDragging) {
					int dropCol = static_cast<int>(event.mouseButton.x / squareSize);
					int dropRow = static_cast<int>(event.mouseButton.y / squareSize);
					bool moved = false;
					if (dropCol >= 0 && dropCol < 8 && dropRow >= 0 && dropRow < 8) {
						if (validMoves[dropRow][dropCol]) {
							char target = board[dropRow][dropCol];
							char source = board[draggedRow][draggedCol];

							// Check if a piece is captured
							if (target != ' ') {
								char source = draggedChar;   // IMPORTANT FIX
								bool attackerWhite = (source >= 'A' && source <= 'Z');

								// store captured piece
								if (attackerWhite)
									whiteCaptured[whiteCount++] = target;
								else
									blackCaptured[blackCount++] = target;

								// update score
								updateScore(target, attackerWhite);


								cout << "White Score: " << whiteScore
									<< "   Black Score: " << blackScore << endl;

								// Print captured pieces
								cout << "\t\t\t\tWhite has captured: ";
								for (int i = 0; i < whiteCount; i++) cout << whiteCaptured[i] << " ";
								cout << endl;

								cout << "\t\t\t\tBlack has captured: ";
								for (int i = 0; i < blackCount; i++) cout << blackCaptured[i] << " ";
								cout << endl;
							}

							// Perform move
							board[dropRow][dropCol] = draggedChar;
							board[draggedRow][draggedCol] = ' ';


							// pawn promotion handled after
							checkPawnPromotion(board, whiteTurn);

							// print board so console matches SFML
							printing(board, size);

							// check for check/checkmate on opponent
							int kx = -1, ky = -1;
							if (FindKing(board, kx, ky, whiteTurn)) {
								if (isCheck(board, kx, ky, !whiteTurn)) {
									statusText.setString("Check!");
									if (isCheckMate(board, kx, ky, !whiteTurn)) {
										statusText.setString("Checkmate!");
										cout << (whiteTurn ? "White wins!" : "Black wins!") << endl;
										window.close();
									}
								}
								else {
									statusText.setString("");
								}
							}

							// switch turn
							whiteTurn = !whiteTurn;
							moved = true;
						}
					}

					if (!moved) {
						// invalid drop — restore piece to original square
						board[originalRow][originalCol] = draggedChar;
					}

					// clear drag state and valid moves
					isDragging = false;
					draggedChar = ' ';
					draggedRow = draggedCol = -1;
					originalRow = originalCol = -1;
					for (int r = 0; r < 8; ++r) for (int c = 0; c < 8; ++c) validMoves[r][c] = false;
				}
			} // MouseReleased
		} // event polling

		// --- Sync sprites to board each frame (so SFML view matches board) ---
		// Move all sprites off-screen first
		const float offX = -200.f, offY = -200.f;
		for (int i = 0; i < 8; ++i) { whitePawns[i].setPosition(offX, offY); blackPawns[i].setPosition(offX, offY); }
		for (int i = 0; i < 2; ++i) {
			whiteRooks[i].setPosition(offX, offY); blackRooks[i].setPosition(offX, offY);
			whiteKnights[i].setPosition(offX, offY); blackKnights[i].setPosition(offX, offY);
			whiteBishops[i].setPosition(offX, offY); blackBishops[i].setPosition(offX, offY);
		}
		whiteQueen.setPosition(offX, offY); blackQueen.setPosition(offX, offY);
		whiteKing.setPosition(offX, offY); blackKing.setPosition(offX, offY);

		// counters for which sprite to use
		int wp = 0, bp = 0, wr = 0, br = 0, wn = 0, bn = 0, wb = 0, bb = 0;
		for (int r = 0; r < 8; ++r) {
			for (int c = 0; c < 8; ++c) {
				char p = board[r][c];
				if (p == 'P') {
					if (wp < 8) whitePawns[wp].setPosition(c * squareSize + 12, r * squareSize + 8), ++wp;
				}
				else if (p == 'p') {
					if (bp < 8) blackPawns[bp].setPosition(c * squareSize + 12, r * squareSize + 8), ++bp;
				}
				else if (p == 'R') {
					if (wr < 2) whiteRooks[wr].setPosition(c * squareSize + 12, r * squareSize + 8), ++wr;
				}
				else if (p == 'r') {
					if (br < 2) blackRooks[br].setPosition(c * squareSize + 12, r * squareSize + 8), ++br;
				}
				else if (p == 'N') {
					if (wn < 2) whiteKnights[wn].setPosition(c * squareSize + 12, r * squareSize + 8), ++wn;
				}
				else if (p == 'n') {
					if (bn < 2) blackKnights[bn].setPosition(c * squareSize + 12, r * squareSize + 8), ++bn;
				}
				else if (p == 'B') {
					if (wb < 2) whiteBishops[wb].setPosition(c * squareSize + 12, r * squareSize + 8), ++wb;
				}
				else if (p == 'b') {
					if (bb < 2) blackBishops[bb].setPosition(c * squareSize + 12, r * squareSize + 8), ++bb;
				}
				else if (p == 'Q') {
					whiteQueen.setPosition(c * squareSize + 12, r * squareSize + 8);
				}
				else if (p == 'q') {
					blackQueen.setPosition(c * squareSize + 12, r * squareSize + 8);
				}
				else if (p == 'K') {
					whiteKing.setPosition(c * squareSize + 12, r * squareSize + 8);
				}
				else if (p == 'k') {
					blackKing.setPosition(c * squareSize + 12, r * squareSize + 8);
				}
			}
		}

		// --- Hover effect (only when not dragging) ---
		Vector2i mousePos = Mouse::getPosition(window);
		int hoverCol = -1, hoverRow = -1;
		if (!isDragging) {
			hoverCol = static_cast<int>(mousePos.x / squareSize);
			hoverRow = static_cast<int>(mousePos.y / squareSize);
			if (hoverCol < 0 || hoverCol > 7 || hoverRow < 0 || hoverRow > 7) hoverCol = hoverRow = -1;
		}
		scoreText.setString(
			"White: " + to_string(whiteScore) +
			"\nBlack: " + to_string(blackScore)
		);


		// --- Draw board ---
		window.clear(Color::White);
		Color gray(90, 90, 90);

		for (int row = 0; row < 8; ++row) {
			for (int col = 0; col < 8; ++col) {
				RectangleShape square(Vector2f(squareSize, squareSize));
				square.setPosition(col * squareSize, row * squareSize);
				square.setFillColor((row + col) % 2 == 0 ? Color::White : gray);

				if (row == hoverRow && col == hoverCol) {
					square.setOutlineThickness(-3);
					square.setOutlineColor(Color::Yellow);
				}
				else {
					square.setOutlineThickness(0);
				}
				window.draw(square);
			}
		}

		// --- Draw valid move highlights (green) ---
		for (int r = 0; r < 8; ++r) {
			for (int c = 0; c < 8; ++c) {
				if (validMoves[r][c]) {
					greenHighlight.setPosition(c * squareSize, r * squareSize);
					window.draw(greenHighlight);
				}
			}
		}

		// --- Draw small a-h at top corners and 1-8 at left corners ---
		for (int i = 0; i < 8; ++i) {

			// Top letters
			Text tTop;
			tTop.setFont(font);
			tTop.setCharacterSize(14);

			sf::String s;
			s += letters[i];   // <---- fix here
			tTop.setString(s);

			tTop.setFillColor(Color::Black);
			float x = i * squareSize + squareSize - 18.f;
			float y = 2.0f;
			tTop.setPosition(x, y);
			window.draw(tTop);
		}

		for (int i = 0; i < 8; ++i) {
			Text tLeft;
			tLeft.setFont(font);
			tLeft.setCharacterSize(14);
			// number string
			char numch[3] = { char('0' + (8 - i)), 0, 0 };
			tLeft.setString(numch);
			tLeft.setFillColor(Color::Black);
			float x = 2.0f;
			float y = i * squareSize + squareSize - 18.f; // near bottom-left corner of square
			tLeft.setPosition(x, y);
			window.draw(tLeft);
		}

		// --- Draw pieces (sprites) ---
		for (int i = 0; i < 8; ++i) {
			window.draw(whitePawns[i]);
			window.draw(blackPawns[i]);
		}
		for (int i = 0; i < 2; ++i) {
			window.draw(whiteRooks[i]);
			window.draw(blackRooks[i]);
			window.draw(whiteKnights[i]);
			window.draw(blackKnights[i]);
			window.draw(whiteBishops[i]);
			window.draw(blackBishops[i]);
		}
		window.draw(whiteQueen);
		window.draw(blackQueen);
		window.draw(whiteKing);
		window.draw(blackKing);

		// --- Draw dragged visual on top if dragging ---
		if (isDragging) {
			window.draw(draggedVisual);
		}

		// --- Draw turn box/text ---
		RectangleShape turnBox(Vector2f(120.f, 30.f));
		turnBox.setPosition(10.f, 6.f);
		if (whiteTurn) {
			turnBox.setFillColor(Color(230, 230, 0));
			turnText.setFillColor(Color::Black);
			turnText.setString("Turn: White");
		}
		else {
			turnBox.setFillColor(Color(40, 40, 40));
			turnText.setFillColor(Color::White);
			turnText.setString("Turn: Black");
		}
		window.draw(turnBox);
		window.draw(turnText);

		// draw status text
		window.draw(statusText);
		window.draw(scoreText);


		window.display();
	}

	return 0;
}

