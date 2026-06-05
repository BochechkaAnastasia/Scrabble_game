#ifndef SCRABBLEGAME_H
#define SCRABBLEGAME_H

/**
 * \file scrabblegame.h
 * \brief Заголовочный файл с классами для игры Scrabble
 */

#include <vector>
#include <string>
#include <map>
#include <unordered_set>
#include <random>

/**
 * \class Tile
 * \brief Представляет одну клетку на игровом поле
 * \details Содержит информацию о букве, типе бонуса и состоянии использования
 */
class Tile {
public:
    char letter;          /**< Буква в клетке (' ' если пусто) */
    int bonusType;        /**< Тип бонуса: 0-обычная, 1-удвоение буквы, 2-утроение буквы, 3-удвоение слова, 4-утроение слова, 5-центр */
    bool used;            /**< Использована ли клетка в текущем ходе */

    /**
     * \brief Конструктор клетки
     * \param l Буква (по умолчанию ' ')
     * \param b Тип бонуса (по умолчанию 0)
     */
    Tile(char l = ' ', int b = 0);
};

/**
 * \class Player
 * \brief Представляет игрока в игре Scrabble
 * \details Хранит имя, счёт, буквы на руке и счётчик пасов
 */
class Player {
public:
    std::string name;           /**< Имя игрока */
    int score;                  /**< Текущий счёт игрока */
    std::vector<char> rack;     /**< Буквы на руке (максимум 7) */
    int passCount;              /**< Счётчик последовательных пасов подряд */

    /**
     * \brief Конструктор игрока
     * \param n Имя игрока (по умолчанию пустая строка)
     */
    Player(const std::string& n = "");
};

/**
 * \class ScrabbleGame
 * \brief Основной класс игры Scrabble
 * \details Управляет игровым процессом: поле, игроки, словарь, проверка ходов, подсчёт очков
 * \warning Для работы требуется файл словаря по пути "C:/dictionary.txt"
 */
class ScrabbleGame {
private:
    Tile board[15][15];                     /**< Игровое поле 15x15 клеток */
    std::vector<Player> players;            /**< Список игроков в игре */
    int currentPlayer;                      /**< Индекс текущего игрока (0..n-1) */
    std::vector<char> letterBag;            /**< Мешок с оставшимися буквами */
    std::map<char, int> letterValues;       /**< Стоимость каждой буквы (A=1, B=3, ...) */
    std::vector<std::string> dictionary;    /**< Список допустимых слов из словаря */
    std::unordered_set<std::string> dictionarySet; /**< Хеш-таблица для быстрого поиска слов */
    bool gameOver;                          /**< Флаг окончания игры */
    int movesWithoutPass;                   /**< Счётчик ходов без паса (не используется) */
    bool firstMoveMade;                     /**< Был ли сделан первый ход в игре */

    /**
     * \brief Инициализирует игровое поле с бонусными клетками
     * \details Расставляет клетки с удвоением/утроением буквы/слова и центр
     */
    void initBoard();

    /**
     * \brief Инициализирует стоимость каждой буквы
     * \details A=1, B=3, C=3, D=2, E=1, F=4, G=2, H=4, I=1, J=8, K=5, L=1, M=3,
     *          N=1, O=1, P=3, Q=10, R=1, S=1, T=1, U=1, V=4, W=4, X=8, Y=4, Z=10
     */
    void initLetterValues();

    /**
     * \brief Инициализирует мешок с буквами
     * \details Наполняет letterBag стандартным набором букв для Scrabble
     */
    void initLetterBag();

    /**
     * \brief Загружает словарь из файла
     * \details Читает слова из "C:/dictionary.txt", приводит к верхнему регистру,
     *          добавляет только слова длиной >= 2 символов
     */
    void loadDictionary();

    /**
     * \brief Перемешивает буквы в мешке
     * \details Использует случайную перестановку для случайного выбора букв
     */
    void shuffleBag();

    /**
     * \brief Извлекает случайную букву из мешка
     * \return Случайная буква или ' ' если мешок пуст
     */
    char getRandomLetter();

    /**
     * \brief Пополняет руку игрока до 7 букв
     * \param p Игрок, чью руку нужно пополнить
     * \details Добавляет буквы из мешка, пока в руке меньше 7 и мешок не пуст
     */
    void refillRack(Player& p);

    /**
     * \brief Вычисляет очки за размещённое слово
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \param newPositions Вектор позиций новых букв (заполняется внутри)
     * \return Количество очков за ход
     */
    int calculateWordScore(const std::string& word, int row, int col, bool horizontal, std::vector<std::pair<int,int>>& newPositions);

    /**
     * \brief Проверяет возможность размещения слова
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \param playerId Индекс текущего игрока
     * \param newPositions Вектор позиций новых букв (заполняется внутри)
     * \return true если слово можно разместить
     */
    bool canPlaceWord(const std::string& word, int row, int col, bool horizontal, int playerId, std::vector<std::pair<int,int>>& newPositions);

    /**
     * \brief Проверяет, соединяется ли слово с существующими буквами на поле
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \param newPositions Вектор позиций новых букв
     * \return true если слово касается хотя бы одной существующей буквы
     */
    bool isConnectedToExistingWord(const std::string& word, int row, int col, bool horizontal, std::vector<std::pair<int,int>>& newPositions);

