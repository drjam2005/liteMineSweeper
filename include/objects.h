#include <raylib.h>
#include <vector>

enum GAME_STATE {
	PLAYING,
	WIN,
	LOSE
};

enum STATE {
	REVEALED,
	HIDDEN,
	FLAGGED
};

enum UPDATETYPE {
	ADD_BOMB,
	REMOVE_BOMB,
	CLEAR
};

enum TYPE {
	EMPTY,
	BOMB,
};

class Square {
public:
	STATE state = HIDDEN;
	TYPE type = EMPTY;
	int closeBombs = 0;

	Vector2 indexPosition;
};


class Grid {
	GAME_STATE gameState = PLAYING;
	Square** squares = nullptr;
	bool firstMove = true;
	int sideCount = 0;
	int squareWidth = 0;
	Vector2 selected = {-1, -1};
public:
	Grid(int, int);

	void Reset();
	void DrawGrid();
	void UpdateGrid();
	void RevealArea(Vector2, std::vector<Vector2>&);
	void UpdateGridInfo(Vector2, UPDATETYPE);
};

class Interface {
public:
	Grid* grid = nullptr;
	Interface(int sideCount, int squareWidth) {
		grid = new Grid(sideCount, squareWidth);
	};
};
