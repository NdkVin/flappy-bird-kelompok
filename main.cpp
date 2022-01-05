//Library
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

// Bring a specific member from the namespace into the current scope
using namespace sf;
using namespace std;

// Flappy singleton struct
// v & g = vertical speed
struct Flappy {
    Sprite sprite;
    float velocity = -0.8f;
    float gravity = 0.4f;
} flappy;

// waiting = game not started yet
// play = playing
// gameover = game is over
enum Status {
    waiting,
    play,
    gameover,
};

// Game settings in singleton game struct
struct Game {
    Status status = waiting;
    Sprite Logo;
    Sprite Flappy[2];
    Sprite Title;
    Sprite Background;
    Sprite Land[2];
    Sprite Obstacle;
    Sprite gameOver;
    Sprite ScoreBoard;
    Text PressEnter;
    Text Score;
    int score = 0;
    int finals = 0;
    int screenWidth = 400;
    int screenHeight = 665;
    float frames = 0;
    float y_floor = 565.0f;
} game;

// Function to move sprite of land
void MoveLand(float deltaTime) {
    int ln = sizeof(game.Land) / sizeof(game.Land[0]);
    for (int i = 0; i < ln; i++) {
        game.Land[i].move(-0.1 - deltaTime, 0);
        Vector2f vbg = game.Land[i].getPosition();
        float width = game.Land[i].getScale().x * 666;
        if (vbg.x < -width) {
            game.Land[i].setPosition(vbg.x + width * 2, vbg.y);
        }
    }
}

