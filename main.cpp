#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <cmath>

using namespace sf;
using namespace std;

struct Flappy {
    Sprite sprite;
    float velocity = -0.8f;
    float gravity = 0.4f;
} flappy;

enum Status {
    waiting,
    play,
    gameover,
};

struct Game {
    Sprite Flappy[2];
    Sprite Background;
    Sprite Land[2];
    Sprite Obstacle;
    Sprite GameOver;
    Sprite ScoreBoard;
    Text PressEnter;
    Text Score;
    float frames = 0;

    Status status = waiting;
    int score = 0;
    int finals = 0;
    float y_floor = 565.0f;
    int screenWidth = 400;
    int screenHeight = 665;
} Game;

void MoveLand(float deltaTime) {
    int ln = sizeof(Game.Land) / sizeof(Game.Land[0]);
    for (int i = 0; i < ln; i++) {
        Game.Land[i].move(-0.1 - deltaTime, 0);
        Vector2f vbg = Game.Land[i].getPosition();
        float width = Game.Land[i].getScale().x * 667;
        if (vbg.x < -width) {
            Game.Land[i].setPosition(vbg.x + width * 2, vbg.y);
        }
    }
}
int main() {
    int fps = 0;
    RenderWindow Window(VideoMode(Game.screenWidth, Game.screenHeight), "Flappy Bird", Style::Default);

    Texture BackgroundTexture;
    BackgroundTexture.loadFromFile("image/sky.png");
    Game.Background.setTexture(BackgroundTexture, true);
    Game.Background.setScale(0.6, 0.635);
    Texture LandTexture;
    LandTexture.loadFromFile("image/land.png");
    int ln = sizeof(Game.Land) / sizeof(Game.Land[0]);
    for (int i = 0; i < ln; i++) {
        Game.Land[i].setTexture(LandTexture, true);
        Game.Land[i].setScale(0.6, 0.45);
        Game.Land[i].setPosition(667 * 0.6 * i, Game.y_floor);
    }

    Texture FlappyTexture[2];
    FlappyTexture[0].loadFromFile("image/flappy1.png");
    FlappyTexture[1].loadFromFile("image/flappy2.png");
    flappy.sprite.setPosition(40, 350);

    Texture ObstacleTexture;
    ObstacleTexture.loadFromFile("image/obstacle.png");
    vector<Sprite> Obstacles;

    Texture GameOverTexture;
    GameOverTexture.loadFromFile("image/gameover.png");
    Game.GameOver.setTexture(GameOverTexture, true);
    Game.GameOver.setPosition(55, 150);
    Game.GameOver.setScale(0.6, 0.6);

    Texture ScoreBoard;
    ScoreBoard.loadFromFile("image/scoreboard.png");
    Game.ScoreBoard.setTexture(ScoreBoard, true);
    Game.ScoreBoard.setPosition(36, 250);
    Game.ScoreBoard.setScale(0.6, 0.6);

    Font Font;
    Font.loadFromFile("font/flappy.ttf");
    Game.PressEnter.setString("Press Enter to Continue");
    Game.PressEnter.setFont(Font);
    Game.PressEnter.setFillColor(Color::White);
    Game.PressEnter.setCharacterSize(18);
    Game.Score.setFont(Font);
    Game.Score.setFillColor(Color::White);
    Game.Score.setCharacterSize(40);
    Game.Score.setPosition(190, 5);

    Clock clock;
    float deltaTime;
    int gap = 200;

    while (Window.isOpen()) {
        deltaTime = clock.restart().asSeconds();

        if (Game.status == play || Game.status == waiting) {
            Game.frames += deltaTime;
        }

        Event Event;
        while (Window.pollEvent(Event)) {
            if (Event.type == Event::Closed) {
                Window.close();
            }
            if (Game.status == waiting) {
                if (Event.key.code == Keyboard::Enter) {
                    Game.status = play;
                }
            }
            else if (Game.status == play) {
                if (Event.type == Event::KeyReleased) {
                    if (Event.key.code == Keyboard::Space) {
                        flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);
                    }
                }
            }
        }
        flappy.sprite.setTexture(FlappyTexture[0], true);
        flappy.sprite.setScale(0.7, 0.7);
        // jika bermain
        if (Game.status == waiting) {
            flappy.sprite.setPosition(40, 350);
            if (Keyboard::isKeyPressed(Keyboard::Enter)) Game.status = play;
        }
        else if (Game.status == play) {
            Game.Score.setPosition(190, 5);
            flappy.velocity = -0.8f;
            flappy.gravity = 0.4f;
            flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);

            if (Keyboard::isKeyPressed(Keyboard::Space)) {
                flappy.sprite.move(0.0f, flappy.velocity * deltaTime * 1000);
            }
            if ((int)Game.frames >= 2) {
                Game.frames = 0;
                int r = (rand() % 275) + 75;
                gap--;

                Sprite ObstacleLower;
                ObstacleLower.setTexture(ObstacleTexture);
                ObstacleLower.setPosition(400, r + gap);
                ObstacleLower.setScale(1, 1);

                Sprite ObstacleUpper;
                ObstacleUpper.setTexture(ObstacleTexture);
                ObstacleUpper.setPosition(400, r);
                ObstacleUpper.setScale(1, -1);

                Obstacles.push_back(ObstacleLower);
                Obstacles.push_back(ObstacleUpper);
            }
            for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
                (*itr).move(-0.8 * deltaTime * 1000, 0);
            }
            flappy.sprite.setTexture(FlappyTexture[(int)floor(Game.frames) % 2], true);
            float fx = flappy.sprite.getPosition().x;
            float fy = flappy.sprite.getPosition().y;
            float fw = flappy.sprite.getScale().x;
            float fh = flappy.sprite.getScale().x;
            if (fy < 5) {
                flappy.sprite.setPosition(40, 0);
                Game.status = gameover;
            }
            else if (fy > 525) {
                Game.status = gameover;
            }

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

                if (ObsX >= 40 && ObsX <= 40.4) {
                    Game.score++;
                }
    
                if (fx + fw >= ObsX && fx <= ObsX + ObsWidth && fy <= ObsY + ObsHeight && fy + fh >= ObsY) {
                    Game.status = gameover;
                }
            }
        }

        if (Game.status == gameover) {
            Game.Score.setPosition(190, 305);
            flappy.velocity = 0;
            flappy.gravity = 0;
            Obstacles.clear();

            if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                Game.status = waiting;
            }
        }

        Window.clear();
        Window.draw(Game.Background);

        for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
            Window.draw(*itr);
        }

        Window.draw(flappy.sprite);

        Window.draw(Game.Land[0]);
        Window.draw(Game.Land[1]);

        if (Game.status == waiting) {
            Game.score = 0;
            Game.PressEnter.setString("Press Enter to Start");
            Game.PressEnter.setPosition(90, 250);
            Window.draw(Game.PressEnter);
        }

        if (Game.status == gameover) {
            Window.draw(Game.GameOver);
            Window.draw(Game.ScoreBoard);
            Window.draw(Game.Score);

            Game.PressEnter.setString("Press Enter to Continue");
            Game.PressEnter.setPosition(90, 380);
            Window.draw(Game.PressEnter);
        }

        Window.draw(Game.Score);
        Game.Score.setString(to_string(Game.score / 2));

        Window.display();
        MoveLand(deltaTime);
    }
    return 0;
}