#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/**
 * \file mainwindow.h
 * \brief Заголовочный файл главного окна приложения
 */

#include <QMainWindow>
#include <QVector>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "scrabblegame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * \class MainWindow
 * \brief Главное окно игры Scrabble
 * \details Обеспечивает графический интерфейс для игры Scrabble:
 *          отображение игрового поля, ввод слов, отображение информации об игроках.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * \brief Конструктор главного окна
     * \param parent Родительский виджет (по умолчанию nullptr)
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * \brief Деструктор
     * \details Освобождает память, выделенную под объект игры
     */
    ~MainWindow();

private slots:
    /**
     * \brief Слот для кнопки "New Game"
     * \details Запрашивает подтверждение у пользователя и запускает новую игру
     */
    void onNewGame();

    /**
     * \brief Слот для кнопки "Place Word"
     * \details Обрабатывает размещение слова на поле
     * \sa onCellClicked()
     */
    void onMakeMove();

    /**
     * \brief Слот для кнопки "Pass Turn"
     * \details Пропускает ход текущего игрока
     */
    void onPassTurn();

    /**
     * \brief Слот для кнопки "Exchange Tiles"
     * \details Обменивает все буквы на руке текущего игрока
     */
    void onExchangeTiles();

    /**
     * \brief Слот для кнопки "Find Anagrams"
     * \details Находит и отображает все возможные слова из букв текущего игрока
     */
    void onFindAnagrams();

    /**
     * \brief Слот для клика по клетке поля
     * \details Обрабатывает выбор клетки для размещения слова
     */
    void onCellClicked();

private:
    Ui::MainWindow *ui;                     /**< Указатель на UI-компоненты (созданные Qt Designer) */
    ScrabbleGame* game;                     /**< Указатель на объект логики игры */
    QVector<QPushButton*> cellButtons;      /**< Вектор кнопок, представляющих клетки игрового поля (15x15) */
    QLabel* playerLabels[4];                /**< Массив меток с именами игроков (макс. 4 игрока) */
    QLabel* scoreLabels[4];                 /**< Массив меток со счётами игроков */
    QLabel* rackLabels[4];                  /**< Массив меток с буквами на руке игроков */
    QLineEdit* wordInput;                   /**< Поле ввода для слова */
    QPushButton* moveButton;                /**< Кнопка "Place Word" */
    QPushButton* passButton;                /**< Кнопка "Pass Turn" */
    QPushButton* exchangeButton;            /**< Кнопка "Exchange Tiles" */
    QPushButton* anagramButton;             /**< Кнопка "Find Anagrams" */
    QPushButton* newGameButton;             /**< Кнопка "New Game" */
    QLabel* currentPlayerLabel;             /**< Метка с именем текущего игрока */
    int selectedRow;                        /**< Выбранная строка на поле (-1 если ничего не выбрано) */
    int selectedCol;                        /**< Выбранный столбец на поле (-1 если ничего не выбрано) */
    bool horizontalMode;                    /**< true - слово размещается горизонтально, false - вертикально */

    /**
     * \brief Обновляет отображение игрового поля
     * \details Синхронизирует цвета и текст кнопок с состоянием игрового поля
     */
    void updateBoard();

    /**
     * \brief Обновляет информацию об игроках (счёт, буквы на руке)
     * \details Также выделяет жирным текущего игрока
     */
    void updatePlayersInfo();

    /**
     * \brief Создаёт UI игрового поля (15x15 кнопок)
     * \details Каждая кнопка подключается к слоту onCellClicked()
     */
    void createBoardUI();

    /**
     * \brief Показывает диалог окончания игры и победителя
     * \details Предлагает начать новую игру
     */
    void showGameOver();

    /**
     * \brief Настраивает новую игру
     * \details Запрашивает количество игроков, их имена, инициализирует поле и раздаёт буквы
     */
    void setupNewGame();
};

#endif