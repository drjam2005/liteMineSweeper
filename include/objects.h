#include <raylib.h>
#include <vector>

enum STATE {
	REVEALED,
	HIDDEN,
	FLAGGED
};

enum UPDATETYPE {
	ADD_BOMB,
	CLEAR
};

enum TYPE {
	EMPTY,
	NUMBER,
	BOMB,
};

class Square {
public:
	STATE state = HIDDEN;
	TYPE type;
	int closeBombs = 0;

	Vector2 indexPosition;
};


class Grid {
	Square** squares = nullptr;
	int sideCount = 0;
	int squareWidth = 0;
	Vector2 selected = {-1, -1};
public:
	Grid(int, int);

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
