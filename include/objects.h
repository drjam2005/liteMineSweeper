#include <raylib.h>

enum STATE {
	REVEALED,
	HIDDEN,
	FLAGGED
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
};

class Interface {
public:
	Grid* grid = nullptr;
	Interface(int sideCount, int squareWidth) {
		grid = new Grid(sideCount, squareWidth);
	};
};
