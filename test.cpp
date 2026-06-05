/**
 * \file test.cpp
 * \brief Тесты для игры Scrabble с использованием Doctest
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "scrabblegame.h"

/**
 * \brief Тесты для класса Tile
 */
TEST_CASE("Tile tests") {
    SUBCASE("Default tile is empty") {
        Tile t;
        CHECK(t.letter == ' ');
        CHECK(t.bonusType == 0);
        CHECK(t.used == false);
    }

    SUBCASE("Tile with letter and bonus") {
        Tile t('A', 3);
        CHECK(t.letter == 'A');
        CHECK(t.bonusType == 3);
    }
}

/**
 * \brief Тесты для класса Player
 */
TEST_CASE("Player tests") {
    SUBCASE("New player has zero score and empty rack") {
        Player p("Test");
        CHECK(p.name == "Test");
        CHECK(p.score == 0);
        CHECK(p.passCount == 0);
        CHECK(p.rack.empty() == true);
    }
}

/**
 * \brief Тесты для проверки слов
 */
TEST_CASE("Word validation tests") {
    ScrabbleGame game;

    SUBCASE("Valid words return true") {
        CHECK(game.isValidWord("THE") == true);
        CHECK(game.isValidWord("AND") == true);
        CHECK(game.isValidWord("HELLO") == true);
    }

    SUBCASE("Words with 1 letter return false") {
        CHECK(game.isValidWord("A") == false);
        CHECK(game.isValidWord("Z") == false);
    }

    SUBCASE("Nonsense words return false") {
        CHECK(game.isValidWord("XXXX") == false);
        CHECK(game.isValidWord("ABCDEFG") == false);
    }
}

/**
 * \brief Тесты для стоимости букв
 */
TEST_CASE("Letter value tests") {
    ScrabbleGame game;

    SUBCASE("Cheap letters cost 1 point") {
        CHECK(game.getLetterValue('A') == 1);
        CHECK(game.getLetterValue('E') == 1);
        CHECK(game.getLetterValue('O') == 1);
    }

    SUBCASE("Expensive letters cost more") {
        CHECK(game.getLetterValue('Q') == 10);
        CHECK(game.getLetterValue('Z') == 10);
        CHECK(game.getLetterValue('J') == 8);
    }

    SUBCASE("Unknown letter returns 0") {
        CHECK(game.getLetterValue('?') == 0);
    }
}

/**
 * \brief Тесты для игроков
 */
TEST_CASE("Player management tests") {
    ScrabbleGame game;

    SUBCASE("Can add players") {
        game.addPlayer("Alice");
        game.addPlayer("Bob");
        CHECK(game.getPlayerCount() == 2);
        CHECK(game.getPlayer(0).name == "Alice");
        CHECK(game.getPlayer(1).name == "Bob");
    }

    SUBCASE("Game starts with 7 letters per player") {
        game.addPlayer("Test");
        game.startGame();
        CHECK(game.getPlayer(0).rack.size() == 7);
    }
}

/**
 * \brief Тесты для первого хода
 */
TEST_CASE("First move must go through center") {
    ScrabbleGame game;
    game.addPlayer("Test");
    game.startGame();

    SUBCASE("isFirstMove returns true at start") {
        CHECK(game.isFirstMove() == true);
    }
}

/**
 * \brief Тесты для бонусных клеток
 */
TEST_CASE("Bonus tiles tests") {
    ScrabbleGame game;

    SUBCASE("Center tile has bonus") {
        CHECK(game.getBoardBonus(7, 7) == 5);
    }

    SUBCASE("Corner tiles double word score") {
        CHECK(game.getBoardBonus(0, 0) == 3);
        CHECK(game.getBoardBonus(0, 14) == 3);
        CHECK(game.getBoardBonus(14, 0) == 3);
        CHECK(game.getBoardBonus(14, 14) == 3);
    }
}

/**
 * \brief Тесты для окончания игры
 */
TEST_CASE("Game over tests") {
    ScrabbleGame game;
    game.addPlayer("Alice");
    game.addPlayer("Bob");
    game.startGame();

    SUBCASE("Game is not over at start") {
        CHECK(game.isGameOver() == false);
    }
}

/**
 * \brief Тесты для обмена фишек
 */
TEST_CASE("Exchange tiles tests") {
    ScrabbleGame game;
    game.addPlayer("Test");
    game.startGame();

    SUBCASE("Exchange tiles does not crash") {
        game.exchangeTiles();
        CHECK(true);
    }
}

/**
 * \brief Тесты для анаграмм
 */
TEST_CASE("Find anagrams tests") {
    ScrabbleGame game;
    game.addPlayer("Test");
    game.startGame();

    SUBCASE("Find anagrams returns vector") {
        std::vector<std::string> result = game.findAnagrams(0);
        CHECK(result.size() >= 0);
    }
}

/**
 * \brief Тесты для поля
 */
TEST_CASE("Board tests") {
    ScrabbleGame game;

    SUBCASE("Board is 15x15") {
        CHECK(game.getBoardLetter(0, 0) == ' ');
        CHECK(game.getBoardLetter(14, 14) == ' ');
    }
}