int main() {
    // Create the window and set general settings.
    RenderWindow Window(VideoMode(game.screenWidth, game.screenHeight), "Flappy Bird", Style::Default);

    // Asset Manager
    Texture BackgroundTexture;
    BackgroundTexture.loadFromFile("image/sky.png");
    game.Background.setTexture(BackgroundTexture, true);
    game.Background.setScale(0.6, 0.635);

    Texture LandTexture;
    LandTexture.loadFromFile("image/land.png");
    int ln = sizeof(game.Land) / sizeof(game.Land[0]);
    for (int i = 0; i < ln; i++) {
        game.Land[i].setTexture(LandTexture, true);
        game.Land[i].setScale(0.6, 0.45);
        game.Land[i].setPosition(666 * 0.6 * i, game.y_floor);
    }

    Texture LogoTexture;
    LogoTexture.loadFromFile("image/logo.png");
    game.Logo.setTexture(LogoTexture, true);
    game.Logo.setPosition(140, 240);

    Texture FlappyTexture[2];
    FlappyTexture[0].loadFromFile("image/flappy1.png");
    FlappyTexture[1].loadFromFile("image/flappy2.png");
    flappy.sprite.setTexture(FlappyTexture[0], true);
    flappy.sprite.setPosition(40, 350);
    flappy.sprite.setScale(0.7, 0.7);

    Texture TitleTexture;
    TitleTexture.loadFromFile("image/Title.png");
    game.Title.setTexture(TitleTexture, true);
    game.Title.setPosition(40, 100);
    game.Title.setScale(0.5, 0.5);


    Texture ObstacleTexture;
    ObstacleTexture.loadFromFile("image/obstacle.png");
    vector<Sprite> Obstacles;

    Texture gameOverTexture;
    gameOverTexture.loadFromFile("image/gameover.png");
    game.gameOver.setTexture(gameOverTexture, true);
    game.gameOver.setPosition(55, 150);
    game.gameOver.setScale(0.6, 0.6);

    Texture ScoreBoard;
    ScoreBoard.loadFromFile("image/scoreboard.png");
    game.ScoreBoard.setTexture(ScoreBoard, true);
    game.ScoreBoard.setPosition(36, 250);
    game.ScoreBoard.setScale(0.6, 0.6);

    Font Font;
    Font.loadFromFile("font/flappy.ttf");
    game.PressEnter.setString("Press Enter to Continue");
    game.PressEnter.setFont(Font);
    game.PressEnter.setFillColor(Color::White);
    game.PressEnter.setCharacterSize(18);
    game.Score.setFont(Font);
    game.Score.setFillColor(Color::White);
    game.Score.setCharacterSize(40);
    game.Score.setPosition(190, 5);

    // Handling time
    Clock clock;
    float deltaTime;

    // Main loop
    while (Window.isOpen()) {
        // Handling time setting
        deltaTime = clock.restart().asSeconds();

        if (game.status == play || game.status == waiting) {
            game.frames += deltaTime;
        }

        // Events
        Event Event;
        while (Window.pollEvent(Event)) {
            if (Event.type == Event::Closed) {
                Window.close();
            }
            if (game.status == waiting) {
                if (Event.key.code == Keyboard::Enter) {
                    game.status = play;
                }
            }
            else if (game.status == play) {
                if (Event.type == Event::KeyReleased) {
                    if (Event.key.code == Keyboard::Space) {
                        flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);
                    }
                }
            }
        }

        // Update flappy
        flappy.sprite.setTexture(FlappyTexture[(int)floor(game.frames) % 2], true);
        float fx = flappy.sprite.getPosition().x;
        float fy = flappy.sprite.getPosition().y;
        float fw = flappy.sprite.getScale().x;
        float fh = flappy.sprite.getScale().x;

        // Press enter to play the game
        if (game.status == waiting) {
            flappy.sprite.setPosition(40, 350);
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                game.status = play;
            }
        }
        // Game started
        else if (game.status == play) {
            // Setting
            game.Score.setPosition(190, 5);
            flappy.velocity = -0.8f;
            flappy.gravity = 0.4f;
            flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);

            // Move flappy
            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                flappy.sprite.move(0.0f, flappy.velocity * deltaTime * 1000);
            }

            // If out of screen -> game over
            if (fy < 5) {
                flappy.sprite.setPosition(40, 0);
                game.status = gameover;
            }
            else if (fy > 525) {
                game.status = gameover;
            }

            // Generate obstacle
            if ((int)game.frames >= 2) {
                game.frames = 0;
                int r = (rand() % 275) + 75;
                int gap = 200;

                // Obstacle lower
                Sprite ObstacleLower;
                ObstacleLower.setTexture(ObstacleTexture);
                ObstacleLower.setPosition(400, r + gap);
                ObstacleLower.setScale(1, 1);

                // Obstacle upper
                Sprite ObstacleUpper;
                ObstacleUpper.setTexture(ObstacleTexture);
                ObstacleUpper.setPosition(400, r);
                ObstacleUpper.setScale(1, -1);

                // Push to the array
                Obstacles.push_back(ObstacleLower);
                Obstacles.push_back(ObstacleUpper);
            }

            // Move obstacle
            for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
                (*itr).move(-0.8 * deltaTime * 1000, 0);
            }

            // Collision detection
            for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
                float ObsX, ObsY, ObsWidth, ObsHeight;

                if ((*itr).getScale().y > 0) {
                    ObsX = (*itr).getPosition().x;
                    ObsY = (*itr).getPosition().y;
                    ObsWidth = 69 * (*itr).getScale().x;
                    ObsHeight = 368 * (*itr).getScale().y;
                }
                else {
                    ObsHeight = -368 * (*itr).getScale().y;
                    ObsX = (*itr).getPosition().x;
                    ObsY = (*itr).getPosition().y - ObsHeight;
                    ObsWidth = 69 * (*itr).getScale().x;
                }

                // Count the score
                if (ObsX >= 40 && ObsX <= 40.4) {
                    game.score++;
                }

                if (fx + fw >= ObsX && fx <= ObsX + ObsWidth && fy <= ObsY + ObsHeight && fy + fh >= ObsY) {
                    game.status = gameover;
                }
            }
        }
        // Game is over
        else if (game.status == gameover) {
            game.Score.setPosition(190, 305);
            flappy.velocity = 0;
            flappy.gravity = 0;
            Obstacles.clear();
            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                game.status = waiting;
            }
        }

        // Clear, draw, display
        Window.clear();

        Window.draw(game.Background);

        Window.draw(game.Logo);

        // Draw obstacle
        for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
            Window.draw(*itr);
        }

        Window.draw(flappy.sprite);

        Window.draw(game.Land[0]);
        Window.draw(game.Land[1]);

        // Draw when it waiting
        if (game.status == waiting) {
            game.score = 0;
            Window.draw(game.Title);
            game.PressEnter.setString("Press Enter to Start");
            game.PressEnter.setPosition(90, 200);
            Window.draw(game.PressEnter);
        }

        // Draw when it gameover
        if (game.status == gameover) {
            Window.draw(game.gameOver);
            Window.draw(game.ScoreBoard);
            Window.draw(game.Score);

            game.PressEnter.setString("Press Enter to Continue");
            game.PressEnter.setPosition(90, 380);
            Window.draw(game.PressEnter);
        }

        // Draw scores
        Window.draw(game.Score);
        game.Score.setString(to_string(game.score / 2));

        Window.display();
        
        MoveLand(deltaTime);
    }
    return 0;
}