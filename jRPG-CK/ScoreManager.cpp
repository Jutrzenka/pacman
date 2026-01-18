#include "ScoreManager.h"
#include <fstream>
#include <sstream>
#include <string>

void ScoreManager::SaveScore(const std::string& name, int score) {
    std::ofstream file("scores.txt", std::ios::app);
    if (file.is_open()) {
        file << "Gracz: " << (name.empty() ? "Anonim" : name) << " | Wynik: " << score << std::endl;
        file.close();
    }
}

std::string ScoreManager::GetHighScore() {
    std::ifstream file("scores.txt");
    std::string line, bestName = "Brak";
    int highScore = 0;
    if (!file.is_open()) return "Rekord: 0 (Brak)";
    while (std::getline(file, line)) {
        size_t namePos = line.find("Gracz: ");
        size_t scorePos = line.find("Wynik: ");
        if (namePos != std::string::npos && scorePos != std::string::npos) {
            std::string namePart = line.substr(namePos + 7, line.find(" |") - (namePos + 7));
            try {
                int currentScore = std::stoi(line.substr(scorePos + 7));
                if (currentScore > highScore) {
                    highScore = currentScore;
                    bestName = namePart;
                }
            }
            catch (...) {}
        }
    }
    file.close();
    return "Rekord: " + std::to_string(highScore) + " (" + bestName + ")";
}