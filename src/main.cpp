#include <raylib.h>
#include <string>
#include <iostream>

struct Ball
{
    float x, y;
    float speedX, speedY;
    float radius;

    void Draw(){
        DrawCircle((int)x, (int)y, (int)radius, WHITE);
    }

};

struct Paddle
{
    float x, y;
    float speed;
    float width, height;

    Rectangle getRect(){
        return Rectangle{x - width / 2, y - height / 2, width, height};
    }

    void Draw(){
        DrawRectangle(x - width / 2, y - height / 2, width, height, WHITE);
    }
};



int main(int argc, char const *argv[])
{
    int totalPoints;
    std::cout<<"Enter the number of points to win: ";
    std::cin>>totalPoints;

    InitWindow(800, 600, "Pong");
    SetWindowState(FLAG_VSYNC_HINT);
    InitAudioDevice(); 

    Sound fxBeep = LoadSound("resources/beep.wav");
    SetSoundVolume(fxBeep, 0.4f);

    int scoreRight = 0;
    int scoreLeft = 0;

    Ball ball;
    Paddle paddleLeft;
    Paddle paddleRight;

    paddleLeft.x = 50;
    paddleLeft.y = GetScreenHeight() / 2;
    paddleLeft.width = 10;
    paddleLeft.height = 100;
    paddleLeft.speed = 500;

    paddleRight.x = GetScreenWidth()-50;
    paddleRight.y = GetScreenHeight()/2;
    paddleRight.width = 10;
    paddleRight.height = 100;
    paddleRight.speed = 500;


    ball.x = GetScreenWidth()/2;
    ball.y = GetScreenHeight()/2;
    ball.radius = 5;
    ball.speedX = 300;
    ball.speedY = 300;


    const char* winner = nullptr;


    while (!WindowShouldClose())
    {
        ball.x += ball.speedX * GetFrameTime();
        ball.y += ball.speedY * GetFrameTime();

        if(ball.y > GetScreenHeight()){ 
            ball.y = GetScreenHeight();
            ball.speedY *= -1;
        }if (ball.y < 0)
        {
            ball.y = 0;
            ball.speedY *= -1;
        }
        
        if (IsKeyDown(KEY_W))
        {
            paddleLeft.y -= paddleLeft.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_S))
        {
            paddleLeft.y += paddleLeft.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_UP))
        {
            paddleRight.y -= paddleRight.speed * GetFrameTime();
        }
        if (IsKeyDown(KEY_DOWN))
        {
            paddleRight.y += paddleRight.speed * GetFrameTime();
        }

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, paddleLeft.getRect()))
        {
            if (ball.speedX < 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - paddleLeft.y) / (paddleLeft.height / 2) * ball.speedX;
                PlaySoundMulti(fxBeep);
            }
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, paddleRight.getRect()))
        {
            if (ball.speedX > 0)
            {
                ball.speedX *= -1.1f;
                ball.speedY = (ball.y - paddleRight.y) / (paddleRight.height / 2) * -ball.speedX;
                PlaySoundMulti(fxBeep);
            }
        }

        if (paddleLeft.y < 0 + paddleRight.height / 2) paddleLeft.y = paddleRight.height / 2;
        if (paddleRight.y < 0 + paddleRight.height / 2) paddleRight.y = paddleRight.height / 2;
        if (paddleLeft.y > GetScreenHeight() - paddleLeft.height / 2) paddleLeft.y = GetScreenHeight() - paddleLeft.height / 2;
        if (paddleRight.y > GetScreenHeight() - paddleRight.height / 2) paddleRight.y = GetScreenHeight() - paddleRight.height / 2;
        
        if(ball.x < 0)
        {
            if(scoreRight == totalPoints-1)winner = "Right player wins!!!";
			scoreRight += 1;
			ball.x = GetScreenWidth()/2;
			ball.y = GetScreenHeight()/2;
			ball.speedX = 300;
			ball.speedY = 300;
        }
        if(ball.x > GetScreenWidth())
        {
            if(scoreLeft == totalPoints-1) winner = "Left player wins!!!";
			scoreLeft += 1;
			ball.x = GetScreenWidth()/2;
			ball.y = GetScreenHeight()/2;
			ball.speedX = 300;
			ball.speedY = 300;
        }
        if (winner)
        {
            ball.x = GetScreenWidth()/2;
            ball.y = GetScreenHeight()/2;
            ball.speedX = 0;
            ball.speedY = 0;
            scoreLeft = 0;
            scoreRight = 0;
            paddleLeft.y = GetScreenHeight()/2;
            paddleRight.y = GetScreenHeight()/2;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
            ball.x = GetScreenWidth()/2;
            ball.y = GetScreenHeight()/2;
            ball.speedX = 300;
            ball.speedY = 300;
            winner = nullptr;
            scoreLeft = 0;
            scoreRight = 0;
        }
        
        

        BeginDrawing();
            ClearBackground(BLACK);

            ball.Draw();
            paddleLeft.Draw();
            paddleRight.Draw();
            int textWidth = MeasureText(winner, 60);
            DrawText(std::to_string(scoreRight).c_str(), GetScreenWidth()/2+20, 10, 30, GREEN);
            DrawText(":", GetScreenWidth()/2+7, 10, 30, GREEN);
            DrawText(std::to_string(scoreLeft).c_str(), GetScreenWidth()/2-20, 10, 30, GREEN);
            if (winner) DrawText(winner, GetScreenWidth()/2-textWidth/2, GetScreenHeight()/2-30, 60, GREEN);

            DrawFPS(10, 10);
        EndDrawing();

    }


    StopSoundMulti(); 

    UnloadSound(fxBeep);  

    CloseAudioDevice();     

    CloseWindow();

    return 0;
}
