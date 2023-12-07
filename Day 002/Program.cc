#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

/*
Regexes to parse lines:

Game ID: \d+:
Game Results: (\d+\s+\w+);? (The result that contains the semicolon terminates a result set)
*/

enum class MarbleColor {
    Red,
    Green,
    Blue
};

struct MarbleResult {
    MarbleColor drawColor;
    uint8_t     drawQuantity;
};

using MarbleResults = std::vector<MarbleResult>;
MarbleResults modelResults {
    { MarbleColor::Red, 12 },
    { MarbleColor::Green, 13 },
    { MarbleColor::Blue, 14 }
};

struct Game {
    uint8_t                    id;
    std::vector<MarbleResults> results;
};

struct GameCoordinator {
    std::vector<Game> games;

    auto IsGamePossibleGivenModel(const Game& playedGame) noexcept -> bool {
        return false;
    }
};

int main() {
    std::ifstream gamesData("./DataSource.txt");
    std::string line;
    std::regex gameId("\\d+:");
    std::regex gameResults("(\\d+\\s+\\w+);?");

    while(std::getline(gamesData, line)) {
        std::cout << std::regex_match(line, gameId) << std::endl;
    }
    // MarbleResult a {
    //     MarbleColor::Red,
    //     5
    // };
    // MarbleResults b {
    //     { MarbleColor::Green, 10 },
    //     { MarbleColor::Blue, 7 }
    // };

    // for(auto& z : b) {
    //     std::cout << "Marble Color: " <<
    //         (int)z.drawColor <<
    //         ", Draw Quantity: " <<
    //         (int)z.drawQuantity << std::endl;
    // }

    return 0;
}
