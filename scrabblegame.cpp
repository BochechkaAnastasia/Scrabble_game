#include "scrabblegame.h"
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>

Tile::Tile(char l, int b) : letter(l), bonusType(b), used(false) {}

Player::Player(const std::string& n) : name(n), score(0), passCount(0) {}

ScrabbleGame::ScrabbleGame() : currentPlayer(0), gameOver(false), movesWithoutPass(0), firstMoveMade(false) {
    initBoard();
    initLetterValues();
    initLetterBag();
    loadDictionary();
    shuffleBag();
    srand(time(NULL));
}

void ScrabbleGame::initBoard() {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            board[i][j] = Tile(' ', 0);
        }
    }

    board[7][7] = Tile(' ', 5);

    board[0][0] = Tile(' ', 3); board[0][7] = Tile(' ', 3); board[0][14] = Tile(' ', 3);
    board[7][0] = Tile(' ', 3); board[7][14] = Tile(' ', 3);
    board[14][0] = Tile(' ', 3); board[14][7] = Tile(' ', 3); board[14][14] = Tile(' ', 3);

    board[1][5] = Tile(' ', 4); board[1][9] = Tile(' ', 4);
    board[5][1] = Tile(' ', 4); board[5][5] = Tile(' ', 4);
    board[5][9] = Tile(' ', 4); board[5][13] = Tile(' ', 4);
    board[9][1] = Tile(' ', 4); board[9][5] = Tile(' ', 4);
    board[9][9] = Tile(' ', 4); board[9][13] = Tile(' ', 4);
    board[13][5] = Tile(' ', 4); board[13][9] = Tile(' ', 4);

    board[1][1] = Tile(' ', 1); board[1][13] = Tile(' ', 1);
    board[2][2] = Tile(' ', 1); board[2][12] = Tile(' ', 1);
    board[3][3] = Tile(' ', 1); board[3][11] = Tile(' ', 1);
    board[4][4] = Tile(' ', 1); board[4][10] = Tile(' ', 1);
    board[10][4] = Tile(' ', 1); board[10][10] = Tile(' ', 1);
    board[11][3] = Tile(' ', 1); board[11][11] = Tile(' ', 1);
    board[12][2] = Tile(' ', 1); board[12][12] = Tile(' ', 1);
    board[13][1] = Tile(' ', 1); board[13][13] = Tile(' ', 1);

    board[0][3] = Tile(' ', 2); board[0][11] = Tile(' ', 2);
    board[2][6] = Tile(' ', 2); board[2][8] = Tile(' ', 2);
    board[3][0] = Tile(' ', 2); board[3][7] = Tile(' ', 2);
    board[3][14] = Tile(' ', 2); board[6][2] = Tile(' ', 2);
    board[6][6] = Tile(' ', 2); board[6][8] = Tile(' ', 2);
    board[6][12] = Tile(' ', 2); board[7][3] = Tile(' ', 2);
    board[7][11] = Tile(' ', 2); board[8][2] = Tile(' ', 2);
    board[8][6] = Tile(' ', 2); board[8][8] = Tile(' ', 2);
    board[8][12] = Tile(' ', 2); board[11][0] = Tile(' ', 2);
    board[11][7] = Tile(' ', 2); board[11][14] = Tile(' ', 2);
    board[12][6] = Tile(' ', 2); board[12][8] = Tile(' ', 2);
    board[14][3] = Tile(' ', 2); board[14][11] = Tile(' ', 2);
}

void ScrabbleGame::initLetterValues() {
    letterValues['A'] = 1; letterValues['B'] = 3; letterValues['C'] = 3;
    letterValues['D'] = 2; letterValues['E'] = 1; letterValues['F'] = 4;
    letterValues['G'] = 2; letterValues['H'] = 4; letterValues['I'] = 1;
    letterValues['J'] = 8; letterValues['K'] = 5; letterValues['L'] = 1;
    letterValues['M'] = 3; letterValues['N'] = 1; letterValues['O'] = 1;
    letterValues['P'] = 3; letterValues['Q'] = 10; letterValues['R'] = 1;
    letterValues['S'] = 1; letterValues['T'] = 1; letterValues['U'] = 1;
    letterValues['V'] = 4; letterValues['W'] = 4; letterValues['X'] = 8;
    letterValues['Y'] = 4; letterValues['Z'] = 10;
}

void ScrabbleGame::initLetterBag() {
    std::string letters =
        "AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIIIJJKKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ";
    for(char c : letters) {
        letterBag.push_back(c);
    }
}

