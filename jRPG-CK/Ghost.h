#ifndef GHOST_H
#define GHOST_H

#include "Snake.h"
#include "Board.h"

class Ghost : public Snake {
public:
    Ghost();
    void ResetToPosition(Vector2 startPosition);
    void DetermineRandomMovement(const Board& gameBoard);
    void UpdateArtificialIntelligence(const Board& gameBoard, Vector2 playerPosition, bool isFrightened);
    void PerformMovement(const Board& board);  // Dodano - override metody bazowej

    // Nowa metoda - sprawdza czy duch mo¿e iœæ w danym kierunku
    bool CanGhostMoveInDirection(Vector2 direction, const Board& board) const;

private:
    void MoveTowardPlayer(const Board& gameBoard, Vector2 playerPosition);
    void MoveAwayFromPlayer(const Board& gameBoard, Vector2 playerPosition);
};

#endif