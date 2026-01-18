#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>

class ScoreManager {
public:
    void SaveScore(const std::string& name, int score);
    std::string GetHighScore();
};

#endif