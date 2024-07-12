#pragma once
#include "lines.h"
#include "blocks.cpp"

class Game {
public:
    Game();
    ~Game();
    int score;
    void Draw();
    void HandleInput();
    void MoveBlockDown();
    void Reset();
    bool gameOver;
    Music music;
    Sound rotateSound;
    Sound clearSound;
private:

    void MoveBlockLeft();
    void MoveBlockRight();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void UpdateScore(int linesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    float moveDelay = 0.15f; // Задержка между перемещениями в секундах
    float moveTimer = 0.0f;  // Таймер для отслеживания времени с последнего перемещения
    float fallTimer = 0.0f;  // Таймер для падения блока
    float fallSpeed = 1.0f;  // Обычная скорость падения
    float fastFallSpeed = 10.0f; // Ускоренная скорость падения
};