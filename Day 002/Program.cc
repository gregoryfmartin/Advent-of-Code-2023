#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <regex>
#include <algorithm>

/*
Regexes to parse lines:

Game ID: \d+:
Game Results: (\d+\s+\w+);? (The result that contains the semicolon terminates a result set)
*/

enum class MarbleColor : uint8_t {
    Red,
    Green,
    Blue
};

struct MarbleResult {
    MarbleColor drawColor;
    uint8_t     drawQuantity;

    static auto CreateFromString(const std::string& initStr) noexcept -> MarbleResult {
        std::string a = initStr;
        std::string b = initStr.substr(0, a.find(" "));                     // Draw Quantity
        std::string c = initStr.substr(a.find(" ") + 1, std::string::npos); // Draw Color
        MarbleResult t;

        t.drawQuantity = (uint8_t)std::stoi(b);

        if(c.find(";") != std::string::npos) {
            c.pop_back();
        }
        if(c.compare("red") == 0) {
            t.drawColor = MarbleColor::Red;
        } else if(c.compare("green") == 0) {
            t.drawColor = MarbleColor::Green;
        } else if(c.compare("blue") == 0) {
            t.drawColor = MarbleColor::Blue;
        }

        return t;
    }

    auto GetColorString() const noexcept -> std::string {
        switch(drawColor) {
            case MarbleColor::Red:
                return "Red";
            case MarbleColor::Green:
                return "Green";
            case MarbleColor::Blue:
                return "Blue";
        }

        return "???";
    }

    auto CompareDrawQuantities(const MarbleResult& result) const noexcept -> bool {
        if(drawQuantity <= result.drawQuantity) {
            return true;
        }

        return false;
    }

    auto operator<(const MarbleResult& result) const noexcept -> bool {
        int acVal = 0;
        int bcVal = 0;

        for(auto& c : GetColorString()) {
            acVal += (int)c;
        }
        for(auto& d : result.GetColorString()) {
            bcVal += (int)d;
        }

        return acVal < bcVal;
    }
};

using MarbleResults = std::vector<MarbleResult>;
MarbleResults modelResults {
    { MarbleColor::Red, 12 },
    { MarbleColor::Blue, 14 },
    { MarbleColor::Green, 13 }
};

auto IsMarbleResultPossible(const MarbleResults& result) noexcept -> bool {
    return (result[0].drawQuantity <= modelResults[0].drawQuantity) &&
           (result[1].drawQuantity <= modelResults[1].drawQuantity) &&
           (result[2].drawQuantity <= modelResults[2].drawQuantity);
}

struct Game {
    uint8_t                    id;
    std::vector<MarbleResults> results;

    static auto CreateFromString(const std::vector<std::string>& initStr) noexcept -> Game {
        // Elm 0 = Game ID
        // Elm 1-N = Results
        Game a;
        MarbleResults b;
        auto fwdItr = initStr.begin();

        a.id = std::atoi((*fwdItr).c_str());
        fwdItr++;

        for(fwdItr; fwdItr != initStr.end(); fwdItr++) {
            std::string z = *fwdItr;

            if(z.find(";") == std::string::npos && fwdItr != initStr.end() - 1) {
                // This is not the end of the result set, push it into the temporary results
                b.push_back(MarbleResult::CreateFromString(z));
            } else if(z.find(";") != std::string::npos || fwdItr == initStr.end() - 1) {
                // This is the end of the result set, push the val, then push it into the results vector
                b.push_back(MarbleResult::CreateFromString(z));

                // Check to see if we're missing a red draw (set to zero if it isn't present)
                auto redFind = std::find_if(b.begin(), b.end(), [](const MarbleResult& mr) {
                    return mr.drawColor == MarbleColor::Red;
                });
                auto greenFind = std::find_if(b.begin(), b.end(), [](const MarbleResult& mr) {
                    return mr.drawColor == MarbleColor::Green;
                });
                auto blueFind = std::find_if(b.begin(), b.end(), [](const MarbleResult& mr) {
                    return mr.drawColor == MarbleColor::Blue;
                });

                if(redFind == b.end()) {
                    // Red is missing, add it
                    b.push_back({MarbleColor::Red, 0});
                }
                if(greenFind == b.end()) {
                    // Green is missing, add it
                    b.push_back({MarbleColor::Green, 0});
                }
                if(blueFind == b.end()) {
                    // Blue is missing, add it
                    b.push_back({MarbleColor::Blue, 0});
                }

                std::sort(b.begin(), b.end()); // Ensures Red, Blue, Green ordering

                a.results.push_back(b);
                b.clear(); // Clear the vector for future use
            }
        }

        return a;
    }
};

struct GameCoordinator {
    std::vector<Game> games;

    auto PopulateGameDataFromFile() noexcept -> void {
        std::cout << "🏔❄Snow Island❄🏔" << std::endl;
        std::cout << "Populating data..." << std::endl;

        std::ifstream gamesData("./DataSource.txt");
        std::string line;
        std::regex gameIdRegex("\\d+:");
        std::regex gameResultsRegex("(\\d+\\s+\\w+);?");
        std::regex_token_iterator<std::string::iterator> itrEnd;

        while(std::getline(gamesData, line)) {
            std::smatch sm;
            std::regex_search(line, sm, gameIdRegex); // This should give the game ID
            std::regex_token_iterator<std::string::iterator> fwdItr(line.begin(), line.end(), gameResultsRegex); // This iterator should have the game results
            std::vector<std::string> fileLineData;

            fileLineData.push_back(sm[0].str().substr(0, sm[0].str().find(":")));
            while(fwdItr != itrEnd) {
                fileLineData.push_back(*fwdItr);
                fwdItr++;
            }

            games.push_back(Game::CreateFromString(fileLineData));
        }

        std::cout << "DUN!" << std::endl;
    }

    auto IsGamePossibleGivenModel(const Game& playedGame) const noexcept -> bool {
        std::vector<bool> possibleGames;

        for(auto& result : playedGame.results) {
            possibleGames.push_back(IsMarbleResultPossible(result));
        }

        auto findRes = std::find(possibleGames.begin(), possibleGames.end(), false);
        if(findRes == possibleGames.end()) {
            return true;
        }

        return false;
    }

    auto SolveThatPuzzlePat() const noexcept -> void {
        std::cout << "I'd like to solve that puzzle, Pat!" << std::endl;
        std::cout << "Checking my gambling results... 💰🤑" << std::endl;

        int idSum = 0;

        for(auto& game : games) {
            if(IsGamePossibleGivenModel(game)) {
                idSum += game.id;
            }
        }

        std::cout << "I believe the answer is " << idSum << "! It better be right, or I'm going to be pissed! 👿💢😡😠" << std::endl;
    }
};

int main() {
    GameCoordinator gameCoordinator;
    gameCoordinator.PopulateGameDataFromFile();
    gameCoordinator.SolveThatPuzzlePat();

    return 0;
}
