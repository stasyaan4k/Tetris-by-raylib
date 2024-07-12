#include "raylib.h"
#include "game.h"
#include "Colors.h"
#include "const.h"
#include <iostream>
#include <fstream>

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

int loadHighScore() {
    int highScore;
    std::ifstream file("HighScore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void saveHighScore(const std::string& HighScore, int highScore) {
    std::ofstream file("HighScore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

int main() {

    InitWindow(screenWidth, screenHeight, "Tetris");
    SetTargetFPS(60);

    std::string fileName = "HighScore.txt";
    int highScore = loadHighScore();
    GameState gameState = MENU;
    Font font = LoadFontEx("C:\\Users\\stasm\\Downloads\\Font\\myFont.ttf", 64, 0, 0);

    while (!WindowShouldClose()) {

        switch (gameState) {

        case MENU: {
            Vector2 mousePoint = GetMousePosition();

            Rectangle startButton = { screenWidth / 2 - buttonWidth / 2,
                                            screenHeight / 2 - buttonHeight - buttonSpacing * 2,
                                            buttonWidth, buttonHeight };
            Rectangle settingsButton = { screenWidth / 2 - buttonWidth / 2,
                                                screenHeight / 2 - buttonHeight / 2,
                                                buttonWidth, buttonHeight };
            Rectangle quitButton = { screenWidth / 2 - buttonWidth / 2,
                                            screenHeight / 2 + buttonHeight / 2 + buttonSpacing,
                                            buttonWidth, buttonHeight };

            bool startButtonClicked = CheckCollisionPointRec(mousePoint, startButton);
            bool settingsButtonClicked = CheckCollisionPointRec(mousePoint, settingsButton);
            bool quitButtonClicked = CheckCollisionPointRec(mousePoint, quitButton);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (startButtonClicked) {
                    gameState = GAMEPLAY;
                }
                else if (settingsButtonClicked) {
                    gameState = MODE;
                }
                else if (quitButtonClicked) {
                    CloseWindow();
                    return 0;
                }
            }

            BeginDrawing();
            ClearBackground(backgroundColor);

            Color startButtonColor = startButtonClicked ? buttonHoverColor : lightBlue;
            Color settingsButtonColor = settingsButtonClicked ? buttonHoverColor : lightBlue;
            Color quitButtonColor = quitButtonClicked ? buttonHoverColor : lightBlue;

            DrawTextEx(font, "TETRIS",
                { screenWidth / 2 - MeasureTextEx(font, "TETRIS", 40, 2).x / 2, 40 }, 40, 2, textColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 - buttonHeight - buttonSpacing * 2,
                buttonWidth, buttonHeight, startButtonColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 - buttonHeight / 2,
                buttonWidth, buttonHeight, settingsButtonColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 + buttonHeight / 2 + buttonSpacing,
                buttonWidth, buttonHeight, quitButtonColor);

            DrawTextEx(font, "Start Game",
                { screenWidth / 2 - MeasureTextEx(font, "Start Game", 20, 2).x / 2,
                  screenHeight / 2 - buttonHeight - buttonSpacing * 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Mode",
                { screenWidth / 2 - MeasureTextEx(font, "Mode", 20, 2).x / 2,
                  screenHeight / 2 - buttonHeight / 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Quit",
                { screenWidth / 2 - MeasureTextEx(font, "Quit", 20, 2).x / 2,
                  screenHeight / 2 + buttonHeight / 2 + buttonSpacing + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Created by Stas Malenkov",
                { screenWidth - MeasureTextEx(font, "Created by Stas Malenkov", 15, 2).x - 10,
                  screenHeight - 20 },
                15, 2, textColor);

            EndDrawing();
            break;
        }

        case GAMEPLAY: {

            Game game = Game();
            PlayMusicStream(game.music);

            while (!WindowShouldClose() && !game.gameOver) {
                if (pause) {
                    game.HandleInput();

                    if (EventTriggered(interval)) {
                        game.MoveBlockDown();
                    }
                }

                UpdateMusicStream(game.music);

                BeginDrawing();
                ClearBackground(darkBlue);

                DrawTextEx(font, "Score", { 395 + (170 - MeasureTextEx(font, "Score", 38, 2).x) / 2, 15 }, 38, 2, textColor);
                DrawRectangleRounded({ 395, 55, 170, 60 }, 0.3, 6, lightBlue);

                char scoreText[10];
                sprintf_s(scoreText, "%d", game.score);
                Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
                DrawTextEx(font, scoreText, { 395 + (170 - textSize.x) / 2, 65 }, 38, 2, textColor);

                DrawTextEx(font, "High Score", { 395 + (170 - MeasureTextEx(font, "High Score", 38, 2).x) / 2, 125 }, 38, 2, textColor);
                DrawRectangleRounded({ 395, 165, 170, 60 }, 0.3, 6, lightBlue);

                char highScoreText[10];
                sprintf_s(highScoreText, "%d", highScore);
                Vector2 highScoreTextSize = MeasureTextEx(font, highScoreText, 38, 2);
                DrawTextEx(font, highScoreText, { 395 + (170 - highScoreTextSize.x) / 2, 175 }, 38, 2, textColor);

                DrawTextEx(font, "Next", { 395 + (170 - MeasureTextEx(font, "Next", 38, 2).x) / 2, 235 }, 38, 2, textColor);
                DrawRectangleRounded({ 395, 275, 170, 180 }, 0.3, 6, lightBlue);

                Vector2 mousePoint = GetMousePosition();

                Rectangle pauseButton = { 325, 475, 150, buttonHeight };
                Rectangle soundButton = { 325, 535, 150, buttonHeight };
                Rectangle restartButton = { 495, 475, 150, buttonHeight };
                Rectangle ggButton = { 495, 535, 150, buttonHeight };

                bool pauseButtonClicked = CheckCollisionPointRec(mousePoint, pauseButton);
                bool soundButtonClicked = CheckCollisionPointRec(mousePoint, soundButton);
                bool restartButtonClicked = CheckCollisionPointRec(mousePoint, restartButton);
                bool ggButtonClicked = CheckCollisionPointRec(mousePoint, ggButton);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (pauseButtonClicked) {
                        if (pause) {
                            pause = false;
                            PauseMusicStream(game.music);
                        }
                        else if (!pause) {
                            pause = true;
                            ResumeMusicStream(game.music);
                        }
                    }
                    else if (soundButtonClicked) {
                        if (swSound) {
                            CloseAudioDevice();
                            swSound = false;
                        }
                        else if (!swSound) {
                            InitAudioDevice();
                            swSound = true;
                        }
                    }
                    else if (restartButtonClicked) {
                        game.Reset();
                    }
                    else if (ggButtonClicked) {
                        game.gameOver = true;
                    }
                }

                Color pauseButtonColor = pauseButtonClicked ? buttonHoverColor : lightBlue;
                Color soundButtonColor = soundButtonClicked ? buttonHoverColor : lightBlue;
                Color restartButtonColor = restartButtonClicked ? buttonHoverColor : lightBlue;
                Color ggButtonColor = ggButtonClicked ? buttonHoverColor : lightBlue;

                DrawRectangleRounded({ 325, 475, 150, buttonHeight }, 0.3, 6, pauseButtonColor);
                DrawRectangleRounded({ 325, 535, 150, buttonHeight }, 0.3, 6, soundButtonColor);
                DrawRectangleRounded({ 495, 475, 150, buttonHeight }, 0.3, 6, restartButtonColor);
                DrawRectangleRounded({ 495, 535, 150, buttonHeight }, 0.3, 6, ggButtonColor);

                DrawTextEx(font, "PAUSE", { 325 + (150 - MeasureTextEx(font, "PAUSE", 35, 2).x) / 2, 485 }, 35, 2, textColor);
                DrawTextEx(font, "SOUNDS", { 325 + (150 - MeasureTextEx(font, "SOUNDS", 35, 2).x) / 2, 545 }, 35, 2, textColor);
                DrawTextEx(font, "GG!", { 495 + (150 - MeasureTextEx(font, "GG!", 35, 2).x) / 2, 545 }, 35, 2, textColor);
                DrawTextEx(font, "RESTART", { 495 + (150 - MeasureTextEx(font, "RESTART", 35, 2).x) / 2, 485 }, 35, 2, textColor);

                game.Draw();
                EndDrawing();

                if (game.score > highScore) {
                    highScore = game.score;
                    saveHighScore("HighScore.txt", highScore);
                }

                temp = game.score;
            }
            gameState = GAME_OVER;
            break;
        }

        case MODE: {
            Vector2 mousePoint = GetMousePosition();

            Rectangle backButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                           screenHeight - buttonHeight - 35,
                                           buttonWidth, buttonHeight };

            bool backButtonClicked = CheckCollisionPointRec(mousePoint, backButtonBounds);

            Rectangle easyModeButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                               screenHeight / 2 - buttonHeight - buttonSpacing * 2,
                                               buttonWidth, buttonHeight };

            bool easyModeButtonClicked = CheckCollisionPointRec(mousePoint, easyModeButtonBounds);

            Rectangle mediumModeButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                                 screenHeight / 2 - buttonHeight / 2,
                                                 buttonWidth, buttonHeight };

            bool mediumModeButtonClicked = CheckCollisionPointRec(mousePoint, mediumModeButtonBounds);

            Rectangle impossibleModeButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                                     screenHeight / 2 + buttonHeight / 2 + buttonSpacing,
                                                     buttonWidth, buttonHeight };

            bool impossibleModeButtonClicked = CheckCollisionPointRec(mousePoint, impossibleModeButtonBounds);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (backButtonClicked) {
                    gameState = MENU;
                }
                else if (easyModeButtonClicked) {
                    interval = 0.5;
                }
                else if (mediumModeButtonClicked) {
                    interval = 0.2;
                }
                else if (impossibleModeButtonClicked) {
                    interval = 0.1;
                }
            }

            BeginDrawing();
            ClearBackground(backgroundColor);

            DrawTextEx(font, "Difficult Mode", { 315 - MeasureTextEx(font, "Difficult Mode", 40, 2).x / 2, 40 }, 40, 2, textColor);

            Color backButtonColor = backButtonClicked ? buttonHoverColor : lightBlue;
            Color easyModeButtonColor = easyModeButtonClicked ? buttonHoverColor : lightBlue;
            Color mediumModeButtonColor = mediumModeButtonClicked ? buttonHoverColor : lightBlue;
            Color impossibleModeButtonColor = impossibleModeButtonClicked ? buttonHoverColor : lightBlue;

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 - buttonHeight - buttonSpacing * 2,
                buttonWidth, buttonHeight, easyModeButtonColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 - buttonHeight / 2,
                buttonWidth, buttonHeight, mediumModeButtonColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 + buttonHeight / 2 + buttonSpacing,
                buttonWidth, buttonHeight, impossibleModeButtonColor);

            DrawTextEx(font, "Easy",
                { screenWidth / 2 - MeasureTextEx(font, "Easy", 20, 2).x / 2,
                  screenHeight / 2 - buttonHeight - buttonSpacing * 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Medium",
                { screenWidth / 2 - MeasureTextEx(font, "Medium", 20, 2).x / 2,
                  screenHeight / 2 - buttonHeight / 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Impossible",
                { screenWidth / 2 - MeasureTextEx(font, "Impossible", 20, 2).x / 2,
                  screenHeight / 2 + buttonHeight / 2 + buttonSpacing + buttonHeight / 4 },
                20, 2, textColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight - buttonHeight - 35,
                buttonWidth, buttonHeight, backButtonColor);

            DrawTextEx(font, "Back to Menu",
                { screenWidth / 2 - MeasureTextEx(font, "Back to Menu", 20, 2).x / 2,
                  screenHeight - buttonHeight - 35 + (buttonHeight - MeasureTextEx(font, "Back to Menu", 20, 2).y) / 2 },
                20, 2, textColor);

            Color easyModeIndicatorColor = interval == 0.5 ? green : darkGrey;
            Color mediumModeIndicatorColor = interval == 0.2 ? green : darkGrey;
            Color impossibleModeIndicatorColor = interval == 0.1 ? green : darkGrey;

            DrawRectangle(screenWidth / 2 + buttonWidth / 2 + 15,
                screenHeight / 2 - buttonHeight - buttonSpacing * 2 + buttonHeight / 4,
                20, 20, easyModeIndicatorColor);

            DrawRectangle(screenWidth / 2 + buttonWidth / 2 + 15,
                screenHeight / 2 - buttonHeight / 2 + buttonHeight / 4,
                20, 20, mediumModeIndicatorColor);

            DrawRectangle(screenWidth / 2 + buttonWidth / 2 + 15,
                screenHeight / 2 + buttonHeight / 2 + buttonSpacing + buttonHeight / 4,
                20, 20, impossibleModeIndicatorColor);

            EndDrawing();

            break;
        }

        case GAME_OVER: {
            Vector2 mousePoint = GetMousePosition();

            Rectangle restartButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                              screenHeight / 2 - buttonHeight - buttonSpacing / 2,
                                              buttonWidth, buttonHeight };
            Rectangle menuButtonBounds = { screenWidth / 2 - buttonWidth / 2,
                                           screenHeight / 2 + buttonSpacing / 2,
                                           buttonWidth, buttonHeight };

            bool restartButtonClicked = CheckCollisionPointRec(mousePoint, restartButtonBounds);
            bool menuButtonClicked = CheckCollisionPointRec(mousePoint, menuButtonBounds);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (restartButtonClicked) {
                    gameState = GAMEPLAY;
                }
                else if (menuButtonClicked) {
                    gameState = MENU;
                }
            }

            BeginDrawing();
            ClearBackground(backgroundColor);

            Color restartButtonColor = restartButtonClicked ? buttonHoverColor : lightBlue;
            Color menuButtonColor = menuButtonClicked ? buttonHoverColor : lightBlue;

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 - buttonHeight - buttonSpacing / 2,
                buttonWidth, buttonHeight, restartButtonColor);

            DrawRectangle(screenWidth / 2 - buttonWidth / 2,
                screenHeight / 2 + buttonSpacing / 2,
                buttonWidth, buttonHeight, menuButtonColor);

            DrawTextEx(font, "Restart Game",
                { screenWidth / 2 - MeasureTextEx(font, "Restart Game", 20, 2).x / 2,
                  screenHeight / 2 - buttonHeight - buttonSpacing / 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawTextEx(font, "Back to Menu",
                { screenWidth / 2 - MeasureTextEx(font, "Back to Menu", 20, 2).x / 2,
                  screenHeight / 2 + buttonSpacing / 2 + buttonHeight / 4 },
                20, 2, textColor);

            DrawText("Game Over!", screenWidth / 2 - MeasureText("Game Over!", 30) / 2,
                screenHeight / 2 - buttonHeight * 2 - buttonSpacing * 2, 30, red);

            char scoreText[10];
            sprintf_s(scoreText, "%d", temp);
            Vector2 tempSize = MeasureTextEx(font, scoreText, 38, 2);
            DrawTextEx(font, scoreText, { 240 + (160 - tempSize.x) / 2, 485 }, 35, 2, textColor);
            DrawTextEx(font, "Score: ", { 100 + (150 - MeasureTextEx(font, "Score: ", 35, 2).x) / 2, 485 }, 35, 2, textColor);

            char highScoreText[10];
            sprintf_s(highScoreText, "%d", highScore);
            Vector2 highScoreTextSize = MeasureTextEx(font, highScoreText, 35, 2);
            DrawTextEx(font, highScoreText, { 240 + (160 - highScoreTextSize.x) / 2, 525 }, 35, 2, textColor);
            DrawTextEx(font, "High Score: ", { 100 + (150 - MeasureTextEx(font, "High Score: ", 35, 2).x) / 2, 525 }, 35, 2, textColor);

            EndDrawing();
            break;
        }
        }
           
    }
    CloseWindow();
    return 0;
}