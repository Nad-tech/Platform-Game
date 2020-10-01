#pragma once

#include<windows.h>
#include<vector>
#include<\Users\dexte\source\repos\new projects\Platform Game\Platform Game\GUI_Headers\globals.h>

using namespace std;

class ConsoleWindow {
public:
	INPUT_RECORD *eventBuffer;

	ConsoleWindow() {}
	
	ConsoleWindow(const char* title, short screenWidth, short screenHeight, int fontWidth, int fontHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		this->fontWidth = fontWidth;
		this->fontHeight = fontHeight;
		characters = vector<short>(screenWidth * screenHeight);
		colours = vector<short>(screenWidth * screenHeight);

		//Window size dimensions
		windowSize = { 0, 0, screenWidth - 1, screenHeight - 1 };

		//Screen buffer dimensions
		bufferSize = { screenWidth , screenHeight };

		// Setting up different variables for passing to WriteConsoleOutput
		characterBufferSize = { screenWidth, screenHeight };
		characterPosition = { 0, 0 };
		consoleWriteArea = { 0, 0, screenWidth - 1, screenHeight - 1 };

		//A CHAR_INFO structure containing data about a single character 
		consoleBuffer = new CHAR_INFO[screenWidth * screenHeight];

		//Initialise handles
		h_out = GetStdHandle(STD_OUTPUT_HANDLE);
		h_in = GetStdHandle(STD_INPUT_HANDLE);

		//Set console Title
		size_t size = strlen(title) + 1;
		wchar_t* w_title = new wchar_t[size];
		size_t outSize;
		mbstowcs_s(&outSize, w_title, size, title, size - 1);
		SetConsoleTitle(w_title);

		//Set window size
		SetConsoleWindowInfo(h_out, true, &windowSize);

		//Set screen buffer size
		SetConsoleScreenBufferSize(h_out, bufferSize);

		//Set font attributes
		font.cbSize = sizeof(font);
		font.nFont = 0;
		font.dwFontSize.X = fontWidth;
		font.dwFontSize.Y = fontHeight;
		font.FontFamily = FF_DONTCARE;
		font.FontWeight = FW_NORMAL;
		wcscpy_s(font.FaceName, L"Consolas");
		SetCurrentConsoleFontEx(h_out, false, &font);

		//Console cursor visability
		GetConsoleCursorInfo(h_out, &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(h_out, &cursorInfo);

		//Turn off mouse select (quick edit select) but enable mouse input
		GetConsoleMode(h_in, &prev_mode);
		SetConsoleMode(h_in, prev_mode & ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

		//Full screen mode variables and setter
		//COORD fullScreen = GetLargestConsoleWindowSize(h_out);
		//PCOORD fulScreen = &fullScreen;
		//SetConsoleDisplayMode(h_out, CONSOLE_FULLSCREEN_MODE, fulScreen);

		//Set default background
		clearScreen(' ', FG_BLACK | BG_DARK_GREY);
	}

	void clearScreen(short character, short colour) {
		for (int i = 0; i < screenWidth * screenHeight; i++) {
			consoleBuffer[i].Attributes = colour;
			consoleBuffer[i].Char.UnicodeChar = character;
			colours[i] = colour;
			characters[i] = character;
		}
	}

	void upDateWindow() {
		WriteConsoleOutput(h_out, consoleBuffer, characterBufferSize, characterPosition, &consoleWriteArea);
	}

	void setPixel(int idx, short chr, int col) {
		consoleBuffer[idx].Attributes = col;
		consoleBuffer[idx].Char.UnicodeChar = chr;
		characters[idx] = chr;
		colours[idx] = col;
	}

	DWORD getInput() {
		DWORD numEvents = 0;
		DWORD numEventsRead = 0;

		GetNumberOfConsoleInputEvents(h_in, &numEvents);

		if (numEvents) {
			eventBuffer = new INPUT_RECORD[sizeof(INPUT_RECORD) * numEvents];
			ReadConsoleInput(h_in, eventBuffer, numEvents, &numEventsRead);
		}

		return numEventsRead;
	}

	void setCursorPos(short x, short y, bool b) {
		SetConsoleCursorPosition(h_out, { x, y });
	}

	short getCharAt(int i) { return characters[i]; }
	short getColourAt(int i) { return colours[i]; }

private:
	short screenWidth;
	short screenHeight;
	int fontWidth;
	int fontHeight;
	SMALL_RECT windowSize;
	COORD bufferSize;
	COORD characterBufferSize;
	COORD characterPosition;
	SMALL_RECT consoleWriteArea;
	CHAR_INFO* consoleBuffer;
	CONSOLE_FONT_INFOEX font;
	CONSOLE_CURSOR_INFO cursorInfo;
	DWORD prev_mode;
	DWORD numEventsRead;
	HANDLE h_out;
	HANDLE h_in;
	vector<short> characters;
	vector<short> colours;
};


