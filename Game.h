#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Game
{
public:
	bool init();
	void clean();

	//void handleBallCollision(Vector2 paddlePos, string side, Ball& Ball);
	SDL_Texture* LoadMedia(string path);
	void CheckCollision();
	void Shuffle();
	void CompareCards();

	void loop();
	void handleEvents();
	void update();
	void render();
private:
	bool isRunning;

	const int SCREEN_WIDTH = 890;
	const int SCREEN_HEIGHT = 650;
	//const int mCardCount = 6;  <--- ???

	Uint32 ticksCount = 0;
	int viewCardTimer = 0;
	int viewCardDuration = 1000;
	bool foundMatch = false;

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* cardBackTexture;

	vector<class Card*> cards;
	//vector<class Card*> activeCards;
	Card* cardOne, *cardTwo = NULL;
};

