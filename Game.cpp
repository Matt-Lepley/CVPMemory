#include "Game.h"
#include "Card.h"

bool Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("CVP Memory", 300, 30, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!window) {
		SDL_Log("Failed to create Window: %s", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		SDL_Log("Failed to create Renderer: %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == NULL) {
		SDL_Log("Failed to initialize IMG: %s", IMG_GetError());
		return false;
	}
	if (TTF_Init() != 0)
	{
		SDL_Log("Failed to initialize TTF: %s", IMG_GetError());
		return false;
	}

	font = TTF_OpenFont("Assets/bubbleFont.ttf", 50);

	isRunning = true;

	// Buttons
	startBtnTexture = LoadMedia("Assets/start.png");
	startBtnRect = {
		SCREEN_WIDTH / 2 - btnWidth / 2,
		SCREEN_HEIGHT / 2 - btnHeight - 20,
		btnWidth,
		btnHeight
	};

	quitBtnTexture = LoadMedia("Assets/quit.png");
	quitBtnRect = {
		SCREEN_WIDTH / 2 - btnWidth / 2,
		SCREEN_HEIGHT / 2,
		btnWidth,
		btnHeight
	};

	gameoverTexture = LoadMedia("Assets/gameover.png");
	gameoverRect = {
		SCREEN_WIDTH / 2 - 225,
		20,
		450,
		100
	};

	matchCountString = "Matches: 0";
	flipCountString = "Flips: 0";

	// Back Card Texture
	cardBackTexture = LoadMedia("Assets/cardBack.png");

	// Load all cards and push on vector
	cards.emplace_back(new Card(this, "Assets/1.png", 0));
	cards.emplace_back(new Card(this, "Assets/1.png", 0));
	cards.emplace_back(new Card(this, "Assets/2.png", 1));
	cards.emplace_back(new Card(this, "Assets/2.png", 1));
	cards.emplace_back(new Card(this, "Assets/3.png", 2));
	cards.emplace_back(new Card(this, "Assets/3.png", 2));
	cards.emplace_back(new Card(this, "Assets/4.png", 3));
	cards.emplace_back(new Card(this, "Assets/4.png", 3));
	cards.emplace_back(new Card(this, "Assets/5.png", 4));
	cards.emplace_back(new Card(this, "Assets/5.png", 4));
	cards.emplace_back(new Card(this, "Assets/6.png", 5));
	cards.emplace_back(new Card(this, "Assets/6.png", 5));
	cards.emplace_back(new Card(this, "Assets/7.png", 6));
	cards.emplace_back(new Card(this, "Assets/7.png", 6));
	cards.emplace_back(new Card(this, "Assets/8.png", 7));
	cards.emplace_back(new Card(this, "Assets/8.png", 7));
	cards.emplace_back(new Card(this, "Assets/9.png", 8));
	cards.emplace_back(new Card(this, "Assets/9.png", 8));
	cards.emplace_back(new Card(this, "Assets/10.png", 9));
	cards.emplace_back(new Card(this, "Assets/10.png", 9));
	cards.emplace_back(new Card(this, "Assets/11.png", 10));
	cards.emplace_back(new Card(this, "Assets/11.png", 10));
	cards.emplace_back(new Card(this, "Assets/12.png", 11));
	cards.emplace_back(new Card(this, "Assets/12.png", 11));
	cards.emplace_back(new Card(this, "Assets/13.png", 12));
	cards.emplace_back(new Card(this, "Assets/13.png", 12));
	cards.emplace_back(new Card(this, "Assets/14.png", 13));
	cards.emplace_back(new Card(this, "Assets/14.png", 13));
	cards.emplace_back(new Card(this, "Assets/15.png", 14));
	cards.emplace_back(new Card(this, "Assets/15.png", 14));
	cards.emplace_back(new Card(this, "Assets/16.png", 15));
	cards.emplace_back(new Card(this, "Assets/16.png", 15));

	allMatchCount = cards.size() / 2;

	Shuffle();
	
	return true;
}

void Game::clean() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::loop() {
	while (isRunning) {
		handleEvents();
		update();
		render();
	}
}

SDL_Texture* Game::LoadMedia(string path) {
	SDL_Surface* temp = IMG_Load(path.c_str());
	if (temp == NULL) {
		cout << "Unable to load surface: " << IMG_GetError() << endl;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
	if (texture == NULL) {
		cout << "Unable to convert to texture: " << SDL_GetError() << endl;
	}

	SDL_FreeSurface(temp);

	return texture;
}

bool Game::MouseCollision(int mX, int mY, SDL_Rect* rect) {
	return mX >= rect->x &&
		mX <= rect->x + rect->w &&
		mY >= rect->y &&
		mY <= rect->y + rect->h;
}

void Game::ResetData() {
	Shuffle();

	currentMatchCount = 0;
	currentFlipCount = 0;
	matchCountString = "Matches: 0";
	flipCountString = "Flips: 0";

	for (int i = 0; i < cards.size(); i++)
	{
		cards[i]->SetFlipped(false);
	}
}

void Game::HandleButtonCollision() {
	int mX, mY;

	SDL_PumpEvents();
	SDL_GetMouseState(&mX, &mY);

	if (MouseCollision(mX, mY, &startBtnRect)) {
		if (mState == StateManager::GAMEOVER) {
			ResetData();
		}

		mState = StateManager::PLAYING;
	}
	else if (MouseCollision(mX, mY, &quitBtnRect)) {
		isRunning = false;
	}
}

void Game::HandleCardCollision() {
	int mX, mY;

	SDL_PumpEvents();
	SDL_GetMouseState(&mX, &mY);

	for (int i = 0; i < cards.size(); i++)
	{
		SDL_Rect* rect = cards[i]->GetRect();
		if (MouseCollision(mX, mY, rect)) {
			// Need to skip iteration if card is already flipped
			if (cards[i]->GetFlipped()) {
				break;
			}

			if (cardOne == NULL) {
				cardOne = cards[i];
				cardOne->SetFlipped(true);
				currentFlipCount += 1;
			}
			else {
				cardTwo = cards[i];
				cardTwo->SetFlipped(true);
				currentFlipCount += 1;
			}
		}
	}

	flipCountString = "Flips: " + to_string(currentFlipCount);
}

void Game::CompareCards() {
	if (viewCardTimer == 0) {
		foundMatch = false;

		if (cardOne->GetValue() == cardTwo->GetValue()) {
			foundMatch = true;
			currentMatchCount += 1;
		}

		viewCardTimer = SDL_GetTicks();
	}

	matchCountString = "Matches: " + to_string(currentMatchCount);

	if (SDL_GetTicks() - viewCardTimer >= viewCardDuration) {
		if (!foundMatch) {
			cardOne->SetFlipped(false);
			cardTwo->SetFlipped(false);
		}
		cardOne = NULL;
		cardTwo = NULL;
		viewCardTimer = 0;

		if (currentMatchCount == allMatchCount) {
			mState = StateManager::GAMEOVER;
		}
	}
}

void Game::Shuffle() {
	int rowLength = 8;
	int padding = 10;

	srand(time(0));
	random_shuffle(cards.begin(), cards.end());

	for (int i = 0; i < cards.size(); i++)
	{
		int x = i % rowLength * cards[i]->GetWidth() + (i % rowLength * padding);
		int y = i / rowLength * cards[i]->GetHeight() + (i / rowLength * padding);
		cards[i]->SetRect(x + padding, y + padding);
		cards[i]->SetFlipped(false);
	}
}

void Game::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_s) {
				Shuffle();
			}
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_q) {
				mState = StateManager::GAMEOVER;
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (mState == StateManager::MENU || mState == StateManager::GAMEOVER) {
				HandleButtonCollision();
			}
			else if (mState == StateManager::PLAYING) {
				if (!cardOne || !cardTwo) {
					HandleCardCollision();
				}
			}
		}
	}

	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_ESCAPE]) {
		isRunning = false;
	}
}

