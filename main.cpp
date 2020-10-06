#include<\Users\dexte\source\repos\new stuff\PlatformGameV2\PlatformGameV2\ConsoleWindow.h>
#include<vector>
#include<fstream>
#include<chrono>
#include<string>

ConsoleWindow cWindow;
bool quit = false;
auto tp1 = chrono::system_clock::now();
auto tp2 = chrono::system_clock::now();

bool spaceBarPressed = false;

struct Frame {
	int width;
	int height;
	int size;
	vector<short> chrs;
	vector<short> cols;
};

Frame world;

int sceneWidth;
int sceneHeight;
int sceneSize;

int cameraWidth;
int cameraHeight;
float cameraX = 0;
int cameraXvel = 0;
int cameraScrPosX = 1;
int cameraScrPosY = 1;

Frame player;
int playerWidth;
int playerHeight;
float playerX;
float playerY;
int playerXvel = 0;
int playerYvel = 0;
float velocityIncrement = 0.3;

Frame enemy;
float enemyX;
float enemyY;
int enemyHeight;

void displayPlayer() {
	int si = (int)playerY * SCREEN_WIDTH + (int)playerX;
	
	for (int i = 0; i < player.size; i++) {
		if(player.chrs[i] != 'A') cWindow.setPixel(si, player.chrs[i], player.cols[i]);
		if (i % player.width == player.width - 1) {
			si += SCREEN_WIDTH - player.width;
		}
		si++;
	}
}

void displayCameraView() {
	int wi = (int)cameraX;
	int sx = 0;
	int sy = 0;
	
	for (int i = 0; i < sceneSize; i++) {
		cWindow.setPixel((sy + cameraScrPosY)*SCREEN_WIDTH + (sx + cameraScrPosX), world.chrs[wi], world.cols[wi]);
		sx++;
		wi++;
		if (i % cameraWidth == cameraWidth - 1) {
			wi += world.width - cameraWidth;
			sx = 0;
			sy++;
		}
	}
}

Frame loadPlayerFile(string fileName) {
	Frame player;
	ifstream inFile;
	inFile.open("data/" + fileName + ".dan");

	if (!inFile) {
		string s = "Cant open " + fileName;
		for (int i = 0; i < s.length(); i++) {
			cWindow.setPixel(i, s[i], BG_RED | FG_WHITE);
		}
		cWindow.upDateWindow();
		return player;
	}

	short width;
	short height;

	inFile >> width >> height;
	player.width = width;
	player.height = height;
	player.size = width * height;

	short chr;
	short col;
	while (inFile >> chr >> col) {
		player.chrs.push_back(chr);
		col == (BG_BLACK | FG_BLACK) ? col = (BG_BLUE | FG_GREEN) : col;
		player.cols.push_back(col);
	}

	return player;
}

Frame loadSceneFile(string fileName) {
	Frame scene;
	ifstream inFile;
	inFile.open("data/" + fileName + ".dan");

	if (!inFile) {
		string s = "Cant open " + fileName;
		for (int i = 0; i < s.length(); i++) {
			cWindow.setPixel(i, s[i], BG_RED | FG_WHITE);
		}
		cWindow.upDateWindow();
		return scene;
	}

	short width;
	short height;

	inFile >> width >> height;

	sceneWidth = width;
	sceneHeight = height;
	sceneSize = width * height;

	scene.width = width;
	scene.height = height;
	scene.size = width * height;

	cameraWidth = scene.width;
	cameraHeight = scene.height;
	
	short chr;
	short col;
	while (inFile >> chr >> col) {
		scene.chrs.push_back(chr);
		scene.cols.push_back(col);
	}

	inFile.close();

	return scene;
}

void buildWorldFromScenes() {
	vector<Frame> scenes;
	scenes.push_back(loadSceneFile("BG1"));
	scenes.push_back(loadSceneFile("BG2"));
	scenes.push_back(loadSceneFile("BG3"));
	scenes.push_back(loadSceneFile("BG4"));

	world.height = sceneHeight;
	world.width = sceneWidth * scenes.size();
	world.size = world.width * world.height;
	world.chrs = vector<short>(world.size);
	world.cols = vector<short>(world.size);
	
	int ww = world.width;
	int sw = scenes[0].width;

	for (int j = 0; j < 4; j++) {
		for (int x = 0; x < scenes[j].width; x++) {
			for (int y = 0; y < scenes[j].height; y++) {
				world.chrs[y * ww + j * sw + x] = scenes[j].chrs[y*sw + x];
				world.cols[y * ww + j * sw + x] = scenes[j].cols[y*sw + x];
			}
		}
	}
}

