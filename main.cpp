#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <random>
#include <conio.h>


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

int scores[2] = {0, 0};
int activePlayer = 0;
int currentScore = 0;
int lastDiceOne = 0;
int lastDiceTwo = 0;
int targetScore = 0;
bool run = true;

HANDLE handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
string diceASCII;

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

        diceFaces.append(string(15, ' '))
                .append(firstTempLine)
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
        cout << endl << string(33, ' ') << "Please set a target score: ";
        cin >> targetScore;
    } else {
        cout << "Menu file could not be opened.";
    }
}

void displayRules() {
    ifstream ruleFile = openInputFile("assets/rules.txt");
    string rules, tempLine;

    if (ruleFile.good()) {
        while (getline(ruleFile, tempLine)) {
            rules.append(string(15, ' '))
                    .append(tempLine)
                    .append(string(15, ' '))
                    .append("\n");
        }
    } else {
        cout << "Rule file could not be opened.";
    }

    cout << rules;
}

void displayUI(string diceASCII, HANDLE &handleOut) {
    char buffOne[100];
    char buffTwo[100];
    sprintf(buffOne, "Global Score:  %d\n\n\t\t\t\t\tRound Score:  %d\n", scores[0], currentScore);
    sprintf(buffTwo, "Global Score:  %d\n\n", scores[1]);

    cout << diceASCII;

    if (activePlayer == 0) {
        SetConsoleTextAttribute(handleOut, FOREGROUND_RED);
    }
    cout << string(15, ' ') << "Player One\n\n";
    SetConsoleTextAttribute(handleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << string(15, ' ') << buffOne;
    if (activePlayer == 1) {
        SetConsoleTextAttribute(handleOut, FOREGROUND_RED);
    }
    cout << string(15, ' ') << "Player Two\n\n";
    SetConsoleTextAttribute(handleOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    cout << string(15, ' ') << buffTwo;
}

void nextPlayer() {
    currentScore = 0;
    if (activePlayer == 0) {
        activePlayer = 1;
    } else {
        activePlayer = 0;
    }
}

void holdScore() {
    scores[activePlayer] += currentScore;
    if (scores[activePlayer] >= targetScore) {
        string winMessage = activePlayer == 0 ? "Player One Win!" : "Player Two Win!";
        cout << winMessage << endl;
        Sleep(3000);
        run = false;
    } else {
        nextPlayer();
        displayUI(diceASCII, handleOut);
    }
}


void play(ifstream &diceFile, HANDLE &handleOut) {
    int diceOne, diceTwo;

    diceOne = rollDice();
    diceTwo = rollDice();

    if ((diceOne == 6 && lastDiceOne == 6) && (diceTwo == 6 && lastDiceTwo == 6)) {
        scores[activePlayer] = 0;
        nextPlayer();
    } else if (diceOne == 1 || diceTwo == 1) {
        currentScore = 0;
        nextPlayer();
    } else {
        currentScore += diceOne + diceTwo;
    }

    diceASCII = getDiceAscii(diceFile, diceOne, diceTwo);

    displayUI(diceASCII, handleOut);

    lastDiceOne = diceOne;
    lastDiceTwo = diceTwo;

}

int main() {
    ifstream diceFile = openInputFile("assets/dices.txt");

    displayMenu();
    while (run) {
        if (_kbhit()) {
            switch (_getch()) {
                case 's':
                    system("CLS");
                    displayRules();
                    break;
                case 'r':
                    system("CLS");
                    play(diceFile, handleOut);
                    break;
                case 'h':
                    system("CLS");
                    holdScore();
                    break;
                case 'q':
                    system("CLS");
                    run = false;
            }
        }

    }

    return 0;
}