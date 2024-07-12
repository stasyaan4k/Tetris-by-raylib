#include "game.h"
#include <random>

Game::Game() {

    grid = Grid();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
    InitAudioDevice();
    music = LoadMusicStream("C:\\Users\\stasm\\Downloads\\Sounds\\music.mp3");
    PlayMusicStream(music);
    rotateSound = LoadSound("C:\\Users\\stasm\\Downloads\\Sounds\\rotate.mp3");
    clearSound = LoadSound("C:\\Users\\stasm\\Downloads\\Sounds\\clear.mp3");
}

Game::~Game() {

   UnloadSound(rotateSound);
   UnloadSound(clearSound);
   UnloadMusicStream(music);
   CloseAudioDevice();
}

Block Game::GetRandomBlock() {

    if (blocks.empty()) {
   
        blocks = GetAllBlocks();
    }
    int randomIndex = rand() % blocks.size();
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game::GetAllBlocks() {
    return { IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock() };
}

void Game::Draw() {

    grid.Draw();
    currentBlock.Draw(11, 11);
    switch (nextBlock.id) {
    
    case 3: nextBlock.Draw(330, 350); break;
    case 4: nextBlock.Draw(330, 335); break;
    default: nextBlock.Draw(345, 330); break;
    }
}

void Game::HandleInput() {

    float deltaTime = GetFrameTime();
    moveTimer += deltaTime;
    fallTimer += deltaTime;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        if (moveTimer >= moveDelay) {
            MoveBlockLeft();
            moveTimer = 0.0f;
        }
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        if (moveTimer >= moveDelay) {
            MoveBlockRight();
            moveTimer = 0.0f;
        }
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) {
        if (fallTimer >= 1.0f / fastFallSpeed) {
            MoveBlockDown();
            UpdateScore(0, 1);
            fallTimer = 0.0f;
        }
    }
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        RotateBlock();
    }

    if (fallTimer >= 1.0f / fallSpeed) {
        MoveBlockDown();
        fallTimer = 0.0f;
    }
}

void Game::MoveBlockLeft() {

    if (!gameOver) {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight() {
    if (!gameOver) {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown() {
    if (!gameOver) {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

bool Game::IsBlockOutside() {
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) {
        if (grid.IsCellOutside(item.row, item.column)) {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock() {
    if (!gameOver) {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false) {
            currentBlock.UndoRotation();
        }
        else {
            PlaySound(rotateSound);
        }
    }
}

void Game::LockBlock() {
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false) {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0) {
        PlaySound(clearSound);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits() {
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles) {
        if (grid.IsCellEmpty(item.row, item.column) == false) {
            return false;
        }
    }
    return true;
}

void Game::UpdateScore(int linesCleared, int moveDownPoints) {
    switch (linesCleared) {
    case 1: score += 100; break;
    case 2: score += 200; break;
    case 3: score += 300; break;
    case 4: score += 400; break;
    default: break;
    }
    score += moveDownPoints;
}

void Game::Reset() {
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0; 
}