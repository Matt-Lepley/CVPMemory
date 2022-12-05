#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

enum class StateManager { MENU, PLAYING, GAMEOVER };

class Game
{
public:
	bool init();
	void clean();

	SDL_Texture* LoadMedia(string path);
	SDL_Texture* loadFromRenderedText(string textureText, int& width, int& height);

	bool MouseCollision(int mX, int mY, SDL_Rect* rect);
	void HandleButtonCollision();
	void HandleCardCollision();
	void Shuffle();
	void CompareCards();
	void ResetData();

	void loop();
	void handleEvents();
	void update();
	void render();
private:
	bool isRunning;

	const int SCREEN_WIDTH = 890;
	const int SCREEN_HEIGHT = 750;

	Uint32 ticksCount = 0;
	int viewCardTimer = 0;
	int viewCardDuration = 1000;
	bool foundMatch = false;
	int currentMatchCount = 0;
	int currentFlipCount = 0;
	int allMatchCount;

	StateManager mState = StateManager::MENU;

	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* font = NULL;

	SDL_Texture* cardBackTexture;
	SDL_Texture* startBtnTexture;
	SDL_Texture* quitBtnTexture;
	SDL_Texture* gameoverTexture;
	SDL_Texture* matchCountTexture;
	SDL_Texture* flipCountTexture;

	string matchCountString;
	string flipCountString;

	SDL_Rect startBtnRect;
	SDL_Rect quitBtnRect;
	SDL_Rect gameoverRect;

	int btnWidth = 300;
	int btnHeight = 100;
	int flipCountWidth = 0;
	int matchCountWidth = 0;
	int flipCountHeight = 0;
	int matchCountHeight = 0;

	vector<class Card*> cards;
	Card* cardOne, *cardTwo = NULL;
};

