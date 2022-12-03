#include "Card.h"
#include "Game.h"

Card::Card(Game* game, string path, int value) {
	mTexture = game->LoadMedia(path);
	mValue = value;
	mFlipped = false;
	mWidth = 100;
	mHeight = 150;
}

Card::~Card() {
	SDL_DestroyTexture(mTexture);
}

SDL_Texture* Card::GetTexture() {
	return mTexture;
}

int Card::GetWidth() {
	return mWidth;
}

int Card::GetHeight() {
	return mHeight;
}

int Card::GetValue() {
	return mValue;
}

SDL_Rect* Card::GetRect() {
	return &mRect;
}

bool Card::GetFlipped() {
	return mFlipped;
}

void Card::SetFlipped(bool flipped) {
	mFlipped = flipped;
}

void Card::SetRect(int x, int y) {
	mRect.x = x;
	mRect.y = y;
	mRect.w = mWidth;
	mRect.h = mHeight;
}