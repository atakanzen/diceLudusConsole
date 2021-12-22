//#include <ncurses.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <random>


using namespace std;

const map<int, int> diceLineMap = {{1, 0},
                                   {2, 96},
                                   {3, 192},
                                   {4, 288},
                                   {5, 384},
                                   {6, 480}};


const map<int, int> byteCountMap = {{0, 0},
                                    {1, 30},
                                    {2, 42},
                                    {3, 54},
                                    {4, 66},
                                    {5, 95}};

const int diceRangeFrom = 1;
const int diceRangeTo = 6;

// Returns an int in range from 1 to 6. Using uniform_int_distribution since the
// rand() does not generate numbers uniformly.
int rollDice() {
    std::random_device rand_device;
    std::mt19937 generator(rand_device());
    std::uniform_int_distribution<int> distribution(diceRangeFrom, diceRangeTo);

    return distribution(generator);
}

// Returns the line number of the specified dice by calculating the byte count of each line.
int getLineNumber(int diceLineMapKey, int byteCountMapKey) {
    return diceLineMap.at(diceLineMapKey) + byteCountMap.at(byteCountMapKey);
}

// Returns the ASCII art of the specified dice by using sprite technique.
string getDiceAscii(ifstream &file, int firstDice, int secondDice) {
    int diceOneNextLine = 0, diceTwoNextLine = 0;
    string firstTempLine, secondTempLine, diceFaces;

    for (int i = 0; i < 6; ++i) {
        diceOneNextLine = getLineNumber(firstDice, i);
        diceTwoNextLine = getLineNumber(secondDice, i);

        file.seekg(diceOneNextLine);
        getline(file, firstTempLine);
        file.seekg(diceTwoNextLine);
        getline(file, secondTempLine);

        diceFaces.append(firstTempLine)
                .append(string(5, ' '))
                .append(secondTempLine)
                .append("\n");
    }

    return diceFaces;
}

ifstream openInputFile(string filePath) {
    ifstream file(filePath, ios::in);
    return file;
};

void displayMenu() {
    ifstream menuFile = openInputFile("assets/menu.txt");
    string tempLine, menu;
    if (menuFile.good()) {
        while (getline(menuFile, tempLine)) {
            menu.append(string(15, ' '))
                    .append(tempLine)
                    .append(string(15, ' '))
                    .append("\n");
        }

        cout << menu;
    } else {
        cout << "";
    }
}

void play() {
    int firstDice, secondDice;
    ifstream diceFile = openInputFile("assets/dices.txt");

    if (diceFile.good()) {
        firstDice = rollDice();
        secondDice = rollDice();

        string diceASCII = getDiceAscii(diceFile, firstDice, secondDice);

    } else {
        cout << "Could not open dice sprite map." << endl;
        diceFile.close();
    }

    diceFile.close();
}

int main() {
    displayMenu();

    if (GetKeyState('S')) {
        cout << "BABA UYYUORUM";
        Sleep(1000);
        system("CLS");
        play();
    } else if (GetKeyState('Q')) {
        return 0;
    }
    return 0;
}