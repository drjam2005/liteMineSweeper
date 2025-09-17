#include <objects.h>
#include <iostream>

Grid::Grid(int sideCount, int squareWidth){
	this->squareWidth = squareWidth;
	this->sideCount = sideCount;
	squares = new Square*[sideCount];
	for(int i = 0; i < sideCount; ++i){
		squares[i] = new Square[sideCount];
	}

	for(int y = 0; y < sideCount; ++y){
		for(int x = 0; x < sideCount; ++x){
			squares[x][y].indexPosition = {(float)x, (float)y};
		}
	}
}

void Grid::DrawGrid(){
	for(int y = 0; y < sideCount; ++y){
		for(int x = 0; x < sideCount; ++x){
			Color color = WHITE;
			if(squares[x][y].state == REVEALED)
				color = GREEN;
			if(squares[x][y].state == FLAGGED)
				color = RED;
			DrawRectangle(15+(x*squareWidth)+2*x, 25+(y*squareWidth)+2*y, squareWidth, squareWidth, color);
		}
	}
}


void Grid::UpdateGrid(){
	Vector2 mPos = GetMousePosition();
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		for(int y = 0; y < sideCount; ++y){
			for(int x = 0; x < sideCount; ++x){
				if(mPos.x > (float)15+x*squareWidth+2*x 
				&& mPos.x < (float)15+x*squareWidth+2*x+squareWidth
				&& mPos.y > (float)25+y*squareWidth+2*y
				&& mPos.y < (float)25+y*squareWidth+2*y+squareWidth){
					selected = Vector2{(float)x, (float)y};
					if(squares[x][y].state == HIDDEN)
						squares[x][y].state = REVEALED;
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
