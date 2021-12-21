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

// Returns the ASCII art of the specified dice by using sprite technique.
string getDiceAscii(ifstream &file, int firstDice, int secondDice) {
    int diceOneNextPosition = 0, diceTwoNextPosition = 0;
    string firstTempLine, secondTempLine, diceFaces;

    for (int i = 0; i < 6; ++i) {
        diceOneNextPosition = diceLineMap.at(firstDice) + byteCountMap.at(i);
        diceTwoNextPosition = diceLineMap.at(secondDice) + byteCountMap.at(i);

        file.seekg(diceOneNextPosition);
        getline(file, firstTempLine);
        file.seekg(diceTwoNextPosition);
        getline(file, secondTempLine);

        diceFaces.append(firstTempLine)
                .append(string(5, ' '))
                .append(secondTempLine)
                .append("\n");
    }

    return diceFaces;
}

int main() {
    int firstDice, secondDice;
    ifstream diceFile("assets/dices.txt", ios::in);

    if (diceFile.good()) {
        firstDice = rollDice();
        secondDice = rollDice();

        string diceASCII = getDiceAscii(diceFile, firstDice, secondDice);
        cout << diceASCII;
    }

    diceFile.close();

    return 0;
}