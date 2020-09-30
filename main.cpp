#include<\Users\dexte\source\repos\new stuff\Platform_Game\Source_Files\GUI_Headers\ConsoleWindow.h>
#include<\Users\dexte\source\repos\new stuff\Platform_Game\Source_Files\GUI_Headers\FrameBuffer.h>
#include<string>
#include<fstream>
#include<iostream>
#include<chrono>

ConsoleWindow cWindow;
FrameBuffer camera = FrameBuffer(40, 30, 1, 0);
FrameBuffer world;
ifstream inFile;
float cameraX = 0;
float cameraY = 0;
float playerX = 5;
float playerY = (float)camera.height / 2;
float speed = 30;

FrameBuffer player = FrameBuffer(10, 5, (int)playerX, (int)playerY);
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();

	

void setPixel(int x, int y, short chr, short col, FrameBuffer &f) {
	f.setCellChrCol(y*f.width + x, chr, col);
}

void updateCamera() {
	int worldIdx = (int)cameraX;
	for (int i = 0; i < camera.size; i++) {
		camera.setCellChrCol(i, world.characters[worldIdx], world.colours[worldIdx]);
		if (i % camera.width == camera.width - 1) {
			worldIdx += world.width - camera.width;
		}
		worldIdx++;
	}
}
	
void writeCamera() {
	int x = 0;
	int y = 0;
	for (int i = 0; i < camera.size; i++) {
		x++;
		if (i % camera.width == 0) {
			x = 0;
			y++;
		}
	
		cWindow.setPixel(
			(y + camera.y)*SCREEN_WIDTH + (x + camera.x),
			camera.characters[i], camera.colours[i]
		);
	}
}

void writePlayer() {
	player.x = (int)playerX;
	player.y = (int)playerY;
	
	int x = 0;
	int y = 0;
	for (int i = 0; i < player.size; i++) {
		x++;
		if (i % player.width == 0) {
			x = 0;
			y++;
		}

		cWindow.setPixel(
			(y + player.y)*SCREEN_WIDTH + (x + player.x),
			player.characters[i], player.colours[i]
		);
	}
}

FrameBuffer loadFrameBuffer(string fileName) {
	FrameBuffer frameBuffer;
	
	string directory = "data/";
	string fileExtension = ".dan";

	inFile.open(directory + fileName + fileExtension);

	vector<short> characters;
	vector<short> colours;
	short chr;
	short col;
	int width, height;

	inFile >> width >> height;

	while (inFile >> chr >> col) {
		characters.push_back(chr);
		colours.push_back(col);
	}

	inFile.close();

	frameBuffer = FrameBuffer(width, height, 0, 0);

	for (int i = 0; i < int(characters.size()); i++) {
		frameBuffer.characters[i] = characters[i];
		frameBuffer.colours[i] = colours[i];
	}

	return frameBuffer;
}

int main() {
	cWindow = ConsoleWindow(
		"Dans Platform Game", SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT
	);

	FrameBuffer scr_1 = loadFrameBuffer("BG1");
	FrameBuffer scr_2 = loadFrameBuffer("BG2");
	FrameBuffer scr_3 = loadFrameBuffer("BG3");
	FrameBuffer scr_4 = loadFrameBuffer("BG4");

	int worldWidth = 4 * scr_1.width;
	int worldHeight = scr_1.height;

	world = FrameBuffer(worldWidth, worldHeight, 0, 0);

	for (int x = 0; x < scr_1.width; x++) {
		for (int y = 0; y < scr_1.height; y++) {
			setPixel(x, y, scr_1.characters[y*scr_1.width + x], scr_1.colours[y*scr_1.width + x], world);
			setPixel(x + scr_1.width, y, scr_2.characters[y*scr_2.width + x], scr_2.colours[y*scr_1.width + x], world);
			setPixel(x + 2 * scr_1.width, y, scr_3.characters[y*scr_1.width + x], scr_3.colours[y*scr_1.width + x], world);
			setPixel(x + 3 * scr_1.width, y, scr_4.characters[y*scr_1.width + x], scr_4.colours[y*scr_1.width + x], world);
		}
	}
	
	bool quit = false;
		
	while (!quit) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) quit = true;


		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (player.x + player.width < camera.width) playerX += speed * fElapsedTime;
			if (player.x > camera.x + camera.width / 2) {
				if (cameraX < 4 * scr_1.width - camera.width) {
					cameraX += speed * fElapsedTime;
				}
			}
		}

		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (player.x > camera.x + 1) playerX -= speed * fElapsedTime;
			if (playerX + player.width < camera.x + camera.width / 2) {
				if (cameraX > 0) {
					cameraX -= speed * fElapsedTime;
				}
			}
		}
		
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (player.y > camera.y + 1) playerY -= speed * fElapsedTime;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			if (player.y + player.height < camera.y + camera.height - 1) playerY += speed * fElapsedTime;
		}

		updateCamera();
		writeCamera();
		writePlayer();
		cWindow.upDateWindow();
	}
	return 0;
}