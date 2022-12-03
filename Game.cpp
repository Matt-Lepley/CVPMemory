#include "Game.h"
#include "Card.h"

bool Game::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("PONG", 500, 200, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
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

	isRunning = true;

	cardBackTexture = LoadMedia("Assets/cardBack.png");

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

void Game::CheckCollision() {
	int mX, mY;

	SDL_PumpEvents();
	SDL_GetMouseState(&mX, &mY);

	for (int i = 0; i < cards.size(); i++)
	{
		SDL_Rect* rect = cards[i]->GetRect();
		if (
			mX >= rect->x &&
			mX <= rect->x + rect->w &&
			mY >= rect->y &&
			mY <= rect->y + rect->h
			) {
			if (cardOne == NULL) {
				cardOne = cards[i];
				cardOne->SetFlipped(true);
			}
			else {
				cardTwo = cards[i];
				cardTwo->SetFlipped(true);
			}
		}
	}
}

void Game::CompareCards() {
	if (viewCardTimer == 0) {
		foundMatch = false;

		if (cardOne->GetValue() == cardTwo->GetValue()) {
			cout << "MATCH" << endl;
			foundMatch = true;
		}

		viewCardTimer = SDL_GetTicks();
	}

	if (SDL_GetTicks() - viewCardTimer >= viewCardDuration) {
		if (!foundMatch) {
			cardOne->SetFlipped(false);
			cardTwo->SetFlipped(false);
		}
		cardOne = NULL;
		cardTwo = NULL;
		viewCardTimer = 0;
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
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (!cardOne || !cardTwo) {
					CheckCollision();
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				Shuffle();
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

	for (int i = 0; i < cards.size(); i++)
	{
		if (cards[i]->GetFlipped()) {
			SDL_RenderCopy(renderer, cards[i]->GetTexture(), NULL, cards[i]->GetRect());
		}
		else {
			SDL_RenderCopy(renderer, cardBackTexture, NULL, cards[i]->GetRect());
		}
	}

	SDL_RenderPresent(renderer);
}