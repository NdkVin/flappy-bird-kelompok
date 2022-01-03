#include <SFML/Graphics.hpp>
#include <iostream>
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

struct Game {
    Sprite Flappy[2];
    Sprite Background;
    Sprite Land[2];
    Sprite Obstacle;
    float frames = 0;
    int score = 0;
    int finals = 0;
    float y_floor = 565.0f;
    int screenWidth = 400;
    int screenHeight = 665;
} Game;

void MoveLand(float deltaTime) {
  int ln = sizeof(Game.Land)/sizeof(Game.Land[0]);
  cout << deltaTime << endl;
  for(int i =0; i< ln; i++) {
    Game.Land[i].move(-0.1, 0);
    Vector2f vbg = Game.Land[i].getPosition();
    float width = Game.Land[i].getScale().x*667;
    if(vbg.x < -width) {
      Game.Land[i].setPosition(vbg.x+width*2, vbg.y);
    } 
  }
}

int main() {
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
        Game.Land[i].setPosition(667* 0.6 * i, Game.y_floor);
    }

    Texture FlappyTexture[2];
    FlappyTexture[0].loadFromFile("image/flappy1.png");
    FlappyTexture[1].loadFromFile("image/flappy2.png");
    flappy.sprite.setPosition(40, 350);

    Texture ObstacleTexture;
    ObstacleTexture.loadFromFile("image/obstacle.png");
    vector<Sprite> Obstacles;

    Clock clock;
    float deltaTime;
    int gap = 200;

    while (Window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        Game.frames += deltaTime;

        Event Event;
        while (Window.pollEvent(Event)) {
            if (Event.type == Event::Closed) {
                Window.close();
            }
            if (Event.type == Event::KeyReleased) {
                if (Event.key.code == Keyboard::Space) {
                    flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);
                }
            }
        }

        flappy.sprite.setTexture(FlappyTexture[0], true);

        flappy.sprite.move(0.0f, flappy.gravity * deltaTime * 1000);
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            flappy.sprite.move(0.0f, flappy.velocity * deltaTime * 1000);
        }

        float fx = flappy.sprite.getPosition().x;
        float fy = flappy.sprite.getPosition().y;

        if (fy < 0) {
            flappy.sprite.setPosition(40, 0);
            flappy.velocity = 0;
        }
        else if (fy > 728) {
            flappy.velocity = 0;
            flappy.gravity = 0;
        }

        flappy.sprite.setTexture(FlappyTexture[(int)floor(Game.frames) % 2], true);

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

        Window.clear();

        Window.draw(Game.Background);
        Window.draw(flappy.sprite);

        for (vector<Sprite>::iterator itr = Obstacles.begin(); itr != Obstacles.end(); itr++) {
            Window.draw(*itr);
        }

        Window.draw(Game.Land[0]);
        Window.draw(Game.Land[1]);

        Window.display();

        MoveLand(deltaTime);

    }
    return 0;
}