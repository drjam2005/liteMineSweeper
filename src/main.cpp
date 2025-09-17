#include <iostream>
#include <objects.h>
#include <raylib.h>

int main(int argc, char **argv) {
	int side = 15;
	int width = 25;
	if(argc == 3){
		side = std::atoi(argv[1]);
		width = std::atoi(argv[2]);
	}

	int windowWidth  = side*width+(2*side)+30;
	int windowHeight = side*width+(2*side)+50;
	Interface ui(side, width);

	InitWindow(windowWidth, windowHeight, "Minesooper");
	while(!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(BLACK);

		ui.grid->DrawGrid();
		ui.grid->UpdateGrid();

		DrawFPS(20, 20);
		EndDrawing();
	}
    return 0;
}
