#pragma once
#include "SDL2/SDL.h"
#include <vector>

// Vector2 struct just stores the x/y coordinates for now
struct Vector2
{
    float x;
    float y;
};

struct Ball {
	Vector2 position;
	Vector2 velocity;
};



// Game class
class Game
{
    public:
        Game();
        // Initialize the game
        bool Initialize();
        // Runs the game loop until the game is over
        void RunLoop();
        // Shutdown the game
        void Shutdown();

    private:
        // Helper functions for the game loop
        void ProcessInput();
        void UpdateGame();
        void GenerateOutput();

        // Window created by SDL
        SDL_Window* mWindow;
        // Renderer for 2D drawing
        SDL_Renderer* mRenderer;
        // Number of ticks since the start of game
        Uint32 mTicksCount;
        // Game should continue to run
        bool mIsRunning;

        // Pong specific
        // Direction of paddle
        int mPaddleDir1;
        int mPaddleDir2;
        // Position of paddle
        Vector2 mPaddlePos1;
        Vector2 mPaddlePos2;
        // Position of ball
        Vector2 mBallPos;
        // Velocity of ball
        Vector2 mBallVel;

        Ball mBall1;
        Ball mBall2;
        std::vector<Ball> mBalls;
};
