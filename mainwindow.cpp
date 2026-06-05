#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
    , game(nullptr)
    , selectedRow(-1)
    , selectedCol(-1)
    , horizontalMode(true)
{
    setWindowTitle("Scrabble Game");
    setMinimumSize(900, 700);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout* topLayout = new QHBoxLayout();
    currentPlayerLabel = new QLabel("Current Player: ");
    topLayout->addWidget(currentPlayerLabel);

    newGameButton = new QPushButton("New Game");
    topLayout->addWidget(newGameButton);

    mainLayout->addLayout(topLayout);

    createBoardUI();
    mainLayout->addLayout(mainLayout->itemAt(1)->layout());

    QGridLayout* playerInfoLayout = new QGridLayout();
    for(int i = 0; i < 4; i++) {
        playerLabels[i] = new QLabel("Player " + QString::number(i+1) + ": ");
        scoreLabels[i] = new QLabel("Score: 0");
        rackLabels[i] = new QLabel("Rack: ");
        playerInfoLayout->addWidget(playerLabels[i], i, 0);
        playerInfoLayout->addWidget(scoreLabels[i], i, 1);
        playerInfoLayout->addWidget(rackLabels[i], i, 2);
    }
    mainLayout->addLayout(playerInfoLayout);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    wordInput = new QLineEdit();
    wordInput->setPlaceholderText("Enter word...");
    bottomLayout->addWidget(wordInput);

    moveButton = new QPushButton("Place Word");
    bottomLayout->addWidget(moveButton);

    passButton = new QPushButton("Pass Turn");
    bottomLayout->addWidget(passButton);

    exchangeButton = new QPushButton("Exchange Tiles");
    bottomLayout->addWidget(exchangeButton);

    anagramButton = new QPushButton("Find Anagrams");
    bottomLayout->addWidget(anagramButton);

    mainLayout->addLayout(bottomLayout);

    connect(newGameButton, &QPushButton::clicked, this, &MainWindow::onNewGame);
    connect(moveButton, &QPushButton::clicked, this, &MainWindow::onMakeMove);
    connect(passButton, &QPushButton::clicked, this, &MainWindow::onPassTurn);
    connect(exchangeButton, &QPushButton::clicked, this, &MainWindow::onExchangeTiles);
    connect(anagramButton, &QPushButton::clicked, this, &MainWindow::onFindAnagrams);

    setupNewGame();
}

MainWindow::~MainWindow() {
    if(game) delete game;
}

void MainWindow::createBoardUI() {
    QWidget* boardWidget = new QWidget();
    QGridLayout* boardLayout = new QGridLayout(boardWidget);
    boardLayout->setSpacing(1);

    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            QPushButton* btn = new QPushButton(" ");
            btn->setFixedSize(45, 45);
            btn->setProperty("row", i);
            btn->setProperty("col", j);
            connect(btn, &QPushButton::clicked, this, &MainWindow::onCellClicked);
            boardLayout->addWidget(btn, i, j);
            cellButtons.push_back(btn);
        }
    }

    centralWidget()->layout()->addWidget(boardWidget);
}

void MainWindow::setupNewGame() {
    if(game) delete game;
    game = new ScrabbleGame();

    bool ok;
    int playerCount = QInputDialog::getInt(this, "Number of Players",
                                           "Enter number of players (2-4):",
                                           2, 2, 4, 1, &ok);
    if(!ok) playerCount = 2;

    for(int i = 0; i < playerCount; i++) {
        QString name = QInputDialog::getText(this, "Player Name",
                                             "Enter name for player " + QString::number(i+1) + ":",
                                             QLineEdit::Normal, "Player " + QString::number(i+1), &ok);
        if(!ok || name.isEmpty()) name = "Player " + QString::number(i+1);
        game->addPlayer(name.toStdString());
        playerLabels[i]->setText(name + ": ");
        playerLabels[i]->setVisible(true);
        scoreLabels[i]->setVisible(true);
        rackLabels[i]->setVisible(true);
    }

    for(int i = playerCount; i < 4; i++) {
        playerLabels[i]->setVisible(false);
        scoreLabels[i]->setVisible(false);
        rackLabels[i]->setVisible(false);
    }

    game->startGame();
    updateBoard();
    updatePlayersInfo();
    selectedRow = -1;
    selectedCol = -1;
    horizontalMode = true;
}

void MainWindow::updateBoard() {
    for(int i = 0; i < 15; i++) {
        for(int j = 0; j < 15; j++) {
            int index = i * 15 + j;
            char letter = game->getBoardLetter(i, j);
            int bonus = game->getBoardBonus(i, j);

            if(letter != ' ') {
                cellButtons[index]->setText(QString(letter));
                cellButtons[index]->setStyleSheet("background-color: #90EE90; font-size: 18px; font-weight: bold;");
            } else {
                cellButtons[index]->setText("");
                QString color;
                if(bonus == 1) color = "#ADD8E6";
                else if(bonus == 2) color = "#00008B";
                else if(bonus == 3) color = "#FFB6C1";
                else if(bonus == 4) color = "#FF6347";
                else if(bonus == 5) color = "#FFD700";
                else color = "#F5DEB3";
                cellButtons[index]->setStyleSheet("background-color: " + color + "; font-size: 12px;");
            }
        }
    }
}