void ScrabbleGame::loadDictionary() {
    std::ifstream file("C:/dictionary.txt");
    if(file.is_open()) {
        std::string word;
        while(std::getline(file, word)) {
            if(word.length() >= 2) {
                for(int i = 0; i < word.length(); i++) {
                    word[i] = toupper(word[i]);
                }
                dictionary.push_back(word);
                dictionarySet.insert(word);
            }
        }
        file.close();
    }
}

void ScrabbleGame::shuffleBag() {
    for(int i = 0; i < letterBag.size(); i++) {
        int j = rand() % letterBag.size();
        char temp = letterBag[i];
        letterBag[i] = letterBag[j];
        letterBag[j] = temp;
    }
}

char ScrabbleGame::getRandomLetter() {
    if(letterBag.empty()) return ' ';
    int index = rand() % letterBag.size();
    char letter = letterBag[index];
    letterBag.erase(letterBag.begin() + index);
    return letter;
}

void ScrabbleGame::refillRack(Player& p) {
    while(p.rack.size() < 7 && !letterBag.empty()) {
        p.rack.push_back(getRandomLetter());
    }
}

int ScrabbleGame::getLetterValue(char c) {
    if(letterValues.find(c) != letterValues.end()) {
        return letterValues[c];
    }
    return 0;
}

void ScrabbleGame::addPlayer(const std::string& name) {
    players.push_back(Player(name));
}

void ScrabbleGame::startGame() {
    for(int i = 0; i < players.size(); i++) {
        for(int j = 0; j < 7; j++) {
            players[i].rack.push_back(getRandomLetter());
        }
    }
    currentPlayer = 0;
    firstMoveMade = false;
}

bool ScrabbleGame::isValidWord(const std::string& word) {
    if(word.length() < 2) return false;
    std::string upperWord = word;
    for(int i = 0; i < upperWord.size(); i++) {
        upperWord[i] = toupper(upperWord[i]);
    }
    return dictionarySet.find(upperWord) != dictionarySet.end();
}

std::string ScrabbleGame::getWordAtPosition(int row, int col, bool horizontal) {
    std::string word = "";
    if(horizontal) {
        int startCol = col;
        while(startCol > 0 && board[row][startCol - 1].letter != ' ') {
            startCol--;
        }
        int endCol = col;
        while(endCol < 14 && board[row][endCol + 1].letter != ' ') {
            endCol++;
        }
        for(int c = startCol; c <= endCol; c++) {
            if(board[row][c].letter != ' ') {
                word += board[row][c].letter;
            }
        }
    } else {
        int startRow = row;
        while(startRow > 0 && board[startRow - 1][col].letter != ' ') {
            startRow--;
        }
        int endRow = row;
        while(endRow < 14 && board[endRow + 1][col].letter != ' ') {
            endRow++;
        }
        for(int r = startRow; r <= endRow; r++) {
            if(board[r][col].letter != ' ') {
                word += board[r][col].letter;
            }
        }
    }
    return word;
}

bool ScrabbleGame::touchesCenterFirstMove(const std::string& word, int row, int col, bool horizontal) {
    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;
        if(r == 7 && c == 7) return true;
    }
    return false;
}

bool ScrabbleGame::areAllLettersOnRack(const Player& p, const std::string& word, int row, int col, bool horizontal) {
    std::vector<char> rackCopy = p.rack;

    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        if(board[r][c].letter == ' ') {
            char letter = word[i];
            bool found = false;
            for(int j = 0; j < rackCopy.size(); j++) {
                if(rackCopy[j] == letter) {
                    rackCopy.erase(rackCopy.begin() + j);
                    found = true;
                    break;
                }
            }
            if(!found) return false;
        }
    }
    return true;
}

bool ScrabbleGame::isConnectedToExistingWord(const std::string& word, int row, int col, bool horizontal, std::vector<std::pair<int,int>>& newPositions) {
    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        if(board[r][c].letter != ' ') return true;

        if(r > 0 && board[r-1][c].letter != ' ') return true;
        if(r < 14 && board[r+1][c].letter != ' ') return true;
        if(c > 0 && board[r][c-1].letter != ' ') return true;
        if(c < 14 && board[r][c+1].letter != ' ') return true;
    }
    return false;
}

