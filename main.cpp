#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

struct PlayerInfo {
    int playerID;
    char playerMark;
    int victories = 0;
    int draws = 0;
    int losses = 0;
    int points = 0;
};

void LoadPlayerData(std::vector<PlayerInfo>& players) {
    std::ifstream file("players.txt");
    if (file.is_open()) {
        PlayerInfo player;
        while (file >> player.playerID >> player.playerMark) {
            players.push_back(player);
        }
        file.close();
    } else {
        std::cout << "Unable to open player data file." << std::endl;
    }
}

void LoadMatchResults(std::vector<PlayerInfo>& players) {
    std::ifstream file("tictactoe.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            char result = line[0];
            bool same = true;
            for (char c : line) {
                if (c != result && c != '.') {
                    same = false;
                    break;
                }
            }
            if (!same && result != '.') {
                char opponent = '.';
                for (char c : line) {
                    if (c != result && c != '.') {
                        opponent = c;
                        break;
                    }
                }
                for (PlayerInfo& player : players) {
                    if (player.playerMark == result) {
                        player.points += 3;
                        player.victories++;
                    } else if (player.playerMark == opponent) {
                        player.losses++;
                    }
                }
            } else if (same && result != '.') {
                for (PlayerInfo& player : players) {
                    if (player.playerMark == result) {
                        player.points += 1;
                        player.draws++;
                    }
                }
            }
        }
        file.close();
    } else {
        std::cout << "Unable to open match results file." << std::endl;
    }
}


void DisplayTable(std::vector<PlayerInfo>& players) {
    std::sort(players.begin(), players.end(), [](const PlayerInfo& a, const PlayerInfo& b) {
        return a.points > b.points;
    });

    std::cout << "PlayerID\tMark\tWins\tDraws\tLosses\tPoints" << std::endl;
    for (const auto& player : players) {
        std::cout << player.playerID << "\t" << player.playerMark << "\t"
                  << player.victories << "\t" << player.draws << "\t"
                  << player.losses << "\t" << player.points << std::endl;
    }
}

void DisplayMatchResults() {
    std::ifstream file("tictactoe.txt");
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "Unable to open match results file." << std::endl;
    }
}

void DisplayPlayerStats(const std::vector<PlayerInfo>& players) {
    int id;
    std::cout << "Enter player ID: ";
    std::cin >> id;

    while (std::cin.fail()) {
        std::cout << "Invalid input. Please enter a number: ";
        std::cin.clear();
        std::cin.ignore(256, '\n');
        std::cin >> id;
    }

    auto it = std::find_if(players.begin(), players.end(),
                           [id](const PlayerInfo& player) {
                               return player.playerID == id;
                           });
    if (it != players.end()) {
        std::cout << "Player stats for ID " << id << ":" << std::endl;
        std::cout << "Mark: " << it->playerMark << std::endl;
        std::cout << "Wins: " << it->victories << std::endl;
        std::cout << "Draws: " << it->draws << std::endl;
        std::cout << "Losses: " << it->losses << std::endl;
        std::cout << "Points: " << it->points << std::endl;
    } else {
        std::cout << "Player with ID " << id << " not found" << std::endl;
    }
}

int main() {
    std::vector<PlayerInfo> players;

    LoadPlayerData(players);
    LoadMatchResults(players);

    int choice;
    do {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Show table" << std::endl;
        std::cout << "2. Show results" << std::endl;
        std::cout << "3. Show player stats" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        while (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number: ";
            std::cin.clear();
            std::cin.ignore(256, '\n');
            std::cin >> choice;
        }

        switch (choice) {
            case 1:
                DisplayTable(players);
                break;
            case 2:
                DisplayMatchResults();
                break;
            case 3:
                DisplayPlayerStats(players);
                break;
            case 4:
                break;
            default:
                std::cout << "Invalid choice. Please enter a number from 1 to 4." << std::endl;
        }
    } while (choice != 4);

    return 0;
}