void MainWindow::updatePlayersInfo() {
    int currentIdx = game->getCurrentPlayerIndex();
    for(int i = 0; i < game->getPlayerCount(); i++) {
        Player& p = game->getPlayer(i);
        scoreLabels[i]->setText("Score: " + QString::number(p.score));

        QString rackText = "Rack: ";
        for(char c : p.rack) {
            rackText += QChar(c);
            rackText += " ";
        }
        rackLabels[i]->setText(rackText);

        if(i == currentIdx) {
            playerLabels[i]->setStyleSheet("font-weight: bold; color: blue;");
            scoreLabels[i]->setStyleSheet("font-weight: bold; color: blue;");
            rackLabels[i]->setStyleSheet("font-weight: bold; color: blue;");
        } else {
            playerLabels[i]->setStyleSheet("");
            scoreLabels[i]->setStyleSheet("");
            rackLabels[i]->setStyleSheet("");
        }
    }
    if(game->getPlayerCount() > 0) {
        currentPlayerLabel->setText("Current Player: " +
                                    QString::fromStdString(game->getPlayer(currentIdx).name));
    }
}

void MainWindow::onCellClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    if(selectedRow == -1 && selectedCol == -1) {
        selectedRow = row;
        selectedCol = col;
        btn->setStyleSheet(btn->styleSheet() + "border: 3px solid red;");
    } else if(selectedRow == row && selectedCol == col) {
        horizontalMode = !horizontalMode;
        int index = row * 15 + col;
        cellButtons[index]->setStyleSheet(cellButtons[index]->styleSheet() +
                                          "border: 3px solid " + QString(horizontalMode ? "blue" : "green") + ";");
    } else {
        int oldIndex = selectedRow * 15 + selectedCol;
        QString oldStyle = cellButtons[oldIndex]->styleSheet();
        oldStyle.replace("border: 3px solid red;", "");
        oldStyle.replace("border: 3px solid blue;", "");
        oldStyle.replace("border: 3px solid green;", "");
        cellButtons[oldIndex]->setStyleSheet(oldStyle);

        selectedRow = row;
        selectedCol = col;
        btn->setStyleSheet(btn->styleSheet() + "border: 3px solid red;");
    }
}

void MainWindow::onMakeMove() {
    if(selectedRow == -1 || selectedCol == -1) {
        QMessageBox::warning(this, "Error", "Please select a cell first!");
        return;
    }

    QString word = wordInput->text().trimmed().toUpper();
    if(word.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a word!");
        return;
    }

    if(game->makeMove(word.toStdString(), selectedRow, selectedCol, horizontalMode)) {
        wordInput->clear();
        selectedRow = -1;
        selectedCol = -1;
        updateBoard();
        updatePlayersInfo();

        if(game->isGameOver()) {
            showGameOver();
        }
    } else {
        QString message = "Invalid move! Check:\n";
        if(game->isFirstMove()) {
            message += "- First word must go through center (row 7, col 7)\n";
        }
        message += "- Word must be at least 2 letters\n";
        message += "- Word must be in dictionary\n";
        message += "- You must have all new letters on your rack\n";
        message += "- Word must connect to existing letters";
        QMessageBox::warning(this, "Invalid Move", message);
    }
}

void MainWindow::onPassTurn() {
    game->passTurn();
    updatePlayersInfo();
    selectedRow = -1;
    selectedCol = -1;
    wordInput->clear();

    if(game->isGameOver()) {
        showGameOver();
    }
}

void MainWindow::onExchangeTiles() {
    game->exchangeTiles();
    updateBoard();
    updatePlayersInfo();
    selectedRow = -1;
    selectedCol = -1;
    wordInput->clear();
}

void MainWindow::onFindAnagrams() {
    if(game->getPlayerCount() == 0) return;

    int currentIdx = game->getCurrentPlayerIndex();
    std::vector<std::string> anagrams = game->findAnagrams(currentIdx);

    if(anagrams.empty()) {
        QMessageBox::information(this, "Anagrams", "No anagrams found with your letters!");
    } else {
        QString message = "Possible words from your letters (" + QString::number(anagrams.size()) + " found):\n\n";
        int count = 0;
        for(int i = 0; i < anagrams.size() && count < 100; i++) {
            message += QString::fromStdString(anagrams[i]) + " ";
            count++;
            if(count % 8 == 0) message += "\n";
        }
        if(anagrams.size() > 100) {
            message += "\n... and " + QString::number(anagrams.size() - 100) + " more";
        }
        QMessageBox::information(this, "Anagrams", message);
    }
}

void MainWindow::showGameOver() {
    std::string winner = game->getWinner();
    int result = QMessageBox::question(this, "Game Over",
                                       "Game Over!\nWinner is: " + QString::fromStdString(winner) + "\n\nPlay again?",
                                       QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {
        setupNewGame();
    }
}

void MainWindow::onNewGame() {
    int result = QMessageBox::question(this, "New Game",
                                       "Start a new game? Current progress will be lost.",
                                       QMessageBox::Yes | QMessageBox::No);

    if(result == QMessageBox::Yes) {
        setupNewGame();
    }
}