#include <objects.h>
#include <string>
#include <random>
#include <iostream>

bool inVector(std::vector<Vector2>& vector, Vector2 pos){
	for(Vector2& vec : vector){
		if(pos.x == vec.x && pos.y == vec.y)
			return true;
	}

	return false;
}

Grid::Grid(int sideCount, int squareWidth){
	this->squareWidth = squareWidth;
	this->sideCount = sideCount;
	squares = new Square*[sideCount];
	for (int x = 0; x < sideCount; ++x) {
		squares[x] = new Square[sideCount];
	}

	for(int y = 0; y < sideCount; ++y){
		for(int x = 0; x < sideCount; ++x){
			squares[x][y].indexPosition = {(float)x, (float)y};
		}
	}

	srand(time(NULL));
	std::vector<Vector2> passed;
	for(int r = 0; r < 20; ++r){
		int x  = rand() % sideCount;
		int y  = rand() % sideCount;
		Vector2 pos  = {float(x), float(y)};
		if(inVector(passed, pos)) continue;

		passed.push_back(pos);
		squares[x][y].type = BOMB;
		UpdateGridInfo(Vector2{(float)x,(float)y}, ADD_BOMB);
	}
}

void Grid::DrawGrid(){
	for(int y = 0; y < sideCount; ++y){
		for(int x = 0; x < sideCount; ++x){
			Color color = WHITE;
			if(squares[x][y].state == FLAGGED){
				color = RED;
				DrawRectangle(15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, squareWidth, squareWidth, color);
			}
			if(squares[x][y].state == REVEALED){
				unsigned char red = 30*squares[x][y].closeBombs;
				color = {red, (unsigned char)(255 - red),0,255};
				DrawRectangle(15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, squareWidth, squareWidth, color);
				if(squares[x][y].type == BOMB){
					DrawRectangle(15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, squareWidth, squareWidth, RED);
					DrawText("X", 15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, 20, BLACK);
				}else if(squares[x][y].closeBombs > 0)
					DrawText(TextFormat("%s", std::to_string(squares[x][y].closeBombs).c_str()),15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, 20, BLACK);
			}else{
				DrawRectangle(15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, squareWidth, squareWidth, color);
			}
		}
	}
}

void Grid::UpdateGridInfo(Vector2 position, UPDATETYPE type) {
    if (type != ADD_BOMB) return;

    int x = int(position.x);
    int y = int(position.y);

    const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    const int dy[] = {-1, 0,  1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx < 0 || ny < 0 || nx >= sideCount || ny >= sideCount)
            continue;

        if (squares[nx][ny].type == BOMB) 
            continue;

        squares[nx][ny].closeBombs++;
    }
}



void Grid::UpdateGrid(){
	Vector2 mPos = GetMousePosition();
	if(IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON)){
		for(int y = 0; y < sideCount; ++y){
			for(int x = 0; x < sideCount; ++x){
				if(mPos.x > (float)15+x*squareWidth+2*x 
				&& mPos.x < (float)15+x*squareWidth+2*x+squareWidth
				&& mPos.y > (float)25+y*squareWidth+2*y
				&& mPos.y < (float)25+y*squareWidth+2*y+squareWidth){
					selected = Vector2{(float)x, (float)y};
					if(squares[x][y].type != BOMB){
						squares[x][y].type = BOMB;
						squares[x][y].closeBombs = 0;
						UpdateGridInfo(Vector2{(float)x,(float)y}, ADD_BOMB);
					}
					return;
				}
			}
		}
	}
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		for(int y = 0; y < sideCount; ++y){
			for(int x = 0; x < sideCount; ++x){
				if(mPos.x > (float)15+x*squareWidth+2*x 
				&& mPos.x < (float)15+x*squareWidth+2*x+squareWidth
				&& mPos.y > (float)25+y*squareWidth+2*y
				&& mPos.y < (float)25+y*squareWidth+2*y+squareWidth){
					selected = Vector2{(float)x, (float)y};
					if(squares[x][y].state == HIDDEN){
						squares[x][y].state = REVEALED;
						std::vector<Vector2> passed;
						RevealArea(Vector2{(float)x, (float)y}, passed); 
					}
					return;
				}
			}
		}
	}
	if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
		for(int y = 0; y < sideCount; ++y){
			for(int x = 0; x < sideCount; ++x){
				if(mPos.x > (float)15+x*squareWidth+2*x 
				&& mPos.x < (float)15+x*squareWidth+2*x+squareWidth
				&& mPos.y > (float)25+y*squareWidth+2*y
				&& mPos.y < (float)25+y*squareWidth+2*y+squareWidth){
					selected = Vector2{(float)x, (float)y};
					if(squares[x][y].state == HIDDEN){
						squares[x][y].state = FLAGGED;
						return;
					}
					if(squares[x][y].state == FLAGGED)
						squares[x][y].state = HIDDEN;
					return;
				}
			}
		}
	}

	if(selected.x != -1 && selected.y != -1){
		std::cout << "Selected square[" << selected.x << "][" << selected.y << "]" << std::endl;
		selected = {-1, -1};
	}
}


void Grid::RevealArea(Vector2 position, std::vector<Vector2>& passed){
	int x = position.x, y = position.y;
	if(x < 0) return;
	if(y < 0) return;
	if(x >= sideCount) return;
	if(y >= sideCount) return;
	if(squares[x][y].type == BOMB) return;
	if(inVector(passed, position)) return;
	squares[x][y].state = REVEALED;
	if(squares[x][y].closeBombs > 0){
		return;
	}

	passed.push_back(position);
	RevealArea(Vector2{(float)x-1, (float)y}, passed);
	RevealArea(Vector2{(float)x+1, (float)y}, passed);
	RevealArea(Vector2{(float)x, (float)y-1}, passed);
	RevealArea(Vector2{(float)x, (float)y+1}, passed);
	RevealArea(Vector2{(float)x-1, (float)y-1}, passed);
	RevealArea(Vector2{(float)x-1, (float)y+1}, passed);
	RevealArea(Vector2{(float)x+1, (float)y-1}, passed);
	RevealArea(Vector2{(float)x+1, (float)y+1}, passed);
}
