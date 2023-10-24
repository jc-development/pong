#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() :
		mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsRunning(true), mPaddleDir1(
				0), mPaddleDir2(0) {

}

bool Game::Initialize() {
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// Create an SDL Window
	mWindow = SDL_CreateWindow("Pong in C++", 100, 100, 1024,
			765, 0);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddlePos1.x = 10.0f;
	mPaddlePos1.y = 768.0f / 2.0f;
	mPaddlePos2.x = 1024.0f - thickness;
	mPaddlePos2.y = 768.0f / 2.0f;
	mBall1.position.x = 1024.0f / 2.0f;
	mBall1.position.y = 768.0f / 2.0f;
	mBall1.velocity.x = 200.0f;
	mBall1.velocity.y = 235.0f;

	mBall2.position.x = 1024.0f / 2.0f;
	mBall2.position.y = 768.0f / 2.0f;
	mBall2.velocity.x = -200.0f;
	mBall2.velocity.y = -235.0f;
	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		// if we get an SDL_Quit event, end loop
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	// Get state of keyboard
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	// Update paddle1 direction based on W/S keys
	mPaddleDir1 = 0;
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir1 -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir1 += 1;
	}

	// Update paddle2 direction based on I/K keys
	mPaddleDir2 = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddleDir2 += 1;
	}
}

void Game::UpdateGame() {
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame converted to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Update tick counts for next frame
	mTicksCount = SDL_GetTicks();

	// Update paddle position based on direction
	if (mPaddleDir1 != 0) {
		// 300 is the speed pixels/sec
		mPaddlePos1.y += mPaddleDir1 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen
		if (mPaddlePos1.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos1.y = paddleH / 2.0f + thickness;
		} else if (mPaddlePos1.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddlePos1.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	if (mPaddleDir2 != 0) {
		// 300 is the speed pixels/sec
		mPaddlePos2.y += mPaddleDir2 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen
		if (mPaddlePos2.y < (paddleH / 2.0f + thickness)) {
			mPaddlePos2.y = paddleH / 2.0f + thickness;
		} else if (mPaddlePos2.y > (768.0f - paddleH / 2.0f - thickness)) {
			mPaddlePos2.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	mBall1.position.x += mBall1.velocity.x * deltaTime;
	mBall1.position.y += mBall1.velocity.y * deltaTime;

	mBall2.position.x += mBall2.velocity.x * deltaTime;
	mBall2.position.y += mBall2.velocity.y * deltaTime;

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff1 = mPaddlePos1.y - mBall1.position.y;
	float diff2 = mPaddlePos2.y - mBall1.position.y;

	float diff3 = mPaddlePos1.y - mBall2.position.y;
	float diff4 = mPaddlePos2.y - mBall2.position.y;

	// Take absolute value of difference
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;

	diff3 = (diff3 > 0.0f) ? diff3 : -diff3;
	diff4 = (diff4 > 0.0f) ? diff4 : -diff4;

	if (
	// Our y-difference is small enough
	diff1 <= paddleH / 2.0f &&
	// We are in the correct x-position
			mBall1.position.x <= 25.0f && mBall1.position.x >= 20.0f &&
			// The ball is moving left
			mBall1.velocity.x < 0.0f) {
		mBall1.velocity.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBall1.position.x <= 0.0f) {
		mIsRunning = false;
	}

	// Take absolute value of difference

	if (
	// Our y-difference is small enough
	diff2 <= paddleH / 2.0f &&
	// We are in the correct x-position
			mBall1.position.x >= 25.0f && mBall1.position.x >= (1024.0f - thickness) &&
			// The ball is moving right
			mBall1.velocity.x > 0.0f) {
		mBall1.velocity.x *= -1.0f;
	}
	// Did the ball go off the screen? (if so, end game)
	else if (mBall1.position.x >= 1024.0f) {
		mIsRunning = false;
	}


	// Did the ball collide with the top wall?
	if (mBall1.position.y <= thickness && mBall1.velocity.y < 0.0f) {
		mBall1.velocity.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBall1.position.y >= (768 - thickness) && mBall1.velocity.y > 0.0f) {
		mBall1.velocity.y *= -1;
	}

	//------

	if (
		// Our y-difference is small enough
		diff3 <= paddleH / 2.0f &&
		// We are in the correct x-position
				mBall2.position.x <= 25.0f && mBall2.position.x >= 20.0f &&
				// The ball is moving left
				mBall2.velocity.x < 0.0f) {
			mBall2.velocity.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (mBall2.position.x <= 0.0f) {
			mIsRunning = false;
		}

		// Take absolute value of difference

		if (
		// Our y-difference is small enough
		diff4 <= paddleH / 2.0f &&
		// We are in the correct x-position
				mBall2.position.x >= 25.0f && mBall2.position.x >= (1024.0f - thickness) &&
				// The ball is moving right
				mBall2.velocity.x > 0.0f) {
			mBall2.velocity.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		else if (mBall2.position.x >= 1024.0f) {
			mIsRunning = false;
		}


		// Did the ball collide with the top wall?
		if (mBall2.position.y <= thickness && mBall2.velocity.y < 0.0f) {
			mBall2.velocity.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (mBall2.position.y >= (768 - thickness) && mBall2.velocity.y > 0.0f) {
			mBall2.velocity.y *= -1;
		}

	//------
}

void Game::GenerateOutput() {
	// Set draw color to blue
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall { 0, 0, 1024, thickness };
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768.0 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw right wall
//    wall.x = 1024 - thickness;
//    wall.y = 0;
//    wall.w = thickness;
//    wall.h = 1024;
//    SDL_RenderFillRect(mRenderer, &wall);

	// Draw paddle1
	SDL_Rect paddle1 { static_cast<int>(mPaddlePos1.x),
			static_cast<int>(mPaddlePos1.y - paddleH / 2), thickness,
			static_cast<int>(paddleH) };
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw paddle2
	SDL_Rect paddle2 { static_cast<int>(mPaddlePos2.x),
			static_cast<int>(mPaddlePos2.y - paddleH / 2), thickness,
			static_cast<int>(paddleH) };
	SDL_RenderFillRect(mRenderer, &paddle2);

	// Draw ball
	SDL_Rect ball { static_cast<int>(mBall1.position.x - thickness / 2),
			static_cast<int>(mBall1.position.y - thickness / 2), thickness, thickness };
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_Rect ball2 { static_cast<int>(mBall2.position.x - thickness / 2),
				static_cast<int>(mBall2.position.y - thickness / 2), thickness, thickness };
		SDL_RenderFillRect(mRenderer, &ball2);

	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