struct Bullet {
	float x = 0;
	float y = 0;
	int velIncrement = 200;
	int col = BG_YELLOW | FG_RED;
	int chr = QUARTER;
	bool active = false;
	int travelDistance = 0;
};
Bullet bullet;

void shoot() {
	if (!bullet.active) {
		bullet.active = true;
		bullet.x = playerX + 10;
		bullet.y = playerY + 3;
	}
}

void moveAndDisplayBullet(float elapsedTime) {
	if (bullet.active) {
		cWindow.setPixel(int(bullet.y) * SCREEN_WIDTH + int(bullet.x), bullet.chr, bullet.col);
		bullet.x += bullet.velIncrement * elapsedTime;
		bullet.travelDistance++;
		if (bullet.x > cameraWidth) {
			bullet.active = false;
			bullet.travelDistance = 0;
		}
	}
}

void moveAndDisplayEnemy(float elapsedTime) {
	enemyX -= (rand() % 50 + 10) * elapsedTime;
	if (enemyX < 1) {
		enemyX = cameraWidth - enemy.width;
		enemyY = rand() % (cameraHeight - enemyHeight) + 1;
	}

	int si = (int)enemyY * SCREEN_WIDTH + (int)enemyX;

	for (int i = 0; i < enemy.size; i++) {
		if (enemy.cols[i] != BG_WHITE && enemyX < cameraWidth) cWindow.setPixel(si, enemy.chrs[i], enemy.cols[i]);
		if (i % enemy.width == enemy.width - 1) {
			si += SCREEN_WIDTH - enemy.width;
		}
		si++;
	}
}

int main() {
	cWindow = ConsoleWindow(
		"Dans Platform Game", SCREEN_WIDTH, SCREEN_HEIGHT, FONT_WIDTH, FONT_HEIGHT
	);
	
	buildWorldFromScenes();

	player = loadPlayerFile("SHIPWITHALPHA");
	playerX = 10;
	playerY = 10;

	enemy = loadPlayerFile("ROBOT");
	enemyX = cameraWidth - enemy.width;
	enemyY = 5;
	enemyHeight = enemy.height;

	std::thread cameraThread;
	std::thread windowThread;
	std::thread playerThread;

	while (!quit) {
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) quit = true;

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			playerXvel++;
		}
		else {
			if(playerXvel > 0) playerXvel--;
			if(cameraXvel > 0) cameraXvel--;
		}
			
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			playerXvel--;
		}
		else {
			if (playerXvel < 0) playerXvel++;
			if (cameraXvel < 0) cameraXvel++;
		}
		
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			playerYvel--;
		}
		else {
			if (playerYvel < 0) playerYvel++;
		}

		if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			playerYvel++;
		}
		else{
			if (playerYvel > 0) playerYvel--;
		}

		playerX += playerXvel * velocityIncrement * fElapsedTime;
		playerY += playerYvel * velocityIncrement * fElapsedTime;
		
		cameraX += 150 * velocityIncrement * fElapsedTime;
			

		if (cameraX + cameraWidth > world.width) {
			cameraX = 0;
		}

		int screenOffset = 10;
		if (playerX > (cameraScrPosX + cameraWidth) - player.width - screenOffset) {
			playerX = (cameraScrPosX + cameraWidth) - player.width - screenOffset;
			playerXvel = 0;
		}

		if (playerX < cameraScrPosX + screenOffset) {
			playerX = cameraScrPosX + screenOffset;
			playerXvel = 0;
		}
		
		if (playerY > (cameraScrPosY + cameraHeight) - player.height + 1) {
			playerY = (cameraScrPosY + cameraHeight) - player.height;
			playerYvel = 0;
		}

		if (playerY < cameraScrPosY) {
			playerY = cameraScrPosY;
			playerYvel = 0;
		}
		
		displayCameraView();

		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			shoot();
		}
		
		moveAndDisplayEnemy(fElapsedTime);
		moveAndDisplayBullet(fElapsedTime);
		displayPlayer();
		cWindow.upDateWindow();

		if (
			(int)bullet.x > (int)enemyX && (int)bullet.x < (int)enemyX + enemy.width &&
			((int)bullet.y > (int)enemyY && (int)bullet.y < (int)enemyY + enemy.height)
			) 
		{
			cameraXvel++;
			enemyX = cameraWidth - enemy.width;
			enemyY = rand() % (cameraHeight - enemyHeight) + 1;
		}
	}
}