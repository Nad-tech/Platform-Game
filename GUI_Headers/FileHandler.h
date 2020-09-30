////File structure for a frame file (all data on first line of file):
////width height character colour character colour ....
//
//#pragma once
//
//#include<fstream>
//#include<vector>
//#include<string>
//#include<\Users\dexte\source\repos\new stuff\Sprite Creator Tool\Source_Files\GUI_Headers\FrameBuffer.h>
//
//using namespace std;
//
//class FileHandler {
//public:
//	FileHandler() {
//		
//		//Load all the exisitng file names in the directory.
//		//
//		//Use this later to check if to save filename already exist
//		fileNamesIn.open("data/frameBufferFileNamesList.dan");
//		std::string name;
//		while (fileNamesIn >> name) {
//			fileNames.push_back(name);
//		}
//		fileNamesIn.close();
//	}
//
//	//Return true if file name already exists
//	bool saveFrameBuffer(string fileName, FrameBuffer frameBuffer) {
//
//		string name = "data/" + fileName + ".dan";
//		
//		//Check if filename already exists
//		//Do nothing if filename found
//		bool found = false;
//		for (string fname : fileNames) {
//			if (fname == name) {
//				found = true;
//				break;
//			}
//		}
//
//		//Save the frame buffer
//		if (!found) {
//			//Update the file names list file
//			fileNamesOut.open("data/frameBufferFileNamesList.dan", std::ios_base::app);
//			fileNamesOut << name << ' ';
//			fileNamesOut.close();
//			outFile.open(name);
//
//			//Save the frame
//			outFile << frameBuffer.width << ' ' << frameBuffer.height << ' ';
//
//			for (int i = 0; i < int(frameBuffer.colours.size()); i++) {
//				outFile << frameBuffer.characters[i] << ' ' << frameBuffer.colours[i] << ' ';
//			}
//
//			outFile.close();
//		}
//
//		return found;
//	}
//
//	//Loads contents of file into supplied frame buffer
//	void loadFrameBuffer(string fileName, FrameBuffer &frameBuffer, int x, int y) {
//		string directory = "data/";
//		string fileExtension = ".dan";
//		
//		inFile.open(directory + fileName + fileExtension);
//
//		vector<short> characters;
//		vector<short> colours;
//		short chr;
//		short col;
//		int width, height;
//
//		inFile >> width >> height;
//
//		while (inFile >> chr >> col) {
//			characters.push_back(chr);
//			colours.push_back(col);
//		}
//
//		inFile.close();
//
//		frameBuffer = FrameBuffer(width, height, x, y);
//
//		for (int i = 0; i < int(characters.size()); i++) {
//			frameBuffer.characters[i] = characters[i];
//			frameBuffer.colours[i] = colours[i];
//		}
//	}
//
//	//return file names in the file names list file as a vector where each element is a name of a file
//	vector<string> getFileNames() {
//		fileNamesIn.open("data/frameBufferFileNamesList.dan");
//		vector<string> names;
//		string f;
//		while (fileNamesIn >> f) {
//			names.push_back(f);
//		}
//		return names;
//	}
//
//private:
//	ofstream outFile;
//	ifstream inFile;
//	ofstream fileNamesOut;
//	ifstream fileNamesIn;
//	vector<string> fileNames;
//};
