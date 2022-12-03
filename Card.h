#pragma once

#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

class Card
{
public:
	Card(class Game* game, string path, int value);
	~Card();

	SDL_Texture* GetTexture();
	int GetWidth();
	int GetHeight();
	int GetValue();
	SDL_Rect* GetRect();
	bool GetFlipped();

	void SetFlipped(bool flipped);
	void SetRect(int x, int y);

private:
	SDL_Texture* mTexture;
	int mValue, mOrder, mWidth, mHeight;
	bool mFlipped;
	SDL_Rect mRect;
};