void Game::update() {
	// delay for 16ms
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;
	ticksCount = SDL_GetTicks();

	// Clamp max delta time
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	if (cardOne && cardTwo) {
		CompareCards();
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	if (mState == StateManager::MENU) {
		SDL_RenderCopy(renderer, startBtnTexture, NULL, &startBtnRect);
		SDL_RenderCopy(renderer, quitBtnTexture, NULL, &quitBtnRect);
	}
	else if (mState == StateManager::PLAYING) {
		for (int i = 0; i < cards.size(); i++)
		{
			if (!cards[i]->GetFlipped()) {
				SDL_RenderCopy(renderer, cards[i]->GetTexture(), NULL, cards[i]->GetRect());
			}
			else {
				SDL_RenderCopy(renderer, cardBackTexture, NULL, cards[i]->GetRect());
			}
		}

		flipCountTexture = loadFromRenderedText(flipCountString, flipCountWidth, flipCountHeight);
		matchCountTexture = loadFromRenderedText(matchCountString, matchCountWidth, matchCountHeight);

		SDL_Rect rect{ 10, SCREEN_HEIGHT - 80, flipCountWidth, flipCountHeight };
		SDL_RenderCopy(renderer, flipCountTexture, NULL, &rect);

		SDL_Rect rect2{ SCREEN_WIDTH - matchCountWidth - 10, SCREEN_HEIGHT - 80, matchCountWidth, matchCountHeight };
		SDL_RenderCopy(renderer, matchCountTexture, NULL, &rect2);
	}
	else if (mState == StateManager::GAMEOVER) {
		SDL_RenderCopy(renderer, gameoverTexture, NULL, &gameoverRect);

		// Show stats
		SDL_RenderCopy(renderer, startBtnTexture, NULL, &startBtnRect);
		SDL_RenderCopy(renderer, quitBtnTexture, NULL, &quitBtnRect);

		SDL_Rect rect{ 10, SCREEN_HEIGHT - 80, flipCountWidth, flipCountHeight };
		SDL_RenderCopy(renderer, flipCountTexture, NULL, &rect);

		SDL_Rect rect2{ SCREEN_WIDTH - matchCountWidth - 10, SCREEN_HEIGHT - 80, matchCountWidth, matchCountHeight };
		SDL_RenderCopy(renderer, matchCountTexture, NULL, &rect2);
	}

	SDL_RenderPresent(renderer);
}

SDL_Texture* Game::loadFromRenderedText(string textureText, int &width, int& height)
{
	SDL_Surface* textSurface;
	SDL_Texture* texture = NULL;
	SDL_Color color = { 0, 200, 0 };

	textSurface = TTF_RenderText_Solid(font, textureText.c_str(), color);
	if (textSurface == NULL)
	{
		cout << "Failed to load font surface: " << TTF_GetError() << endl;
	}
	else
	{
		//Create texture from surface pixels
		texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (texture == NULL)
		{
			cout << "Failed to render font texture: " << SDL_GetError() << endl;
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	return texture;
}