bool ScrabbleGame::canPlaceWord(const std::string& word, int row, int col, bool horizontal, int playerId, std::vector<std::pair<int,int>>& newPositions) {
    if(row < 0 || row >= 15 || col < 0 || col >= 15) return false;
    if(horizontal && col + word.length() > 15) return false;
    if(!horizontal && row + word.length() > 15) return false;
    if(word.length() < 2) return false;

    newPositions.clear();
    bool usesExistingLetter = false;

    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        char boardLetter = board[r][c].letter;
        char wordLetter = word[i];

        if(boardLetter != ' ') {
            if(boardLetter != wordLetter) {
                return false;
            }
            usesExistingLetter = true;
        } else {
            newPositions.push_back({r, c});
        }
    }

    if(newPositions.empty()) return false;

    if(!firstMoveMade) {
        if(!touchesCenterFirstMove(word, row, col, horizontal)) {
            return false;
        }
        usesExistingLetter = true;
    } else {
        if(!usesExistingLetter) {
            return false;
        }
    }

    if(!areAllLettersOnRack(players[playerId], word, row, col, horizontal)) {
        return false;
    }

    return true;
}

bool ScrabbleGame::checkCrossWords(const std::string& word, int row, int col, bool horizontal, std::vector<std::string>& formedWords, std::vector<int>& wordScores) {
    std::set<std::string> uniqueWords;

    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        if(board[r][c].letter == ' ') {
            std::string crossWord = getWordAtPosition(r, c, !horizontal);
            if(crossWord.length() >= 2) {
                std::string fullWord = "";
                if(!horizontal) {
                    int startRow = r;
                    while(startRow > 0 && board[startRow - 1][c].letter != ' ') {
                        startRow--;
                    }
                    int endRow = r;
                    while(endRow < 14 && board[endRow + 1][c].letter != ' ') {
                        endRow++;
                    }
                    for(int rr = startRow; rr <= endRow; rr++) {
                        if(board[rr][c].letter != ' ') {
                            fullWord += board[rr][c].letter;
                        } else if(rr == r) {
                            fullWord += word[i];
                        }
                    }
                } else {
                    int startCol = c;
                    while(startCol > 0 && board[r][startCol - 1].letter != ' ') {
                        startCol--;
                    }
                    int endCol = c;
                    while(endCol < 14 && board[r][endCol + 1].letter != ' ') {
                        endCol++;
                    }
                    for(int cc = startCol; cc <= endCol; cc++) {
                        if(board[r][cc].letter != ' ') {
                            fullWord += board[r][cc].letter;
                        } else if(cc == c) {
                            fullWord += word[i];
                        }
                    }
                }

                if(fullWord.length() >= 2 && uniqueWords.find(fullWord) == uniqueWords.end()) {
                    uniqueWords.insert(fullWord);
                    formedWords.push_back(fullWord);

                    int crossScore = 0;
                    int wordMult = 1;
                    for(int j = 0; j < fullWord.length(); j++) {
                        int rr = r;
                        int cc = c;
                        if(!horizontal) {
                            rr = r - (fullWord.length() - 1 - j);
                        } else {
                            cc = c - (fullWord.length() - 1 - j);
                        }
                        char letter = fullWord[j];
                        int letterScore = getLetterValue(letter);
                        int bonus = board[rr][cc].bonusType;
                        if(bonus == 1) letterScore *= 2;
                        if(bonus == 2) letterScore *= 3;
                        if(bonus == 3) wordMult *= 2;
                        if(bonus == 4) wordMult *= 3;
                        crossScore += letterScore;
                    }
                    wordScores.push_back(crossScore * wordMult);
                }
            }
        }
    }

    return true;
}

int ScrabbleGame::calculateWordScore(const std::string& word, int row, int col, bool horizontal, std::vector<std::pair<int,int>>& newPositions) {
    int wordMultiplier = 1;
    int totalScore = 0;

    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        char letter = word[i];
        int letterScore = getLetterValue(letter);
        int bonus = board[r][c].bonusType;

        if(bonus == 1) letterScore = letterScore * 2;
        if(bonus == 2) letterScore = letterScore * 3;
        if(bonus == 3) wordMultiplier = wordMultiplier * 2;
        if(bonus == 4) wordMultiplier = wordMultiplier * 3;

        totalScore += letterScore;
    }

    totalScore = totalScore * wordMultiplier;

    std::vector<std::string> crossWords;
    std::vector<int> crossScores;
    checkCrossWords(word, row, col, horizontal, crossWords, crossScores);

    for(int i = 0; i < crossScores.size(); i++) {
        totalScore += crossScores[i];
    }

    if(word.length() == 7) {
        totalScore += 50;
    }

    return totalScore;
}

