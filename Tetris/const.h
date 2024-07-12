#pragma once

double lastUpdateTime = 0;
double interval = 0.2;
bool swSound = true;
bool pause = true;
int temp = 0;

const float buttonWidth = 200;
const float buttonHeight = 50;
const float buttonSpacing = 20;

const float screenWidth = 656;
const float screenHeight = 620;

enum GameState {
    MENU,
    GAMEPLAY,
    MODE,
    GAME_OVER
};
