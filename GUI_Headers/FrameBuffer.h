#pragma once

#include<\Users\dexte\source\repos\new projects\Platform Game\Platform Game\GUI_Headers\globals.h>
#include<vector>

using namespace std;

class FrameBuffer {
public:
	bool active;
	bool behindFrame_CharactersAndColoursSaved;
	vector<short> colours;
	vector<short> characters;
	vector<short> behindFrame_Characters;
	vector<short> behindFrame_Colours;
	vector<short> cellType;
	int width;
	int height;
	int x;
	int y;
	int size;

	FrameBuffer(int w = 10, int h = 10, int x = 0, int y = 0) {
		active = false;
		behindFrame_CharactersAndColoursSaved = false;
		width = w;
		height = h;
		size = width * height;
		this->x = x;
		this->y = y;
		colours = vector<short>(w * h);
		characters = vector<short>(w * h);
		behindFrame_Characters = vector<short>(w * h);
		behindFrame_Colours = vector<short>(w * h);
		cellType = vector<short>(w * h);

		//Default frame buffer contents
		for (int i = 0; i < width * height; i++) {
			colours[i] = FG_WHITE | BG_BLACK;
			characters[i] = 'X';
			behindFrame_Colours[i] = FG_WHITE | BG_BLACK;
			behindFrame_Characters[i] = 'X';
		}
	}

	void setChar(int i, int c) {
		characters[i] = c;
	}

	void setColour(int i, int c) {
		colours[i] = c;
	}

	void setRow(int row, int min, int max, short chr, short col) {
		for (int i = row * width + min; i <= row * width + max; i++) {
			setChar(i, chr);
			setColour(i, col);
		}
	}

	void setColumn(int clm, int min, int max, short chr, short col) {
		for (int i = min; i < max; i++) {
			setChar(i * width + clm, chr);
			setColour(i * width + clm, col);
		}
	}

	void setCellChrCol(int x, int y, short chr, short col) {
		setChar(y * width + x, chr);
		setColour(y * width + x, col);
	}

	void setCellChrCol(int x, short chr, short col) {
		setChar(x, chr);
		setColour(x, col);
	}

	void setOutSideBorders(int borderType, short col) {
		if (borderType == 0) {
			setRow(0, 1, width - 2, D_HOR, col);
			setRow(height - 1, 1, width - 2, D_HOR, col);
			setColumn(0, 1, height - 1, D_VERT, col);
			setColumn
			(width - 1, 1, height - 1, D_VERT, col);
			setCellChrCol(0, 0, D_TL_COR, col);//TL
			setCellChrCol(width - 1, 0, D_TR_COR, col);//TR
			setCellChrCol(0, height - 1, D_BL_COR, col);//BL
			setCellChrCol(width - 1, height - 1, D_BR_COR, col);//BR
		}

		if (borderType == 1) {
			setRow(0, 1, width - 2, HOR, col);
			setRow(height - 1, 1, width - 2, HOR, col);
			setColumn(0, 1, height - 1, VERT, col);
			setColumn
			(width - 1, 1, height - 1, VERT, col);
			setCellChrCol(0, 0, TL_COR, col);//TL
			setCellChrCol(width - 1, 0, TR_COR, col);//TR
			setCellChrCol(0, height - 1, BL_COR, col);//BL
			setCellChrCol(width - 1, height - 1, BR_COR, col);//BR
		}
	}

	void setAll(short chr, short col) {
		for (int i = 0; i < size; i++) {
			characters[i] = chr;
			colours[i] = col;
		}
	}

	void setSquareChrCol(int xmin, int xmax, int ymin, int ymax, short chr, short col) {
		for (int x = xmin; x <= xmax; x++) {
			for (int y = ymin; y <= ymax; y++) {
				setChar(y * width + x, chr);
				setColour(y * width + x, col);
			}
		}
	}

	void setRowColour(int row, short colour) {
		for (int i = 0; i < width; i++) {
			setColour(row * width + i, colour);
		}
	}
};