void ScrabbleGame::removeLettersFromRack(Player& p, const std::string& word, int row, int col, bool horizontal) {
    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;

        if(board[r][c].letter == ' ') {
            char letter = word[i];
            for(int j = 0; j < p.rack.size(); j++) {
                if(p.rack[j] == letter) {
                    p.rack.erase(p.rack.begin() + j);
                    break;
                }
            }
        }
    }
}

void ScrabbleGame::placeWordOnBoard(const std::string& word, int row, int col, bool horizontal) {
    for(int i = 0; i < word.length(); i++) {
        int r = row;
        int c = col;
        if(horizontal) c = col + i;
        else r = row + i;
        board[r][c].letter = word[i];
        board[r][c].used = true;
    }
}

bool ScrabbleGame::makeMove(const std::string& word, int row, int col, bool horizontal) {
    if(gameOver) return false;

    std::string upperWord = word;
    for(int i = 0; i < upperWord.size(); i++) {
        upperWord[i] = toupper(upperWord[i]);
    }

    if(!isValidWord(upperWord)) return false;

    std::vector<std::pair<int,int>> newPositions;
    if(!canPlaceWord(upperWord, row, col, horizontal, currentPlayer, newPositions)) {
        return false;
    }

    int score = calculateWordScore(upperWord, row, col, horizontal, newPositions);
    players[currentPlayer].score += score;

    removeLettersFromRack(players[currentPlayer], upperWord, row, col, horizontal);
    placeWordOnBoard(upperWord, row, col, horizontal);
    refillRack(players[currentPlayer]);

    players[currentPlayer].passCount = 0;
    firstMoveMade = true;
    movesWithoutPass = 0;
    currentPlayer = (currentPlayer + 1) % players.size();

    if(letterBag.empty()) {
        int emptyRacks = 0;
        for(int i = 0; i < players.size(); i++) {
            if(players[i].rack.empty()) emptyRacks++;
        }
        if(emptyRacks > 0) gameOver = true;
    }

    return true;
}

void ScrabbleGame::passTurn() {
    if(gameOver) return;
    players[currentPlayer].passCount++;
    currentPlayer = (currentPlayer + 1) % players.size();

    int allPassed = 0;
    for(int i = 0; i < players.size(); i++) {
        if(players[i].passCount >= players.size()) allPassed++;
    }
    if(allPassed >= players.size()) gameOver = true;
}

void ScrabbleGame::exchangeTiles() {
    if(gameOver) return;

    Player& p = players[currentPlayer];
    if(p.rack.empty()) return;

    for(int i = 0; i < p.rack.size(); i++) {
        letterBag.push_back(p.rack[i]);
    }
    p.rack.clear();
    shuffleBag();
    refillRack(p);

    players[currentPlayer].passCount = 0;
    currentPlayer = (currentPlayer + 1) % players.size();
}

bool ScrabbleGame::isGameOver() {
    return gameOver;
}

std::string ScrabbleGame::getWinner() {
    int maxScore = -1;
    std::string winner = "";
    for(int i = 0; i < players.size(); i++) {
        if(players[i].score > maxScore) {
            maxScore = players[i].score;
            winner = players[i].name;
        }
    }
    return winner;
}

char ScrabbleGame::getBoardLetter(int row, int col) {
    return board[row][col].letter;
}

int ScrabbleGame::getBoardBonus(int row, int col) {
    return board[row][col].bonusType;
}

int ScrabbleGame::getCurrentPlayerIndex() {
    return currentPlayer;
}

Player& ScrabbleGame::getPlayer(int index) {
    return players[index];
}

int ScrabbleGame::getPlayerCount() {
    return players.size();
}

bool ScrabbleGame::isFirstMove() {
    return !firstMoveMade;
}

std::vector<std::string> ScrabbleGame::findAnagrams(int playerIndex) {
    std::vector<std::string> anagrams;
    std::vector<char> letters = players[playerIndex].rack;

    int letterCount[26] = {0};
    for(int i = 0; i < letters.size(); i++) {
        if(letters[i] >= 'A' && letters[i] <= 'Z') {
            letterCount[letters[i] - 'A']++;
        }
    }

    for(int i = 0; i < dictionary.size(); i++) {
        std::string word = dictionary[i];
        if(word.length() <= letters.size()) {
            int tempCount[26] = {0};
            bool valid = true;
            for(int j = 0; j < word.length(); j++) {
                tempCount[word[j] - 'A']++;
                if(tempCount[word[j] - 'A'] > letterCount[word[j] - 'A']) {
                    valid = false;
                    break;
                }
            }
            if(valid) {
                anagrams.push_back(word);
            }
        }
    }

    return anagrams;
}