    /**
     * \brief Проверяет, проходит ли первый ход через центр поля
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \return true если хотя бы одна буква слова находится в центре (7,7)
     */
    bool touchesCenterFirstMove(const std::string& word, int row, int col, bool horizontal);

    /**
     * \brief Проверяет, есть ли все новые буквы на руке игрока
     * \param p Игрок
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \return true если все необходимые буквы есть на руке
     */
    bool areAllLettersOnRack(const Player& p, const std::string& word, int row, int col, bool horizontal);

    /**
     * \brief Проверяет все пересекающиеся слова (кросс-слова)
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     * \param formedWords Вектор сформированных слов (заполняется внутри)
     * \param wordScores Вектор очков за каждое слово (заполняется внутри)
     * \return true если все кросс-слова валидны
     */
    bool checkCrossWords(const std::string& word, int row, int col, bool horizontal, std::vector<std::string>& formedWords, std::vector<int>& wordScores);

    /**
     * \brief Возвращает слово, проходящее через указанную позицию
     * \param row Строка
     * \param col Столбец
     * \param horizontal true - горизонтальное слово, false - вертикальное
     * \return Слово, собранное из букв на поле
     */
    std::string getWordAtPosition(int row, int col, bool horizontal);

    /**
     * \brief Удаляет использованные буквы с руки игрока
     * \param p Игрок
     * \param word Размещённое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     */
    void removeLettersFromRack(Player& p, const std::string& word, int row, int col, bool horizontal);

    /**
     * \brief Размещает слово на игровом поле
     * \param word Размещаемое слово
     * \param row Строка начала слова
     * \param col Столбец начала слова
     * \param horizontal true - горизонтально, false - вертикально
     */
    void placeWordOnBoard(const std::string& word, int row, int col, bool horizontal);

public:
    /**
     * \brief Конструктор игры
     * \details Инициализирует поле, стоимость букв, мешок, загружает словарь,
     *          перемешивает буквы и инициализирует генератор случайных чисел
     */
    ScrabbleGame();

    /**
     * \brief Добавляет нового игрока
     * \param name Имя игрока
     */
    void addPlayer(const std::string& name);

    /**
     * \brief Начинает игру
     * \details Раздаёт каждому игроку по 7 букв, устанавливает текущего игрока
     */
    void startGame();

    /**
     * \brief Делает ход
     * \param word Слово для размещения (будет приведено к верхнему регистру)
     * \param row Строка первой буквы слова (0-14)
     * \param col Столбец первой буквы слова (0-14)
     * \param horizontal true - горизонтально, false - вертикально
     * \return true если ход успешен, false если ход невалиден
     */
    bool makeMove(const std::string& word, int row, int col, bool horizontal);

    /**
     * \brief Пропускает ход текущего игрока
     * \details Увеличивает счётчик пасов игрока, переключает ход.
     *          Если все игроки спасовали подряд, игра заканчивается.
     */
    void passTurn();

    /**
     * \brief Обменивает все буквы на руке
     * \details Возвращает буквы в мешок, перемешивает, выдаёт новые.
     *          Считается как ход, переключает игрока.
     */
    void exchangeTiles();

    /**
     * \brief Проверяет, окончена ли игра
     * \return true если игра окончена (победа или все спасовали)
     */
    bool isGameOver();

    /**
     * \brief Определяет победителя
     * \return Имя игрока с максимальным счётом
     */
    std::string getWinner();

    /**
     * \brief Возвращает букву на указанной клетке
     * \param row Строка (0-14)
     * \param col Столбец (0-14)
     * \return Буква или ' ' если клетка пуста
     */
    char getBoardLetter(int row, int col);

    /**
     * \brief Возвращает тип бонуса на указанной клетке
     * \param row Строка (0-14)
     * \param col Столбец (0-14)
     * \return Тип бонуса (0-5)
     */
    int getBoardBonus(int row, int col);

    /**
     * \brief Возвращает индекс текущего игрока
     * \return Индекс в массиве players (0..n-1)
     */
    int getCurrentPlayerIndex();

    /**
     * \brief Возвращает ссылку на игрока по индексу
     * \param index Индекс игрока (0..n-1)
     * \return Ссылка на объект Player
     */
    Player& getPlayer(int index);

    /**
     * \brief Возвращает количество игроков в игре
     * \return Количество игроков (2-4)
     */
    int getPlayerCount();

    /**
     * \brief Находит все возможные слова из букв игрока
     * \param playerIndex Индекс игрока
     * \return Вектор строк со всеми найденными словами
     */
    std::vector<std::string> findAnagrams(int playerIndex);

    /**
     * \brief Проверяет, был ли сделан первый ход
     * \return true если первый ход ещё не сделан
     */
    bool isFirstMove();

    /**
     * \brief Возвращает стоимость буквы
     * \param c Буква (A-Z)
     * \return Стоимость буквы (0 если буква неизвестна)
     */
    int getLetterValue(char c);

    /**
     * \brief Проверяет, есть ли слово в словаре
     * \param word Проверяемое слово
     * \return true если слово есть в словаре и его длина >= 2
     */
    bool isValidWord(const std::string& word);
};

#endif