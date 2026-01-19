#include "ScoreManager.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>      // Dodaj ten include
#include <algorithm>   // Dodaj ten include
#include <utility>     // Dodaj ten include

void ScoreManager::SaveScore(const std::string& name, int score) {
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open()) {
        file << "Gracz: " << (name.empty() ? "Anonim" : name) << " | Wynik: " << score << std::endl;
        file.close();
    }
}

std::string ScoreManager::GetHighScore() {
    std::ifstream file("scores.txt");
    std::vector<std::pair<std::string, int>> scores;

    if (!file.is_open()) {
        return "Brak wynikow";
    }

    std::string line;
    while (std::getline(file, line)) {
        size_t namePos = line.find("Gracz: ");
        size_t scorePos = line.find("Wynik: ");

        if (namePos != std::string::npos && scorePos != std::string::npos) {
            std::string name = line.substr(namePos + 7, line.find(" |") - (namePos + 7));
            std::string scoreStr = line.substr(scorePos + 7);

            try {
                int score = std::stoi(scoreStr);
                scores.push_back(std::make_pair(name, score));
            }
            catch (...) {
                // Ignoruj b≥Ídne wpisy
            }
        }
    }
    file.close();

    // Sortuj malejπco
    std::sort(scores.begin(), scores.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // Weü top 3
    std::string result = "TOP 3 WYNIKI:\n";
    int count = std::min((int)scores.size(), 3);

    for (int i = 0; i < count; i++) {
        result += std::to_string(i + 1) + ". " + scores[i].first + " - " +
            std::to_string(scores[i].second) + "\n";
    }

    if (scores.empty()) {
        result = "Brak wynikow";
    }

    return result;
}